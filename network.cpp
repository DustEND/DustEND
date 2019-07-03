#include "network.h"

#include <QString>
#include <QtWidgets/QMessageBox>

#include <stdio.h>
#include <lm.h>

#include "netcomp.h"

#include <QDebug>

#define MAX_NET_RESOURCES (1024)

Network::Network()
{
    netTbl = new QList<NetComp>();
    notAdded = false;
}

void Network::scanNetwork()
{
    qDebug() << "scann";
    netTbl->clear();
    // Для определения IP адресов нужен WinSock
    // Попытаемся проинициализировать WinSock
    WORD wVersion = MAKEWORD(1, 1);
    WSADATA wsaData;
    DWORD dwRes = WSAStartup(wVersion, &wsaData);
    bWSA = (dwRes == 0);
    if(!bWSA){  // Если WinSock не работает, то определение IP будет не доступно :-(
        QMessageBox msgBox;
        msgBox.setText(QObject::trUtf8("WSAStartup() failed with %1, gethostbyname() unavailable.").arg(dwRes));
        msgBox.exec();
    }
    // Начнем сканирование с самого верхнего уровня
    ResFunc(NULL, 0);
    // Освободим WinSock
    if(bWSA)
        WSACleanup();

//    QString name = QObject::trUtf8("igor");
//    QString ipAddr = QObject::trUtf8("129.168.1.1");
//    QString os = QObject::trUtf8("Windows XP");
//    QString cleanTime = QObject::trUtf8("");
//    QString filterTime = QObject::trUtf8("");
//    NetComp comp(name, ipAddr, os, cleanTime, filterTime);
//    netTbl->push_back(comp);

//    name = QObject::trUtf8("igarok");
//    ipAddr = QObject::trUtf8("192.168.1.110");
//    os = QObject::trUtf8("Windows 7/ Windows Server 2008 RT");
//    cleanTime = QObject::trUtf8("");
//    filterTime = QObject::trUtf8("");
//    NetComp comp1(name, ipAddr, os, cleanTime, filterTime);
//    netTbl->push_back(comp1);

//    name = QObject::trUtf8("igarokigarokigaro");
//    ipAddr = QObject::trUtf8("129.168.255.256");
//    os = QObject::trUtf8("Windows 7/ Windows Server 2008 RT");
//    cleanTime = QObject::trUtf8("");
//    filterTime = QObject::trUtf8("");
//    NetComp comp2(name, ipAddr, os, cleanTime, filterTime);
//    netTbl->push_back(comp2);
}

