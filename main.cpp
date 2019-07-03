//#include <QtGui/QApplication>
#include <QtWidgets/QApplication>
//#include <QtGui/QCleanlooksStyle>

#include <QtWidgets/QStyle>
#include <QTextCodec>
#include <QTranslator>
#include <QSettings>
#include <QResource>

#include <QDebug>

#include "dustendwindow.h"
//#include "predicates.h"


int main(int argc, char *argv[])
{    
    HANDLE myMUT;
    myMUT = CreateMutexW(NULL,FALSE,L"ComputerDustControl");
    if(GetLastError() == ERROR_ALREADY_EXISTS) {
        CloseHandle(myMUT);
    }
    else {
        QApplication a(argc, argv);
        QApplication::setOrganizationName("dustend");
        QApplication::setOrganizationDomain("dust");
        QApplication::setApplicationName("Computer Dust Control");
        QApplication::setApplicationVersion("comp");

//        QApplication::setStyle(new QCleanlooksStyle);

        QTextCodec *codecWin1251 = QTextCodec::codecForName("CP1251");
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
//        QTextCodec::setCodecForTr(codec);
//        QTextCodec::setCodecForCStrings(codecWin1251);
//        QTextCodec::setCodecForLocale(codecWin1251);

        QResource::registerResource("rc.qrc");

        DustendWindow w;
        w.show();
        return a.exec();
    }
}


