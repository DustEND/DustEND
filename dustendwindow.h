#ifndef DUSTENDWINDOW_H
#define DUSTENDWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QDateTime>
#include <QTimer>
#include <QtWidgets/QSystemTrayIcon>
#include <QtNetwork/QHostInfo>
#include <QAbstractItemModel>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QMessageBox>

#include "network.h"
#include "mytextedit.h"

#include "netcomp.h"
#include "computer.h"
#include "tablemodel.h"
#include "progressdelegate.h"
#include "netcomplogic.h"
#include "mailthread.h"

typedef QList<NetComp> NetCompList;
typedef NetComp NetC;

namespace Ui {
    class MainWindow;
}

class ComputerLogic;
class DataBase;
class GeneralSettingsLogic;
class LanguageVariablesLogic;

class DustendWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DustendWindow(QWidget *parent = 0);
    ~DustendWindow();

private:
    Ui::MainWindow *ui;

    QList<NetComp> networkComps;
    ComputerLogic *compL;
    bool firstNet;
    bool firstStart;
    bool compShown;
    bool programFirstExec;
    TableModel *table;
    ProgressDelegate delegat;

    myTextEdit * mTEdit;

    QList<QHostAddress> ipAddressesList;
    Network netW;
    NetCompLogic netLogic;

    QString localIp;
    QString curIp;
    QTimer timer;
    QTimer netTimer;
    QTimer warningTimer;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QAction *showMessageAction;

    QMessageBox stopDialog;
    unsigned int warningTime;
    bool sendEmail;
    QString emailAddress;
    MailThread *mailThr;

    DataBase *myDB;
    GeneralSettingsLogic *genSetLogic;
    LanguageVariablesLogic *langVarLogic;
    bool tableLoaded;
    QString warningMsg;
    QString stopDialogMsg;
    QString okBtn;
    QString cancelBtn;
    QString msg;
    QString showMessage;
    QString minimize;
    QString maximize;
    QString restore;
    QString quit;
    QString trayFilter;
    QString trayClean;
    QString wasFinded;
    QString computer;
    QString scanning;
    QString updating;
    QString myTextLabel;
    QString netTextLabel;
    QString nameHeader;
    QString ipHeader;
    QString cleanHeader;
    QString filterHeader;
    QString osHeader;
    QString cfHeader;
    QString timeHeader;
    QString cleanHint;
    QString filterHint;
    bool setsSaved;
    unsigned int saveTimer;

    void createConnections();
    void setPlace();
    void setCleaningTimeLbl();
    void setFilterTimeLbl();
    void setCompNameLbl();
    void showComputerStats(QString *ipAddress);
    void setNetworkTime();
    void setTableTime(QString *ipAddress, int *netCompIpPos);
    void initStopDialog();

    void registerAutorun();
    void unRegisterAutorun();
    void closeEvent(QCloseEvent *event);

    void setTrayIcon();
    void setTrayIconImage();
    void setTrayIconHint();

    void fillTableModel();
    void setupModel();

    bool isCompInAllNetComps(QString *ipAddress, QList<NetComp> allNetComps);
    int findCompInTable(QString *ipAddress);
    void insertRowToTable(NetComp newComp, int row);

    void setLanguageMenu();
    void setEventFilterToFrames();

    void readProcessorName();
    void setInfoPage();
    void setProgressBar();
    void setProgressBarRed();
    void setProgressBarYellow();
    void setProgressBarGreen();
    void showComputerComment();
    int getCoolerDiameter(QString cBoxText);

    QString getCBoxStyleSheetOnArrowHovered();
    QString getCBoxStyleSheetOnArrowExit();
    QString getArrowStyleOnCBoxHovered(QString arrowPixmap);
    QString getArrowStyleOnCBoxExit(QString arrowPixmap);

    void setCBoxStylesOnArrowHover(QObject *obj);
    void setCBoxNumAndArrowStylesOnCBoxNumHover(QObject *obj);
    void setCBoxSizeAndArrowStylesOnCBoxSizeHover(QObject *obj);

    void setCBoxStylesOnArrowExit(QObject *obj);
    void setCBoxNumAndArrowStylesOnCBoxNumExit(QObject *obj);
    void setCBoxSizeAndArrowStylesOnCBoxSizeExit(QObject *obj);

    void loadNetTblSettings();
    void loadLangSettings();
    bool isAddressBelongsToMe(QString *ipAddress);

    void asmInsertion();
    void setComboBoxes();
    void showPbStart(bool show);
    void setWarningTimeSettings();
    void loadEmailSettings();
    void setPbTimeStyle(bool mouseOnIt);

private slots:
    void setDirtyPlace();
    void setCurCompTab(int tabNumber);
    void setCurActionTab(int tabNumber);
    void showFanControl();
    void showSettings();
    void showTimer();

    void setHomeComputer();
    void setFactoryComputer();
    void showFilterStatus();
    void showCleaningStatus();
    void showInfo();
    void showNetwork();
    void showComputer();
    void startCleaningTime();
    void stopCleaningTime();
    void pauseCleaningTime();
    void timerTick();
    void warningTimerTick();
    void setArrows();
    void setCoolerCountComboBoxes();
    void setCoolerDiameterComboBoxes();
    void setCoolerDiameter();    
    void setBtStart();
    void setGrayArrows();

    void setLeftCoolerAmount();
    void setTopCoolerAmount();
    void setRightCoolerAmount();
    void setBottomCoolerAmount();
    void setSideCoolerAmount();

    void setLeftCoolerDiameter();
    void setTopCoolerDiameter();
    void setRightCoolerDiameter();
    void setBottomCoolerDiameter();
    void setSideCoolerDiameter();

    void setMaxAndCurTimes();
    void saveSettings();
    void saveCurSettings();

    // tray
    void slotTrayActivated( QSystemTrayIcon::ActivationReason reason );
    void slotShowMessageAction();


    void findMyComputerAddresses();

    void showNetComp(const QModelIndex &index);
    void getNetComps(/*QList<NetComp> allNetComps*/NetCompList);
    void removeFromAllNetComps(QList<NetComp> *allNetComps, QString *ipAddress);
    void setNetworkLbl();

    void addCooler(QComboBox *combo);
    void setEngLang();
    void setRusLang();    
    void setUkrLang();
    void pauseNetComps();
    void unPauseNetComps();
    void setFirstNetTrue();

    void addNetComp(NetC newNetComp);
    void slotAnchorClicked(const QString & href);
    void sortNet(int sortedCol, Qt::SortOrder order);
    void saveNetSettings();
    void execStopDialog();
    void setDontShowWarningMsg();
    void set10MinsWarningMsg();
    void set30MinsWarningMsg();
    void set60MinsWarningMsg();
    void setStartupWarningMsg();
    void setWarningTimer();
    void setSendEmail();
    void setEmail(QString newEmailAddress);
    void setAutorunProgam();
    void setUILanguage();
    void settingsSaved();
    void sendTestEmail();

protected:
    virtual bool eventFilter(QObject *obj, QEvent *ev);

signals:
    void compAdded();

};

#endif // DUSTENDWINDOW_H