int Network::ResFunc(NETRESOURCE *pNr, int sp)
{
    HANDLE hEnum;
    DWORD dwRes;

    // Начнем перечисление сетевых ресурсов
    dwRes = WNetOpenEnum(RESOURCE_GLOBALNET,    // область видимости - вся сеть
                         RESOURCETYPE_DISK,    // тип ресурсов - диски и принтеры
                         0,                    // тип ресурсов - подключаемые и содержащие
                         pNr,                // начать с pNr
                         &hEnum);                // передать хендл в hEnum

    NETRESOURCE NetResource[MAX_NET_RESOURCES];  // буффер для хранения информации про ресурсы
    DWORD dwCount = 0xFFFFFFFF, dwSize = sizeof(NETRESOURCE) * MAX_NET_RESOURCES;

    // Перечислим ресурсы в текущем container'е
    dwRes = WNetEnumResource(hEnum,            // хендл на перечисление
                             &dwCount,                // количество ресурсов
                             (LPVOID*)&NetResource,    // буффер для хранения информации про ресурсы
                             &dwSize);

    if(dwRes != NO_ERROR)  // ошибка
        return 2;

    // Будем перебирать все ресурсы
    DWORD dw;    
    for(dw = 0; dw < dwCount; dw++){       
        // Если ресурс - сервер
        if(NetResource[dw].dwDisplayType == RESOURCEDISPLAYTYPE_SERVER){
            char szIP[256];
            if(bWSA) {  // Если WinSock доступен, то определим IP адрес             
                char szName[256];
                for(int i = 0; NetResource[dw].lpRemoteName[i+1] != '\0'; i++){
                    szName[i] = NetResource[dw].lpRemoteName[i+2];
                }

                struct hostent *hp = gethostbyname(szName);
                if(!hp){
                    strcpy(szIP, "ip=???");
                }
                else {
                    DWORD dwIP = ((in_addr*)hp->h_addr_list[0])->S_un.S_addr;

                    int a = LOBYTE(LOWORD(dwIP));
                    int b = HIBYTE(LOWORD(dwIP));
                    int c = LOBYTE(HIWORD(dwIP));
                    int d = HIBYTE(HIWORD(dwIP));
                    sprintf(szIP, "%d.%d.%d.%d", a, b, c, d);
                }
            }
            else
                sprintf(szIP, "ip=???");  
            // Буфферы для информации в текстовом виде
            char szOS_Version[256], szOS_Platform[256], szOS[256];

            // Буффер для информации о сервере
            SERVER_INFO_101 *pSI101;
            NetApiBufferAllocate(sizeof(SERVER_INFO_101), (LPVOID*)&pSI101);

            // Получим информацию о сервере
            NET_API_STATUS nasRes;
            nasRes = NetServerGetInfo((LPWSTR)NetResource[dw].lpRemoteName, 101, (UCHAR**)&pSI101);

            if(nasRes != NERR_Success)
                continue;

            // Версия системы
            sprintf(szOS_Version, "%d.%d",
                    (int)pSI101->sv101_version_major & MAJOR_VERSION_MASK,
                    (int)pSI101->sv101_version_minor);

            // Тип системы
            switch(pSI101->sv101_platform_id) {
            case PLATFORM_ID_DOS: strcpy(szOS_Platform, "DOS"); break;
            case PLATFORM_ID_OS2: strcpy(szOS_Platform, "OS2"); break;
            case PLATFORM_ID_NT: strcpy(szOS_Platform, "NT"); break;
            case PLATFORM_ID_OSF: strcpy(szOS_Platform, "OSF"); break;
            case PLATFORM_ID_VMS: strcpy(szOS_Platform, "VMS"); break;
            default: strcpy(szOS_Platform, "ost=???");
            }

            NetApiBufferFree((LPVOID)pSI101);

            sprintf(szOS, "%s %s", szOS_Platform, szOS_Version);    
            if (!strcmp(szOS, "NT 3.1")) strcpy(szOS, "Windows NT 3.1");
            if (!strcmp(szOS, "NT 3.5")) strcpy(szOS, "Windows NT 3.5");
            if (!strcmp(szOS, "NT 3.51")) strcpy(szOS, "Windows NT 3.51");
            if (!strcmp(szOS, "NT 4.0")) strcpy(szOS, "Windows NT 4.0");
            if (!strcmp(szOS, "NT 5.0")) strcpy(szOS, "Windows 2000");
            if (!strcmp(szOS, "NT 5.2")) strcpy(szOS, "Windows Server 2003");
            if (!strcmp(szOS, "NT 6.0")) strcpy(szOS, "Windows Vista / Windows Server 2008");
            if (!strcmp(szOS, "NT 6.1")) strcpy(szOS, "Windows 7 / Windows Server 2008 R2");
            if (!strcmp(szOS, "NT 5.1")) strcpy(szOS, "Windows XP");
            if (!strcmp(szOS, "NT 3.95")) strcpy(szOS, "Windows 95");
            if (!strcmp(szOS, "NT 6.2")) strcpy(szOS, "Windows 8");

            // Напишем то, что удалось узнать
            QString name = QString::fromWCharArray(NetResource[dw].lpRemoteName);
            QString ip = QString::fromStdString(szIP);
            QString os = QString::fromStdString(szOS);
            QString clean = " ";
            QString filter = " ";            
            if (name[0] == '\\')
                if (name[1] == '\\')
                    name.remove(0,2);

            addComp(&name, &ip, &os, &clean, &filter);

            tempComp.setName(&name);
            tempComp.setIpAddress(&ip);
            tempComp.setOsName(&os);
            tempComp.setCleanTime(&clean);
            tempComp.setFilterTime(&filter);

            emit getNetComp(tempComp);
            pause();
            if (notAdded)
                netSemaph.acquire();

            qDebug() << " 1? " << name << ip << os;
        }
        // Если ресурс является container'ом, то посмотрим, что в нем есть :-)
        if(NetResource[dw].dwUsage & RESOURCEUSAGE_CONTAINER)
            ResFunc(&NetResource[dw], sp+1);
    }
    // Закроем перечисление
    WNetCloseEnum(hEnum);
    return 0;
}

void Network::addComp(QString *name, QString *ipAddr, QString *os,
                 QString *cleanTime, QString *filterTime)
{
    NetComp comp(*name, *ipAddr, *os, *cleanTime, *filterTime);
    netTbl->push_back(comp);
}

QList<NetComp> Network::getNetTbl() const
{    
    return *netTbl;
}

void Network::run()
{
    notAdded = true;
    scanNetwork();    
    emit scanFinished(*netTbl);
}

void Network::wake()
{
    notAdded = false;
    netSemaph.release();
}

void Network::pause()
{
    notAdded = true;
}
