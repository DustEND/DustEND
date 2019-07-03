#include "dustendwindow.h"
#include "ui_mainwindow.h"

#include "computerlogic.h"
#include "deviceinfo.h"

#include <QDebug>
#include <QTimer>
#include <QSettings>
#include <QDir>
#include <QCloseEvent>
#include <QtWidgets/QMenu>

#include <QtNetwork/QtNetwork>
#include <QtNetwork/QNetworkInterface>
#include <QtWidgets/QSplashScreen>
#include <QStandardItemModel>
#include <QtWidgets/QMessageBox>
#include <QDesktopServices>
#include <QtWidgets/QToolButton>

#include <iostream>
#include <stdexcept>

#include "DB/DataBase.h"
#include "generalsettingslogic.h"
#include "languagevariableslogic.h"

#include <WinIoCtl.h>



DustendWindow::DustendWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myDB = new DataBase("localhost", "DustendDB.db3", "Du1st2E9nd0Ad3,min",
                        "5Co4mp2ut8er.C#tp");    
    myDB->connectToBase();
    genSetLogic = new GeneralSettingsLogic();
    langVarLogic = new LanguageVariablesLogic();    
    setsSaved = true;
    saveTimer = 0;

    qDebug() << "support: " << QSslSocket::supportsSsl();
    compL = new ComputerLogic();
    tableLoaded = false;
    setUILanguage();
    compL->loadSettings();
    localIp = "127.0.0.1";
    firstNet = true;
    firstStart = true;
    warningTime = genSetLogic->getWarningInterval();    
    setWarningTimeSettings();    
    setWarningTimer();
    loadEmailSettings();
    ui->rb_autorunProgr->setChecked(genSetLogic->getAutoStartApp());
//    asmInsertion();

    showComputerStats(&localIp);
    setTrayIcon();
    createConnections();
    timer.setInterval(1000);
    timer.start();
    netTimer.setInterval(600000);
    netTimer.start();
    table = new TableModel(this);
    setupModel();
    tableLoaded = true;
    table->setNameHeader(nameHeader);
    table->setIpHeader(ipHeader);
    table->setCleanHeader(cleanHeader);
    table->setFilterHeader(filterHeader);
    table->setOsHeader(osHeader);
    table->setCFHeader(cfHeader);
    table->setTimeHeader(timeHeader);
    table->setCleanHint(cleanHint);
    table->setFilterHint(filterHint);
    ui->networkTbl->setModel(table);
    ui->networkTbl->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->networkTbl->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->networkTbl->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->networkTbl->setColumnWidth(6, 30);
    ui->stackedWidget->setCurrentIndex(0);
    loadNetTblSettings();
    loadLangSettings();
    setInfoPage();
    readProcessorName();
    initStopDialog();
    ui->btnInfo->setVisible(false);
    ui->btnFanControl->setVisible(false);
    ui->pbStart->setVisible(false);

    if (ui->actionTabWdg->currentIndex() == 0)
        ui->pbStart->setMaximum(1300);
    if (ui->actionTabWdg->currentIndex() == 1)
        ui->pbStart->setMaximum(2600);
    ui->pbStart->setValue(compL->getLastMaxTimeHours(&localIp).toInt());
    ui->pbStart->setFormat(compL->getLastMaxTime(&localIp));
    ui->pbStart->repaint();

    ui->bt_ukrLang->setVisible(false);
    int startupMinutes = GetTickCount() / 1000 / 60;
    if (genSetLogic->getAutoStartApp() && startupMinutes < 3)
        QTimer::singleShot(0, this, SLOT(hide()));


//    ui->pushButton->setVisible(false);

//color: black;
//selection-background-color: white;
//border-radius: 0px;
//border-color: white;
//border-style: solid;
//border-width: 1px;
//background-color: rgb(255, 255, 255);
//    this->setStyleSheet("QToolButton#ui->bt_start{color: black; background-color: white;");


//    QProgressBar {
//    border: 1px solid grey;
//    border-radius: 5px;
//    color: black;
//    }
//    QProgressBar::chunk {
//    background-color: qlineargradient(spread:reflect, x1:1, y1:1, x2:1, y2:0, stop:0 rgba(0, 129, 0, 255), stop:1 rgba(0, 223, 0, 255));
//    width: 1px;
//    color: black;
//    }

    //******************************************************************//
}

DustendWindow::~DustendWindow()
{
    qDebug() << "dustendwindow destructor 1";
    myDB->disconnectFromBase();
    qDebug() << "dustendwindow destructor 2";
    delete ui;
    qDebug() << "dustendwindow destructor 3";
}

void DustendWindow::setHomeComputer()
{
    if (!compL->getCleanTimeIsRunning(&localIp)
            && !compL->getFilterTimeIsRunning(&localIp)) {
        unsigned short placeType = 1;
        unsigned short cleanOrFilter = 1;
        if (ui->filterTab->isVisible())
            cleanOrFilter = 2;
        compL->setPlaceType(&localIp,&placeType/*, &cleanOrFilter*/);
        setCleaningTimeLbl();
        setFilterTimeLbl();
        setProgressBar();
    }
    else
        ui->cbDustyPlace->setChecked(false);
}

void DustendWindow::setFactoryComputer()
{
    if (!compL->getCleanTimeIsRunning(&localIp)
            && !compL->getFilterTimeIsRunning(&localIp)) {
        unsigned short placeType = 3;
        unsigned short cleanOrFilter = 1;
        if (ui->filterTab->isVisible())
            cleanOrFilter = 2;
        compL->setPlaceType(&localIp, &placeType/*, &cleanOrFilter*/);
        setCleaningTimeLbl();
        setFilterTimeLbl();
        setProgressBar();
    }
    else
        ui->cbDustyPlace->setChecked(false);
}

void DustendWindow::showFilterStatus()
{
    if (ui->actionTabWdg->currentIndex() == 1) {
        QFont tempFont = ui->filterLbl->font();
        tempFont.setBold(true);
        ui->filterLbl->setFont(tempFont);
        tempFont = ui->cleanLbl->font();
        tempFont.setBold(false);
        ui->cleanLbl->setFont(tempFont);

        ui->cleanLbl->setVisible(false);
        ui->filterLbl->setVisible(true);
        ui->startLbl->setVisible(false);
    }
}

void DustendWindow::showCleaningStatus()
{
    if (ui->actionTabWdg->currentIndex() == 0) {
        QFont tempFont = ui->cleanLbl->font();
        tempFont.setBold(true);
        ui->cleanLbl->setFont(tempFont);
        tempFont = ui->filterLbl->font();
        tempFont.setBold(false);
        ui->filterLbl->setFont(tempFont);

        ui->filterLbl->setVisible(false);
        ui->cleanLbl->setVisible(true);
        ui->startLbl->setVisible(false);
    }
}

void DustendWindow::showInfo()
{
    if (ui->btnTimer->isChecked()
            || ui->btnFanControl->isChecked()
            || ui->btnSettings->isChecked()) {
        ui->btnTimer->setChecked(false);
        ui->btnFanControl->setChecked(false);
        ui->btnSettings->setChecked(false);

        ui->stackedWidget->setCurrentIndex(3);
    }
    ui->btnInfo->setChecked(true);
}

void DustendWindow::showNetwork()
{
//    ui->stackedWidget->setCurrentIndex(0);    
    ui->actionTabWdg->setVisible(false);
    ui->curCompSetsWdg->setVisible(false);
    findMyComputerAddresses();
    if (firstNet) {
        netW.start();
        firstNet = false;
        netTimer.start();
    }
    ui->networkTbl->repaint();
}

void DustendWindow::showComputer()
{
    ui->actionTabWdg->setVisible(true);
    ui->curCompSetsWdg->setVisible(true);
    if (ui->compTabWdg->currentIndex() == 1
            || localIp != tr("127.0.0.1")) {
//        if (!compL->getCleanTimeIsRunning(&localIp)
//                && !compL->getFilterTimeIsRunning(&localIp))
            // если предыдущий комп сетевой и время для него не запущено, то удаляем из списка
//            compL->delNetComp(&localIp);

        localIp = tr("127.0.0.1");
        showComputerStats(&localIp);
    }
}

void DustendWindow::createConnections()
{
    connect(ui->cbDustyPlace, SIGNAL(clicked()), this, SLOT(setDirtyPlace()));
    connect(ui->cbDustyPlace, SIGNAL(clicked()), this, SLOT(saveCurSettings()));
    connect(ui->btnFanControl, SIGNAL(clicked()), this, SLOT(showFanControl()));
    connect(ui->btnSettings, SIGNAL(clicked()), this, SLOT(showSettings()));
    connect(ui->btnTimer, SIGNAL(clicked()), this, SLOT(showTimer()));

    connect(ui->compTabWdg, SIGNAL(currentChanged(int)), this, SLOT(setCurCompTab(int)));
    connect(ui->actionTabWdg, SIGNAL(currentChanged(int)), this, SLOT(setCurActionTab(int)));
    connect(ui->btnInfo, SIGNAL(clicked()), this, SLOT(showInfo()));
    connect(ui->bt_start, SIGNAL(clicked()), this, SLOT(startCleaningTime()));
    connect(ui->bt_start, SIGNAL(clicked()), this, SLOT(saveCurSettings()));
    connect(ui->bt_stop, SIGNAL(clicked()), this, SLOT(execStopDialog()));
    connect(ui->bt_stop, SIGNAL(clicked()), this, SLOT(saveCurSettings()));
    connect(ui->bt_pause, SIGNAL(clicked()), this, SLOT(pauseCleaningTime()));
    connect(ui->bt_pause, SIGNAL(clicked()), this, SLOT(saveCurSettings()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerTick()));
    connect(&warningTimer, SIGNAL(timeout()), this, SLOT(warningTimerTick()));

    connect(ui->cbox_hot_left_num, SIGNAL(currentIndexChanged(int)), this, SLOT(setLeftCoolerAmount()));
    connect(ui->cbox_hot_left_num, SIGNAL(currentIndexChanged(int)), this, SLOT(saveCurSettings()));
    connect(ui->cbox_hot_up_num, SIGNAL(currentIndexChanged(int)), this, SLOT(setTopCoolerAmount()));
    connect(ui->cbox_hot_up_num, SIGNAL(currentIndexChanged(int)), this, SLOT(saveCurSettings()));
    connect(ui->cbox_left_num, SIGNAL(currentIndexChanged(int)), this, SLOT(setRightCoolerAmount()));
    connect(ui->cbox_left_num, SIGNAL(currentIndexChanged(int)), this, SLOT(saveCurSettings()));
    connect(ui->cbox_up_num, SIGNAL(currentIndexChanged(int)), this, SLOT(setBottomCoolerAmount()));
    connect(ui->cbox_up_num, SIGNAL(currentIndexChanged(int)), this, SLOT(saveCurSettings()));
    connect(ui->cbox_side_num, SIGNAL(currentIndexChanged(int)), this, SLOT(setSideCoolerAmount()));
    connect(ui->cbox_side_num, SIGNAL(currentIndexChanged(int)), this, SLOT(saveCurSettings()));

    connect(ui->cbox_hot_left_size, SIGNAL(currentIndexChanged(int)), this, SLOT(setLeftCoolerDiameter()));
    connect(ui->cbox_hot_left_size, SIGNAL(currentIndexChanged(int)), this, SLOT(saveCurSettings()));
    connect(ui->cbox_hot_up_size, SIGNAL(currentIndexChanged(int)), this, SLOT(setTopCoolerDiameter()));
    connect(ui->cbox_hot_up_size, SIGNAL(currentIndexChanged(int)), this, SLOT(saveCurSettings()));
    connect(ui->cbox_left_size, SIGNAL(currentIndexChanged(int)), this, SLOT(setRightCoolerDiameter()));
    connect(ui->cbox_left_size, SIGNAL(currentIndexChanged(int)), this, SLOT(saveCurSettings()));
    connect(ui->cbox_up_size, SIGNAL(currentIndexChanged(int)), this, SLOT(setBottomCoolerDiameter()));
    connect(ui->cbox_up_size, SIGNAL(currentIndexChanged(int)), this, SLOT(saveCurSettings()));
    connect(ui->cbox_side_size, SIGNAL(currentIndexChanged(int)), this, SLOT(setSideCoolerDiameter()));
    connect(ui->cbox_side_size, SIGNAL(currentIndexChanged(int)), this, SLOT(saveCurSettings()));

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(slotTrayActivated(QSystemTrayIcon::ActivationReason)));
    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(showNormal()));
    connect(showMessageAction, SIGNAL(triggered()), this, SLOT(slotShowMessageAction()));
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(pauseNetComps()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(saveSettings()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(quitAction, SIGNAL(triggered()), &netW, SLOT(terminate()));

    connect(ui->bt_engLang, SIGNAL(clicked()), this, SLOT(setEngLang()));
    connect(ui->bt_rusLang, SIGNAL(clicked()), this, SLOT(setRusLang()));
    connect(ui->bt_ukrLang, SIGNAL(clicked()), this, SLOT(setUkrLang()));

    connect(ui->networkTbl, SIGNAL(clicked(QModelIndex)), this, SLOT(showNetComp(QModelIndex)));
    connect(ui->networkTbl->horizontalHeader(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(sortNet(int,Qt::SortOrder)));

    qRegisterMetaType< QList<NetComp> >("NetCompList");
    qRegisterMetaType< NetComp >("NetC");
    connect(&netW, SIGNAL(scanFinished(NetCompList)), this, SLOT(getNetComps(NetCompList)));
    connect(&netW, SIGNAL(getNetComp(NetC)), this, SLOT(addNetComp(NetC)));
    connect(&netTimer, SIGNAL(timeout()), &netW, SLOT(start()));
    connect(&netW, SIGNAL(started()), this, SLOT(setNetworkLbl()));
    connect(ui->refreshNetBtn, SIGNAL(clicked()), this, SLOT(setFirstNetTrue()));
    connect(ui->refreshNetBtn, SIGNAL(clicked()), this, SLOT(showNetwork()));

    connect(ui->cb_showNetName, SIGNAL(clicked()), this, SLOT(saveNetSettings()));
    connect(ui->cb_showIp, SIGNAL(clicked()), this, SLOT(saveNetSettings()));
    connect(ui->cb_showClean, SIGNAL(clicked()), this, SLOT(saveNetSettings()));
    connect(ui->cb_showFilter, SIGNAL(clicked()), this, SLOT(saveNetSettings()));
    connect(ui->cb_showOs, SIGNAL(clicked()), this, SLOT(saveNetSettings()));
    connect(ui->cb_showTime, SIGNAL(clicked()), this, SLOT(saveNetSettings()));
    connect(ui->cb_showStatus, SIGNAL(clicked()), this, SLOT(saveNetSettings()));
    connect(ui->rb_dontShowWarning, SIGNAL(clicked()), this, SLOT(setDontShowWarningMsg()));
    connect(ui->rb_show10minsWarning, SIGNAL(clicked()), this, SLOT(set10MinsWarningMsg()));
    connect(ui->rb_show30minsWarning, SIGNAL(clicked()), this, SLOT(set30MinsWarningMsg()));
    connect(ui->rb_showAtStartWarning, SIGNAL(clicked()), this, SLOT(setStartupWarningMsg()));
    connect(ui->rb_show60minsWarning, SIGNAL(clicked()), this, SLOT(set60MinsWarningMsg()));
    connect(ui->cb_sendEmail, SIGNAL(clicked()), this, SLOT(setSendEmail()));
    connect(ui->emailEdit, SIGNAL(textChanged(QString)), this, SLOT(setEmail(QString)));
    connect(ui->rb_autorunProgr, SIGNAL(clicked()), this, SLOT(setAutorunProgam()));
    connect(compL->getSaveThr(), SIGNAL(settingsSaved()), this, SLOT(settingsSaved()));
    connect(ui->bt_sendTest, SIGNAL(clicked()), this, SLOT(sendTestEmail()));
}

void DustendWindow::startCleaningTime()
{
    bool run = true;
    bool pause = false;
    bool withAverageUsageTime = false;
    ui->bt_stop->setVisible(true);
    ui->bt_pause->setVisible(true);
    ui->bt_start->setVisible(false);
    if (!compL->CompIsPaused(&localIp)) {
        QDateTime curDateTime = QDateTime::fromString(QDateTime::currentDateTimeUtc().toString("dd.MM.yyyy hh:mm:ss"),
                                                      "dd.MM.yyyy hh:mm:ss");
        compL->setDateOfFirstStart(&curDateTime, &localIp);
    }
    ui->pbStart->repaint();
    if (ui->actionTabWdg->currentIndex() == 0) {
        setCleaningTimeLbl();
        setProgressBar();        
        if (!compL->CompIsPaused(&localIp)) {            
            compL->setCleanTimeIsRunning(&localIp, &run);
            ui->pbStart->setMaximum(1300);
            ui->pbStart->setFormat(compL->getShortCleanTime(&localIp, &withAverageUsageTime));
        }
        else
            compL->setCompPause(&localIp, &pause);        
    }
    if (ui->actionTabWdg->currentIndex() == 1) {
        setFilterTimeLbl();
        setProgressBar();        
        if (!compL->CompIsPaused(&localIp)) {
            compL->setFilterTimeIsRunning(&localIp, &run);            
            ui->pbStart->setMaximum(2600);
            ui->pbStart->setFormat(compL->getShortFilterTime(&localIp, &withAverageUsageTime));
        }
        else
            compL->setCompPause(&localIp, &pause);
    }
    if (!compL->CompIsPaused(&localIp)) {
        QString pbStartFormat = ui->pbStart->format();
        compL->setLastMaxTime(&localIp, &pbStartFormat);
        ui->pbStart->setValue(compL->getLastMaxTimeHours(&localIp).toInt());
    }
    ui->networkTbl->repaint();
    ui->pbStart->repaint();
}

void DustendWindow::stopCleaningTime()
{
    bool run = false;    
    ui->bt_stop->setVisible(false);
    ui->bt_pause->setVisible(false);
    ui->bt_start->setVisible(true);
    if (ui->actionTabWdg->currentIndex() == 0) {
        compL->setMaxAndCurCleanTime(&localIp);
        compL->correctCleanTime(&localIp);
        setCleaningTimeLbl();
        compL->setCompPause(&localIp, &run);
        compL->setCleanTimeIsRunning(&localIp, &run);
    }
    if (ui->actionTabWdg->currentIndex() == 1) {
        compL->setMaxAndCurFilterTime(&localIp);
        compL->correctFilterTime(&localIp);
        setFilterTimeLbl();
        compL->setCompPause(&localIp, &run);
        compL->setFilterTimeIsRunning(&localIp, &run);
    }
    setProgressBar();
    ui->networkTbl->repaint();
}

void DustendWindow::pauseCleaningTime()
{
    bool run = true;
    ui->bt_stop->setVisible(true);
    ui->bt_pause->setVisible(false);
    ui->bt_start->setVisible(true);
    if (ui->actionTabWdg->currentIndex() == 0) {
        compL->setCompPause(&localIp, &run);
        setCleaningTimeLbl();
    }
    if (ui->actionTabWdg->currentIndex() == 1) {
        compL->setCompPause(&localIp, &run);
        setFilterTimeLbl();
    }
    setProgressBar();
    ui->networkTbl->repaint();
}

void DustendWindow::timerTick()
{
    if (setsSaved) {
        if (saveTimer == 10) {
            setsSaved = false;
            compL->saveSettingsNew();
            saveTimer = 0;
        }
    }
    saveTimer++;
    compL->timerTick();
    setCleaningTimeLbl();
    setFilterTimeLbl();
    setTrayIconImage();
    setTrayIconHint();
    setNetworkTime();
    setProgressBar();

    asmInsertion();
    QString msg = "";
    QString subj = "";

    if (firstStart && (warningTime == 666666))
        if (compL->getWarningMessage() != "") {
            trayIcon->showMessage(warningMsg, compL->getWarningMessage(), QSystemTrayIcon::Warning, 10000 );
            firstStart = false;                        
            if (genSetLogic->getLocale() == "en") {
                msg = QString("Hi.\n%1. \nHave a nice day!").arg(compL->getWarningMessage());
                subj = "Warning email from ComputerDustControl";
            }
            if (genSetLogic->getLocale() == "ru") {
                msg = QString("Здравствуйте‚. \n%1. \nПриятного дня!").arg(compL->getWarningMessage());
                subj = "Предупреждающее письмо от ComputerDustControl";
            }
            if (sendEmail && emailAddress != "") {
                mailThr->setEmailAddress(&emailAddress);
                mailThr->setWarningMsg(&msg);
                mailThr->setSubject(&subj);
                mailThr->start();
            }
        }
    ui->networkTbl->repaint();
    ui->pbTime->repaint();
}

void DustendWindow::warningTimerTick()
{
    qDebug() << "tick";
    if (compL->getWarningMessage() != "")
        trayIcon->showMessage (warningMsg, compL->getWarningMessage(), QSystemTrayIcon::Warning, 10000 );
}

void DustendWindow::setCleaningTimeLbl()
{
//    ui->homeTimeLbl->setText(compL->getCleanTimeText(&localIp));
}

void DustendWindow::setArrows()
{
}

void DustendWindow::setCoolerCountComboBoxes()
{
    unsigned short newAmount;
    if (compL->getOutLeftCoolerAmount(&localIp) < ui->cbox_hot_left_num->count())
        ui->cbox_hot_left_num->setCurrentIndex(compL->getOutLeftCoolerAmount(&localIp));
    else {
        newAmount = ui->cbox_hot_left_num->count() - 1;
        ui->cbox_hot_left_num->setCurrentIndex(newAmount);
        compL->setOutLeftCoolerAmount(&localIp, &newAmount);
    }

    if (compL->getOutTopCoolerAmount(&localIp) < ui->cbox_hot_up_num->count())
        ui->cbox_hot_up_num->setCurrentIndex(compL->getOutTopCoolerAmount(&localIp));
    else {
        newAmount = ui->cbox_hot_up_num->count() - 1;
        ui->cbox_hot_up_num->setCurrentIndex(newAmount);
        compL->setOutTopCoolerAmount(&localIp, &newAmount);
    }

    if (compL->getInRightCoolerAmount(&localIp) < ui->cbox_left_num->count())
        ui->cbox_left_num->setCurrentIndex(compL->getInRightCoolerAmount(&localIp));
    else {
        newAmount = ui->cbox_left_num->count() - 1;
        ui->cbox_left_num->setCurrentIndex(newAmount);
        compL->setInRightCoolerAmount(&localIp, &newAmount);
    }

    if (compL->getInSideCoolerAmount(&localIp) < ui->cbox_side_num->count())
        ui->cbox_side_num->setCurrentIndex(compL->getInSideCoolerAmount(&localIp));
    else {
        newAmount = ui->cbox_side_num->count() - 1;
        ui->cbox_side_num->setCurrentIndex(newAmount);
        compL->setInSideCoolerAmount(&localIp, &newAmount);
    }

    if (compL->getInBottomCoolerAmount(&localIp) < ui->cbox_up_num->count())
        ui->cbox_up_num->setCurrentIndex(compL->getInBottomCoolerAmount(&localIp));
    else {
        newAmount = ui->cbox_up_num->count() - 1;
        ui->cbox_up_num->setCurrentIndex(newAmount);
        compL->setInBottomCoolerAmount(&localIp, &newAmount);
    }
}

void DustendWindow::setPlace()
{
    if (compL->getPlaceType(&localIp) == 1)
        ui->cbDustyPlace->setChecked(false);

    if (compL->getPlaceType(&localIp) == 3) {
        ui->cbDustyPlace->setChecked(true);
    }
}

void DustendWindow::setBtStart()
{
    if (ui->actionTabWdg->currentIndex() == 0) {
        if (compL->getCleanTimeIsRunning(&localIp)) {
            if (compL->CompIsPaused(&localIp)) {
                ui->bt_stop->setVisible(true);
                ui->bt_start->setVisible(true);
                ui->bt_pause->setVisible(false);
            }
            else {
                ui->bt_stop->setVisible(true);
                ui->bt_start->setVisible(false);
                ui->bt_pause->setVisible(true);
            }
        }
        else {
            ui->bt_stop->setVisible(false);
            ui->bt_start->setVisible(true);
            ui->bt_pause->setVisible(false);
        }
    }
    if (ui->actionTabWdg->currentIndex() == 1) {
        if (compL->getFilterTimeIsRunning(&localIp)) {
            if (compL->CompIsPaused(&localIp)) {
                ui->bt_stop->setVisible(true);
                ui->bt_start->setVisible(true);
                ui->bt_pause->setVisible(false);
            }
            else {
                ui->bt_stop->setVisible(true);
                ui->bt_start->setVisible(false);
                ui->bt_pause->setVisible(true);
            }
        }
        else {
            ui->bt_stop->setVisible(false);
            ui->bt_start->setVisible(true);
            ui->bt_pause->setVisible(false);
        }
    }
}

void DustendWindow::setGrayArrows()
{
    if (compL->getOutLeftCoolerAmount(&localIp) > 0)
        ui->arr_hot_left_2->setVisible(true);
    else
        ui->arr_hot_left_2->setVisible(false);
    if (compL->getOutTopCoolerAmount(&localIp) > 0)
        ui->arr_hot_up_2->setVisible(true);
    else
        ui->arr_hot_up_2->setVisible(false);
    if (compL->getInRightCoolerAmount(&localIp) > 0)
        ui->arr_left_2->setVisible(true);
    else
        ui->arr_left_2->setVisible(false);
    if (compL->getInBottomCoolerAmount(&localIp) > 0)
        ui->arr_up_2->setVisible(true);
    else
        ui->arr_up_2->setVisible(false);
    if (compL->getInSideCoolerAmount(&localIp) > 0)
        ui->arr_side_2->setVisible(true);
    else
        ui->arr_side_2->setVisible(false);
}

void DustendWindow::setLeftCoolerAmount()
{
    unsigned short newAmount, coolerType;
    coolerType = 1;
    newAmount = ui->cbox_hot_left_num->currentIndex();

    compL->addCooler(&localIp, &coolerType, &newAmount);
    setCleaningTimeLbl();
    setFilterTimeLbl();
    setArrows();
    setGrayArrows();
}

void DustendWindow::setTopCoolerAmount()
{
    unsigned short newAmount, coolerType;
    coolerType = 2;
    newAmount = ui->cbox_hot_up_num->currentIndex();
    compL->addCooler(&localIp, &coolerType, &newAmount);
    setCleaningTimeLbl();
    setFilterTimeLbl();
    setArrows();
    setGrayArrows();    

    if (ui->cbox_hot_up_num->currentIndex() == 3) {
        if (ui->cbox_hot_up_size->count() == 5)
            ui->cbox_hot_up_size->removeItem(4);
        else
            ui->cbox_hot_up_size->addItem(tr("200 mm"));
    }
    else {
        if (ui->cbox_hot_up_num->currentIndex() == 4) {
            ui->cbox_hot_up_size->removeItem(4);
            ui->cbox_hot_up_size->removeItem(3);
        }
        else {
            if (ui->cbox_hot_up_size->count() == 4)
                ui->cbox_hot_up_size->addItem(tr("230 mm"));
            if (ui->cbox_hot_up_size->count() == 3) {
                ui->cbox_hot_up_size->addItem(tr("200 mm"));
                ui->cbox_hot_up_size->addItem(tr("230 mm"));
            }
        }
    }
}

void DustendWindow::setRightCoolerAmount()
{
    unsigned short newAmount, coolerType;
    coolerType = 3;
    newAmount = ui->cbox_left_num->currentIndex();
    compL->addCooler(&localIp, &coolerType, &newAmount);
    setCleaningTimeLbl();
    setFilterTimeLbl();
    setArrows();
    setGrayArrows();

    if (ui->cbox_left_num->currentIndex() == 3) {
        if (ui->cbox_left_size->count() == 5)
            ui->cbox_left_size->removeItem(4);
        else
            ui->cbox_left_size->addItem(tr("200 mm"));
    }
    else {
        if (ui->cbox_left_num->currentIndex() == 4) {
            ui->cbox_left_size->removeItem(4);
            ui->cbox_left_size->removeItem(3);
        }
        else {
            if (ui->cbox_left_size->count() == 4)
                ui->cbox_left_size->addItem(tr("230 mm"));
            if (ui->cbox_left_size->count() == 3) {
                ui->cbox_left_size->addItem(tr("200 mm"));
                ui->cbox_left_size->addItem(tr("230 mm"));
            }
        }
    }
}

void DustendWindow::setBottomCoolerAmount()
{
    unsigned short newAmount, coolerType;
    coolerType = 4;
    newAmount = ui->cbox_up_num->currentIndex();
    compL->addCooler(&localIp, &coolerType, &newAmount);
    setCleaningTimeLbl();
    setFilterTimeLbl();
    setArrows();
    setGrayArrows();
}

void DustendWindow::setSideCoolerAmount()
{
    unsigned short newAmount, coolerType;
    coolerType = 5;
    newAmount = ui->cbox_side_num->currentIndex();
    compL->addCooler(&localIp, &coolerType, &newAmount);
    setCleaningTimeLbl();
    setFilterTimeLbl();
    setArrows();
    setGrayArrows();

    if (ui->cbox_side_num->currentIndex() == 3) {
        if (ui->cbox_side_size->count() == 5)
            ui->cbox_side_size->removeItem(4);
        else
            ui->cbox_side_size->addItem(tr("200 mm"));
    }
    else {
        if (ui->cbox_side_num->currentIndex() == 4) {
            ui->cbox_side_size->removeItem(4);
            ui->cbox_side_size->removeItem(3);
        }
        else {
            if (ui->cbox_side_size->count() == 4)
                ui->cbox_side_size->addItem(tr("230 mm"));
            if (ui->cbox_side_size->count() == 3) {
                ui->cbox_side_size->addItem(tr("200 mm"));
                ui->cbox_side_size->addItem(tr("230 mm"));
            }
        }
    }
}

void DustendWindow::setLeftCoolerDiameter()
{
    unsigned short newDiameter;
    newDiameter = getCoolerDiameter(ui->cbox_hot_left_size->currentText());
    unsigned short coolerType = 1;
    compL->changeCoolerDiameter(&localIp, &coolerType, &newDiameter);
    setCleaningTimeLbl();
    setFilterTimeLbl();
}

void DustendWindow::setTopCoolerDiameter()
{
    unsigned short newDiameter;
    newDiameter = getCoolerDiameter(ui->cbox_hot_up_size->currentText());
    unsigned short coolerType = 2;
    compL->changeCoolerDiameter(&localIp, &coolerType, &newDiameter);
    setCleaningTimeLbl();
    setFilterTimeLbl();
}

void DustendWindow::setRightCoolerDiameter()
{
    unsigned short newDiameter;
    newDiameter = getCoolerDiameter(ui->cbox_left_size->currentText());
    unsigned short coolerType = 3;
    compL->changeCoolerDiameter(&localIp, &coolerType, &newDiameter);
    setCleaningTimeLbl();
    setFilterTimeLbl();
}

void DustendWindow::setBottomCoolerDiameter()
{
    unsigned short newDiameter;
    newDiameter = getCoolerDiameter(ui->cbox_up_size->currentText());
    unsigned short coolerType = 4;
    compL->changeCoolerDiameter(&localIp, &coolerType, &newDiameter);
    setCleaningTimeLbl();
    setFilterTimeLbl();
}

void DustendWindow::setSideCoolerDiameter()
{
    unsigned short newDiameter;
    newDiameter = getCoolerDiameter(ui->cbox_side_size->currentText());
    unsigned short coolerType = 5;
    compL->changeCoolerDiameter(&localIp, &coolerType, &newDiameter);
    setCleaningTimeLbl();
    setFilterTimeLbl();
}

void DustendWindow::setMaxAndCurTimes()
{
}

void DustendWindow::setFilterTimeLbl()
{
//    ui->filterTimeLbl->setText(compL->getFilterTimeText(&localIp));
}

void DustendWindow::setCoolerDiameterComboBoxes()
{
    if (compL->getOutLeftCoolerDiameter(&localIp) == 80)
        ui->cbox_hot_left_size->setCurrentIndex(0);
    if (compL->getOutLeftCoolerDiameter(&localIp) == 120)
        ui->cbox_hot_left_size->setCurrentIndex(1);
    if (compL->getOutLeftCoolerDiameter(&localIp) == 140)
        ui->cbox_hot_left_size->setCurrentIndex(2);

    if (compL->getOutTopCoolerDiameter(&localIp) == 80)
        ui->cbox_hot_up_size->setCurrentIndex(0);
    if (compL->getOutTopCoolerDiameter(&localIp) == 120)
        ui->cbox_hot_up_size->setCurrentIndex(1);
    if (compL->getOutTopCoolerDiameter(&localIp) == 140)
        ui->cbox_hot_up_size->setCurrentIndex(2);
    if (compL->getOutTopCoolerDiameter(&localIp) == 200)
        ui->cbox_hot_up_size->setCurrentIndex(3);
    if (compL->getOutTopCoolerDiameter(&localIp) == 230)
        ui->cbox_hot_up_size->setCurrentIndex(4);

    if (compL->getInRightCoolerDiameter(&localIp) == 80)
        ui->cbox_left_size->setCurrentIndex(0);
    if (compL->getInRightCoolerDiameter(&localIp) == 120)
        ui->cbox_left_size->setCurrentIndex(1);
    if (compL->getInRightCoolerDiameter(&localIp) == 140)
        ui->cbox_left_size->setCurrentIndex(2);
    if (compL->getInRightCoolerDiameter(&localIp) == 200)
        ui->cbox_left_size->setCurrentIndex(3);
    if (compL->getInRightCoolerDiameter(&localIp) == 230)
        ui->cbox_left_size->setCurrentIndex(4);

    if (compL->getInBottomCoolerDiameter(&localIp) == 80)
        ui->cbox_up_size->setCurrentIndex(0);
    if (compL->getInBottomCoolerDiameter(&localIp) == 120)
        ui->cbox_up_size->setCurrentIndex(1);
    if (compL->getInBottomCoolerDiameter(&localIp) == 140)
        ui->cbox_up_size->setCurrentIndex(2);

    if (compL->getInSideCoolerDiameter(&localIp) == 80)
        ui->cbox_side_size->setCurrentIndex(0);
    if (compL->getInSideCoolerDiameter(&localIp) == 120)
        ui->cbox_side_size->setCurrentIndex(1);
    if (compL->getInSideCoolerDiameter(&localIp) == 140)
        ui->cbox_side_size->setCurrentIndex(2);
    if (compL->getInSideCoolerDiameter(&localIp) == 200)
        ui->cbox_side_size->setCurrentIndex(3);
    if (compL->getInSideCoolerDiameter(&localIp) == 230)
        ui->cbox_side_size->setCurrentIndex(4);
}

void DustendWindow::setCoolerDiameter()
{
    if (compShown) {
        compL->correctCleanTime(&localIp);
        compL->correctFilterTime(&localIp);
    }
    setCleaningTimeLbl();
    setFilterTimeLbl();
}

void DustendWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible())
    {
        hide();
        event->ignore();
    }
    else
        netW.wait();
}

void DustendWindow::showComputerStats(QString *ipAddress)
{    
    compShown = false;
    localIp = *ipAddress;
    setPlace();    
    if (!this->signalsBlocked())
        this->blockSignals(true);
    if (compL->getCleanTimeIsRunning(&localIp)) {
        ui->actionTabWdg->setCurrentIndex(0);
        ui->cleanLbl->setVisible(true);
        ui->startLbl->setVisible(false);
        showCleaningStatus();
        setProgressBar();
    }
    else {
        if (compL->getFilterTimeIsRunning(&localIp)) {
            ui->actionTabWdg->setCurrentIndex(1);
            showFilterStatus();
            setProgressBar();
        }
        else {
            ui->actionTabWdg->setCurrentIndex(0);
            ui->filterLbl->setVisible(false);
            ui->cleanLbl->setVisible(true);
            ui->startLbl->setVisible(false);
            setProgressBar();
        }
    }
    this->blockSignals(false);
    setBtStart();

    setArrows();
    setGrayArrows();    
    setCoolerCountComboBoxes();    
    setCoolerDiameterComboBoxes();
    if (localIp == "127.0.0.1")
        setCompNameLbl();
    showComputerComment();
    setEventFilterToFrames();    
    compShown = true;
    setComboBoxes();
}

void DustendWindow::setNetworkTime()
{
    QString curIp, curClean, curFilter;
    NetComp curCmp;
    QString selectedIp = "";
    bool withAverageUsageTime = true;
    QModelIndexList selection = ui->networkTbl->selectionModel()->selectedIndexes();
    if (!selection.empty()) {
        QModelIndex index = selection.at(1);
        selectedIp = table->data(index, Qt::DisplayRole).toString();
    }
    for (int i = 0; i < networkComps.size(); ++i) {
        curIp = networkComps.at(i).getIpAddress();
        curClean = compL->getShortCleanTime(&curIp, &withAverageUsageTime);
        curFilter = compL->getShortFilterTime(&curIp, &withAverageUsageTime);
        curCmp = networkComps.at(i);
        QString nulStr = "";        
        if ((curIp != selectedIp)
                || (curIp == selectedIp
                    && (compL->getCleanTimeIsRunning(&curIp)
                        || compL->getFilterTimeIsRunning(&curIp))) ) {
            if (compL->getCleanTimeIsRunning(&curIp))
                curCmp.setCleanTime(&curClean);
            else
                curCmp.setCleanTime(&nulStr);
            if (compL->getFilterTimeIsRunning(&curIp))
                curCmp.setFilterTime(&curFilter);
            else
                curCmp.setFilterTime(&nulStr);
        }
        else {
            if (ui->actionTabWdg->currentIndex() == 0) {
                curCmp.setCleanTime(&curClean);
                curCmp.setFilterTime(&nulStr);
            }
            else {
                curCmp.setCleanTime(&nulStr);
                curCmp.setFilterTime(&curFilter);
            }
        }

        networkComps.replace(i, curCmp);
        setTableTime(&curIp, &i);
    }
}

void DustendWindow::setTableTime(QString *ipAddress, int *netCompIpPos)
{
    QModelIndex index;
    for (int i = 0; i < networkComps.size(); ++i) {
        index = table->index(i, 1, QModelIndex());
        if (table->data(index, Qt::DisplayRole).toString() == *ipAddress) {
            index = table->index(i, 2, QModelIndex());
            table->setData(index, networkComps.at(*netCompIpPos).getCleanTime(), Qt::EditRole);
            index = table->index(i, 3, QModelIndex());
            table->setData(index, networkComps.at(*netCompIpPos).getFilterTime(), Qt::EditRole);
        }
    }
}

void DustendWindow::initStopDialog()
{
    stopDialog.setWindowTitle(warningMsg + "!");
    stopDialog.setText(stopDialogMsg);
    QAbstractButton *okButton =
            stopDialog.addButton(okBtn, QMessageBox::ActionRole);
            stopDialog.addButton(cancelBtn, QMessageBox::ActionRole);
    connect(okButton, SIGNAL(clicked()), this, SLOT(stopCleaningTime()));
}

void DustendWindow::setCompNameLbl()
{    
    if (localIp != "127.0.0.1")
        ui->osNameLbl->setText(QString(QObject::tr("%1").arg(compL->getOsName(&localIp))));
    else
        ui->osNameLbl->setText(QString::fromLocal8Bit(osName().c_str()));
    compL->setOsName(&localIp, &QString("%1").arg(ui->osNameLbl->text()));
}

void DustendWindow::registerAutorun()
{
#ifdef Q_OS_WIN
    QSettings *autorun = new QSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    autorun->setValue("DUSTENDTimer", QDir::toNativeSeparators(QCoreApplication::applicationFilePath()) + " --minimized");
    autorun->sync();
#endif
}

void DustendWindow::unRegisterAutorun()
{
#ifdef Q_OS_WIN
    QSettings *autorun = new QSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    autorun->remove("DUSTENDTimer");
    autorun->sync();
#endif
}

void DustendWindow::slotTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        emit(this->showNormal());
    }
}

void DustendWindow::slotShowMessageAction()
{
    //trayIcon->showMessage ( const QString & title, const QString & message, MessageIcon icon = Information, int millisecondsTimeoutHint = 10000 );

    //   QSystemTrayIcon::NoIcon       0   No icon is shown.
    //   QSystemTrayIcon::Information  1   An information icon is shown.
    //   QSystemTrayIcon::Warning      2   A standard warning icon is shown.
    //   QSystemTrayIcon::Critical     3   A critical warning icon is shown.

    //Note that display of messages are dependent on the system configuration and user preferences, and
    //that messages may not appear at all. Hence, it should not be relied upon as the sole means for
    //providing critical information.
    //
    //On Windows, the millisecondsTimeoutHint is usually ignored by the system when the application has focus.
    //
    //On Mac OS X, the Growl notification system must be installed for this function to display messages.


    if(QSystemTrayIcon::supportsMessages())
    {
        trayIcon->showMessage ( warningMsg, msg + "\n", QSystemTrayIcon::Warning, 1 );
    }
}

void DustendWindow::setTrayIcon()
{
    showMessageAction = new QAction(showMessage, this);
    minimizeAction = new QAction(minimize, this);
    maximizeAction = new QAction(maximize, this);
    restoreAction = new QAction(restore, this);
    quitAction = new QAction(quit, this);

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(showMessageAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    trayIcon->setIcon(QPixmap(":/images/Dustend19.ico"));
    trayIcon->show();
}

void DustendWindow::setTrayIconImage()
{
    trayIcon->setIcon(QPixmap(":/images/Dustend19.ico"));
//    if (compL->getCleanTimeIsRunning(&localIp) || compL->getFilterTimeIsRunning(&localIp))
//        trayIcon->setIcon(QPixmap(":/images/active.png"));
//    else
//        trayIcon->setIcon(QPixmap(":/images/inactive.png"));
}

void DustendWindow::saveSettings()
{    
    compL->saveComputers();
}

void DustendWindow::saveCurSettings()
{
    if (setsSaved) {
        setsSaved = false;
        compL->saveSettingsNew();
    }
}

void DustendWindow::setTrayIconHint()
{
    QString hint = "";
    bool withAverageUsageTime = false;
    if (compL->getCleanTimeIsRunning(&localIp))
        hint = trayClean + ": " + compL->getShortCleanTime(&localIp, &withAverageUsageTime);
    if (compL->getFilterTimeIsRunning(&localIp))
        hint = trayFilter + ": " + compL->getShortFilterTime(&localIp, &withAverageUsageTime);
    trayIcon->setToolTip(hint);
}

void DustendWindow::findMyComputerAddresses()
{
    QString curIpAddress;
    ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < networkComps.size(); ++i) {        
        curIpAddress = networkComps.at(i).getIpAddress();
        if (isAddressBelongsToMe(&curIpAddress))
            networkComps.removeAt(i);       
    }
}

void DustendWindow::fillTableModel()
{
    int rowCount = table->rowCount(QModelIndex());
    table->clear();
    if (rowCount > 0)
        ui->networkTbl->model()->removeRows(0, rowCount);
    for (int i = 0; i < networkComps.size(); ++i) {
        insertRowToTable(networkComps.at(i), i);
    }
    ui->networkTbl->resizeRowsToContents();
    ui->networkTbl->resizeColumnToContents(5);
    ui->networkTbl->resizeColumnToContents(1);
    ui->networkTbl->resizeColumnToContents(3);
}

void DustendWindow::showNetComp(const QModelIndex &index)
{    
    ui->actionTabWdg->setVisible(true);
    ui->curCompSetsWdg->setVisible(true);

    int row = index.row();
    QModelIndex index1 = table->index(row, 1, QModelIndex());
    curIp = table->data(index1, Qt::DisplayRole).toString();

//    if (!compL->getCleanTimeIsRunning(&localIp)
//            && !compL->getFilterTimeIsRunning(&localIp))
        // если предыдущий комп сетевой и время для него не запущено, то удаляем из списка
//        compL->delNetComp(&localIp);

    // ищем комп в списке из compL, если нету, то добавляем его с параметрами по умолчанию
    QString name, osName, lastMaxTime;
    unsigned short placeType, outLeftCoolerAmount, outLeftCoolerDiameter,
    outTopCoolerAmount, outTopCoolerDiameter, inRightCoolerAmount,
    inRightCoolerDiameter, inBottomCoolerAmount, inBottomCoolerDiameter,
    inSideCoolerAmount, inSideCoolerDiameter, maxTemperature;
    int maxCleanTime, maxFilterTime;
    unsigned int maxFanSpeed;
    unsigned long usageTime;
    bool cleanTimeIsRunning, filterTimeIsRunning, compIsPaused;
    QDateTime cleanTime, filterTime, dateOfFirstStart;

    index1 = table->index(row, 0, QModelIndex());
    name = table->data(index1, Qt::DisplayRole).toString();
    index1 = table->index(row, 4, QModelIndex());
    osName = table->data(index1, Qt::DisplayRole).toString();

    placeType = 1;
    outLeftCoolerAmount = 0;
    outTopCoolerAmount = 0;
    inRightCoolerAmount = 0;
    inBottomCoolerAmount = 0;
    inSideCoolerAmount = 0;
    outLeftCoolerDiameter = 120;
    outTopCoolerDiameter = 120;
    inRightCoolerDiameter = 120;
    inBottomCoolerDiameter = 120;
    inSideCoolerDiameter = 120;
    maxCleanTime = 4680000;
    lastMaxTime = "1300";
    maxFilterTime = 9360000;
    maxFanSpeed = 2000;
    usageTime = 0;
    maxTemperature = 70;
    cleanTimeIsRunning = false;
    filterTimeIsRunning = false;
    cleanTime = QDateTime::currentDateTime().addSecs(4680000);
    filterTime = QDateTime::currentDateTime().addSecs(9360000);
    compIsPaused = false;
    dateOfFirstStart = QDateTime::fromString("24.10.1795 12:12:12",
                                             "dd.MM.yyyy hh:mm:ss");

    compL->addComputer(compL->getAllComps(),&curIp, &name, &placeType, &cleanTime, &filterTime,
                       &cleanTimeIsRunning, &filterTimeIsRunning, &maxCleanTime,
                       &maxFilterTime, &outLeftCoolerAmount, &outLeftCoolerDiameter,
                       &outTopCoolerAmount, &outTopCoolerDiameter, &inRightCoolerAmount,
                       &inRightCoolerDiameter, &inBottomCoolerAmount, &inBottomCoolerDiameter,
                       &inSideCoolerAmount, &inSideCoolerDiameter, &maxTemperature,
                       &usageTime, &maxFanSpeed, &osName, &compIsPaused, &dateOfFirstStart,
                       &lastMaxTime);

    localIp = curIp;    
    showComputerStats(&localIp);   
    ui->stackedWidget->setCurrentIndex(0);
    ui->networkTbl->repaint(); 
}

void DustendWindow::setupModel()
{    
//    table = new QStandardItemModel(0, 5, this);
//    table->setHeaderData(0, Qt::Horizontal, tr("Name"));
//    table->setHeaderData(1, Qt::Horizontal, tr("ip-address"));
//    table->setHeaderData(2, Qt::Horizontal, tr("clean time"));
//    table->setHeaderData(3, Qt::Horizontal, tr("filter time"));
//    table->setHeaderData(4, Qt::Horizontal, tr("OS"));
    ui->networkTbl->setItemDelegateForColumn(5, new ProgressDelegate);
}

void DustendWindow::getNetComps(QList<NetComp> allNetComps)
{
    // ищем компы в сети, и как только находим комп, сразу же его передаем в
    // dustendwindow.cpp и пытаемся его добавить в table и networkComps
    qDebug() << "scanned";
    if (networkComps.size() == 0) {
        networkComps = allNetComps;
        findMyComputerAddresses();
        unPauseNetComps();
        netLogic.sortByTime(networkComps, false);
        fillTableModel();
    }
    else {
        QString curIp;
        bool pause = false;
        // ищем в allNetComps компы из netComps, если находим, то удаляем комп из allNetComps
        // если не находим, то удаляем комп из netComps и table, и ставим его на паузу
        for (int i = 0; i < networkComps.size(); ++i) {
            curIp = networkComps.at(i).getIpAddress();
            if (isCompInAllNetComps(&curIp, allNetComps))
                removeFromAllNetComps(&allNetComps, &curIp);
            else {
                table->removeRow(findCompInTable(&curIp));
                networkComps.removeAt(i);
                compL->setCompPause(&curIp, &pause);
            }
        }
        // ищем в table компы из allNetComps. Если не находим, то добавляем в netComps и
        // в table, снимаем с паузы.
        for (int i = 0; i < allNetComps.size(); ++i) {
            curIp = allNetComps.at(i).getIpAddress();
            if (findCompInTable(&curIp) != -1
                    && !isAddressBelongsToMe(&curIp)) {
                networkComps.push_back(allNetComps.at(i));
                insertRowToTable(allNetComps.at(i), table->rowCount(QModelIndex()));
                compL->setCompPause(&curIp, &pause);
            }
        }
    }
    ui->netStatusLbl->setText(QString("%1 %2 %3").arg(wasFinded).arg(networkComps.size()).arg(computer));
//    ui->networkTbl->setCurrentIndex(table->index(0,5, QModelIndex()));
//    showNetComp(table->index(0,5, QModelIndex()));
    ui->networkTbl->repaint();
}

void DustendWindow::addNetComp(NetC newNetComp)
{
    bool pause = false;
    curIp = newNetComp.getIpAddress();    
    if (findCompInTable(&curIp) == -1
            && !isAddressBelongsToMe(&curIp)) {        
        networkComps.push_back(newNetComp);
        insertRowToTable(newNetComp, table->rowCount(QModelIndex()));
        compL->setCompPause(&curIp, &pause);
        ui->networkTbl->resizeRowsToContents();
        ui->networkTbl->resizeColumnToContents(5);
        ui->networkTbl->resizeColumnToContents(1);
        ui->networkTbl->resizeColumnToContents(3);
    }
    netW.wake();
}

void DustendWindow::pauseNetComps()
{
    QString curIp;
    bool pause = true;
    for (int i = 0; i < networkComps.size(); ++i) {
        curIp = networkComps.at(i).getIpAddress();
        compL->setCompPause(&curIp, &pause);
    }
}

void DustendWindow::unPauseNetComps()
{
    QString curIp;
    bool pause = false;
    for (int i = 0; i < networkComps.size(); ++i) {
        curIp = networkComps.at(i).getIpAddress();
        compL->setCompPause(&curIp, &pause);
    }
}

void DustendWindow::setFirstNetTrue()
{
    firstNet = true;
}

bool DustendWindow::isCompInAllNetComps(QString *ipAddress, QList<NetComp> allNetComps)
{
    bool compHere = false;
    for (int i = 0; i < allNetComps.size(); ++i) {
        if (allNetComps.at(i).getIpAddress() == *ipAddress) {
            compHere = true;
            break;
        }
    }
    return compHere;
}

int DustendWindow::findCompInTable(QString *ipAddress)
{
    QModelIndex index;
    bool finded = false;
    for (int i = 0; i < table->rowCount(QModelIndex()); ++i) {
        index = table->index(i, 1, QModelIndex());
        if (table->data(index, Qt::DisplayRole).toString() == *ipAddress) {
            finded = true;
            break;
        }
    }
    return finded ? index.row() : -1;
}

void DustendWindow::insertRowToTable(NetComp newComp, int row)
{
    table->insertRows(row, 1, QModelIndex());
    QModelIndex index = table->index(row, 0, QModelIndex());
    QString newVal = newComp.getName();
    table->setData(index, newVal, Qt::EditRole);
    index = table->index(row, 1, QModelIndex());
    newVal = newComp.getIpAddress();
    table->setData(index, newVal, Qt::EditRole);
    index = table->index(row, 2, QModelIndex());
    newVal = newComp.getCleanTime();
    table->setData(index, newVal, Qt::EditRole);
    index = table->index(row, 3, QModelIndex());
    newVal = newComp.getFilterTime();
    table->setData(index, newVal, Qt::EditRole);
    index = table->index(row, 4, QModelIndex());
    newVal = newComp.getOsName();
    table->setData(index, newVal, Qt::EditRole);    
}

void DustendWindow::removeFromAllNetComps(QList<NetComp> *allNetComps, QString *ipAddress)
{
    for(int i = 0; i < allNetComps->size(); ++i) {
        if (allNetComps->at(i).getIpAddress() == *ipAddress)
            allNetComps->removeAt(i);
    }
}

void DustendWindow::setNetworkLbl()
{
    if (firstNet)
        ui->netStatusLbl->setText(scanning);
    else
        ui->netStatusLbl->setText(updating);
}

void DustendWindow::addCooler(QComboBox *combo)
{
    if (combo->currentIndex() == combo->count() - 1)
        combo->setCurrentIndex(0);
    else
        combo->setCurrentIndex(combo->currentIndex() + 1);
}

void DustendWindow::setEngLang()
{
    if (!ui->bt_engLang->isChecked()) {
        if (!ui->bt_rusLang->isChecked()                
                && !ui->bt_ukrLang->isChecked())
            ui->bt_engLang->setChecked(true);
    }
    else {
        ui->bt_rusLang->setChecked(false);        
        ui->bt_ukrLang->setChecked(false);
        QString locale = "en";
        genSetLogic->setLocale(&locale);
        setUILanguage();
    }
}

void DustendWindow::setRusLang()
{
    if (!ui->bt_rusLang->isChecked()) {
        if (!ui->bt_engLang->isChecked()                
                && !ui->bt_ukrLang->isChecked())
            ui->bt_rusLang->setChecked(true);
    }
    else {
        ui->bt_engLang->setChecked(false);        
        ui->bt_ukrLang->setChecked(false);
        QString locale = "ru";
        genSetLogic->setLocale(&locale);
        setUILanguage();
    }
}

void DustendWindow::setUkrLang()
{
    if (!ui->bt_ukrLang->isChecked()) {
        if (!ui->bt_rusLang->isChecked()               
                && !ui->bt_engLang->isChecked())
            ui->bt_ukrLang->setChecked(true);
    }
    else {
        ui->bt_rusLang->setChecked(false);        
        ui->bt_engLang->setChecked(false);
        QString locale = "uk";
        genSetLogic->setLocale(&locale);
        setUILanguage();
    }
}

bool DustendWindow::eventFilter(QObject *obj, QEvent *ev)
{    
    //check mouse click on frames
    if(ev->type() == QEvent::MouseButtonPress) {
        if(obj == ui->arr_left)
            addCooler(ui->cbox_left_num);
        if(obj == ui->arr_hot_up)
            addCooler(ui->cbox_hot_up_num);
        if(obj == ui->arr_up)
            addCooler(ui->cbox_up_num);
        if(obj == ui->arr_side)
            addCooler(ui->cbox_side_num);
        if(obj == ui->arr_hot_left)
            addCooler(ui->cbox_hot_left_num);
        if(obj == ui->logoLbl)
            QDesktopServices::openUrl(QUrl("http://www.dustend.com"));
//        if(obj == ui->pbTime)
//            showPbStart(true);
//        if(obj == ui->pbStart)
//            showPbStart(false);
    }
    if (ev->type() == QEvent::HoverEnter || ev->type() == QEvent::HoverMove) {
        if (obj == ui->pbTime)
            setPbTimeStyle(true);
    }
    if (ev->type() == QEvent::HoverLeave) {
        if (obj == ui->pbTime)
            setPbTimeStyle(false);
//        if(obj == ui->pbStart)
//            showPbStart(false);
    }

    if(ev->type() == QEvent::MouseMove) {
        setCBoxStylesOnArrowHover(obj);
        setCBoxNumAndArrowStylesOnCBoxNumHover(obj);
        setCBoxSizeAndArrowStylesOnCBoxSizeHover(obj);
    }

    if(ev->type() == QEvent::Leave) {
        setCBoxStylesOnArrowExit(obj);
        setCBoxNumAndArrowStylesOnCBoxNumExit(obj);
        setCBoxSizeAndArrowStylesOnCBoxSizeExit(obj);
    }
    return QMainWindow::eventFilter(obj,ev);
}

void DustendWindow::setEventFilterToFrames()
{
    ui->arr_left->installEventFilter(this);
    ui->arr_hot_up->installEventFilter(this);
    ui->arr_up->installEventFilter(this);
    ui->arr_side->installEventFilter(this);
    ui->arr_hot_left->installEventFilter(this);

    ui->cbox_hot_left_num->installEventFilter(this);
    ui->cbox_left_num->installEventFilter(this);
    ui->cbox_hot_up_num->installEventFilter(this);
    ui->cbox_up_num->installEventFilter(this);
    ui->cbox_side_num->installEventFilter(this);

    ui->cbox_hot_left_size->installEventFilter(this);
    ui->cbox_left_size->installEventFilter(this);
    ui->cbox_hot_up_size->installEventFilter(this);
    ui->cbox_up_size->installEventFilter(this);
    ui->cbox_side_size->installEventFilter(this);

    ui->logoLbl->installEventFilter(this);
    ui->pbStart->installEventFilter(this);
    ui->pbTime->installEventFilter(this);
}

void DustendWindow::setPbTimeStyle(bool mouseOnIt)
{
    if (!mouseOnIt)
        ui->pbTime->setStyleSheet("QProgressBar { border: 1px solid grey; "
                                  "border-radius: 0px; color: black; "
                                  "selection-background-color: green; }"
                                  "QProgressBar::chunk { width: 5px; color: black; }"
                                  "QToolTip { border: 1px solid darkgray; "
                                  "border-radius: 0px; background-color: lightgray; "
                                  "color: black; }");
    else
        ui->pbTime->setStyleSheet("QProgressBar { border: 1px solid grey; "
                                  "border-radius: 0px; color: black; "
                                  "selection-background-color: green; "
                                  "font-weight: bold; }"
                                  "QProgressBar::chunk { width: 5px; color: black; }"
                                  "QToolTip { border: 1px solid darkgray; "
                                  "border-radius: 0px; background-color: lightgray; "
                                  "color: black; }");
}

void DustendWindow::readProcessorName()
{
    QString prName;
    bool del = false;
    QString tempPrName = QString::fromStdString(processorName());
    for (int i = 0; i < tempPrName.length(); ++i) {
        if (tempPrName[i] == ' ') {
            if (tempPrName[i + 1] == ' ') {
                tempPrName.remove(i, 1);
                --i;
            }
        }
        if (tempPrName[i] == '(') {
            tempPrName.remove(i, 1);
            del = true;
        }
        if (tempPrName[i] == ')') {
            tempPrName.remove(i, 1);
            del = false;
        }
        else
            if (del) {
                tempPrName.remove(i, 1);
                --i;
            }
    }
    prName = tempPrName;
    prName += ", ";
    prName += QString("%1GB RAM").arg(ramSize());
    prName += ", ";
    prName += QString::fromStdString(videoName());
    ui->deviceLbl->setText(prName);
}

void DustendWindow::setInfoPage()
{
    mTEdit = new myTextEdit(this);
    ui->infoTextMainLayout->addWidget(mTEdit);
    mTEdit->setReadOnly(true);

    connect(mTEdit, SIGNAL( anchor_clicked(const QString &) ), this, SLOT( slotAnchorClicked(const QString &) ) );

    QString html = QString(tr("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"));
    html += tr("<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">");
    html += tr("p, li { white-space: pre-wrap; }");
    html += tr("</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">");
    html += tr("<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">PC Dust Control - С‚Р°Р№РјРµСЂ РѕР±СЂР°С‚РЅРѕРіРѕ РѕС‚СЃС‡РµС‚Р°, РЅР°РїРѕРјРёРЅР°РµС‚ Рѕ С‡РёСЃС‚РєРµ РєРѕРјРїСЊСЋС‚РµСЂР° РѕС‚ РїС‹Р»Рё РёР»Рё Р·Р°РјРµРЅРµ РїС‹Р»РµРІС‹С… С„РёР»СЊС‚СЂРѕРІ. </span><a href=\"http://www.dustend.com\"><span style=\" font-size:12pt; text-decoration: underline; color:#0000ff;\">РџСЂРѕРІРµСЂРёС‚СЊ РѕР±РЅРѕРІР»РµРЅРёРµ...</span></a></p>");
    html += tr("<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">.....................................................................................</span></p>");
    html += tr("<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">РџРѕРјРѕС‰СЊ</span></p>");
    html += tr("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">РРЅС„РѕСЂРјР°С†РёСЏ Рѕ С‡РёСЃС‚РєРµ РєРѕРјРїСЊСЋС‚РµСЂР° РѕС‚ РїС‹Р»Рё Рё РїС‹Р»РµРІС‹С… С„РёР»СЊС‚СЂР°С… РЅР°С…РѕРґРёС‚СЃСЏ </span><a href=\"http://www.boulten.com\"><span style=\" font-size:12pt; text-decoration: underline; color:#0000ff;\">Р·РґРµСЃСЊ...</span></a></p>");
    html += tr("<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">.....................................................................................</span></p>");
    html += tr("<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">РљРѕРЅС‚Р°РєС‚</span></p>");
    html += tr("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">РђРІС‚РѕСЂ: РђР»РµРєСЃР°РЅРґСЂ РљРёРєР»РµРІРёС‡</span></p>");
    html += tr("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">E-mail: </span><a href=\"mailto:foo@mail.com\"><span style=\" font-size:12pt; text-decoration: underline; color:#0000ff;\">mail@dustend.com</span></a></p>");
    html += tr("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">Site: </span><a href=\"http://www.boulten.com\"><span style=\" font-size:12pt; text-decoration: underline; color:#0000ff;\">http://www.boulten.com</span></a></p>");
    html += tr("<p> </p>");
    html += tr("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">Р Р°Р·СЂР°Р±РѕС‚С‡РёРє: РРіРѕСЂСЊ РЎРµРІРёР·РґСЂР°Р»</span></p>");
    html += tr("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">E-mail: </span><a href=\"mailto:foo@mail.com\"><span style=\" font-size:12pt; text-decoration: underline; color:#0000ff;\">kasperq@mail.ru</span></a></p>");
    html += tr("<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p>");
    html += tr("<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p></body></html>");

    mTEdit->setHtml(html);
}

void DustendWindow::setProgressBar()
{    
    if (ui->compTabWdg->currentIndex() == 0) {
        ui->pbTime->setMaximum(2600);
        bool withAverageUsageTime = true;
        if (ui->cleanTab->isVisible()) {
            ui->pbTime->setFormat(compL->getShortCleanTime(&localIp, &withAverageUsageTime));
            //        ui->pbTime->setMaximum(compL->getMaxCleanHours());
            int curHours = compL->getCurrentCleanHours(&localIp);
            if (compL->getCurrentCleanHours(&localIp) < 5) {
                //            setProgressBarYellow();
                curHours *= 10;
            }
            //        if (compL->getCurrentCleanHours(&localIp) < 1)
            //            setProgressBarRed();
            //        if (compL->getCurrentCleanHours(&localIp) >= 5)
            //            setProgressBarGreen();
            ui->pbTime->setValue(curHours);
        }
        else {
            ui->pbTime->setFormat(compL->getShortFilterTime(&localIp, &withAverageUsageTime));
            ui->pbTime->setValue(compL->getCurrentFilterHours(&localIp));
            //        ui->pbTime->setMaximum(compL->getMaxFilterHours());
        }
        ui->pbTime->repaint();
        ui->pbTime->update();
        ui->pbTime->repaint();
        if (ui->actionTabWdg->currentIndex() == 0)
            ui->pbStart->setMaximum(1300);
        if (ui->actionTabWdg->currentIndex() == 1)
            ui->pbStart->setMaximum(2600);
        ui->pbStart->setValue(compL->getLastMaxTimeHours(&localIp).toInt());
        ui->pbStart->setFormat(compL->getLastMaxTime(&localIp));
        ui->pbStart->repaint();
    }
}

void DustendWindow::setProgressBarRed()
{
    ui->pbTime->setStyleSheet("QProgressBar { border: 2px solid grey; "
                              "background-color: qlineargradient("
                              "spread:pad, x1:1, y1:1, x2:1, y2:0, "
                              "stop:0 rgba(255, 0, 0, 255), stop:1 "
                              "rgba(255, 147, 118, 255));"
                              "border-radius: 5px; color: black; }"
                              "QProgressBar::chunk {"
                              "background-color: qlineargradient("
                              "spread:pad, x1:1, y1:1, x2:1, y2:0, "
                              "stop:0 rgba(255, 0, 0, 255), stop:1 "
                              "rgba(255, 147, 118, 255)); width: 1px;"
                              "color: black; }");
}

void DustendWindow::setProgressBarYellow()
{
    ui->pbTime->setStyleSheet("QProgressBar { border: 2px solid grey; "
                              "border-radius: 5px; color: black; }"
                              "QProgressBar::chunk {"
                              "background-color: qlineargradient("
                              "spread:pad, x1:1, y1:1, x2:1, y2:0, "
                              "stop:0 rgba(255, 162, 0, 255), stop:1 "
                              "rgba(255, 255, 118, 255)); width: 1px;"
                              "color: black; }");
}

void DustendWindow::setProgressBarGreen()
{
    ui->pbTime->setStyleSheet("QProgressBar { border: 2px solid grey; "
                              "border-radius: 5px; color: black; }"
                              "QProgressBar::chunk {"
                              "background-color: qlineargradient("
                              "spread:reflect, x1:1, y1:1, x2:1, y2:0, "
                              "stop:0 rgba(0, 129, 0, 255), stop:1 "
                              "rgba(0, 223, 0, 255)); width: 1px;"
                              "color: black; }");
}

void DustendWindow::showComputerComment()
{
    if (localIp == "127.0.0.1")
        ui->commentLbl->setText(myTextLabel);
    else {
        if (ui->compTabWdg->currentIndex() == 1) {
            QModelIndexList selection = ui->networkTbl->selectionModel()->selectedRows(0);
            if (!selection.empty()) {
                QModelIndex index = selection.at(0);
                ui->commentLbl->setText(QString("%1 %2").arg(netTextLabel).arg(table->data(index, Qt::DisplayRole).toString()));
            }
        }
    }
}

int DustendWindow::getCoolerDiameter(QString cBoxText)
{    
    if (cBoxText == "120 mm")
        return 120;
    else {
        if (cBoxText == "140 mm")
            return 140;
        else {
            if (cBoxText == "200 mm")
                return 200;
            else {
                if (cBoxText == "230 mm")
                    return 230;
                else
                    return 80;
            }
        }
    }
}

QString DustendWindow::getCBoxStyleSheetOnArrowHovered()
{
    QString styleStr;
    styleStr = "QComboBox::down-arrow { image: url(:/images/down3.png); "
               "width: 14; height: 14; }"
               "QComboBox::drop-down { subcontrol-origin: padding;"
               "subcontrol-position: top right; width: 15px; color: red;"
               "border-left-width: 1px; border-left-color: darkgray;"
               "border-left-style: solid; border-top-right-radius: 3px; "
               "border-bottom-right-radius: 3px;	}"
            "QComboBox { "
            "background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, "
            "y2:0, stop:0 rgba(160, 160, 160, 255), stop:1 rgba(255, 255, 255, 255));"
            "selection-color: white; selection-background-color: gray;"
            "border: 1px solid gray; border-color: darkgray; border-radius: 3px;"
            "padding: 1px 1px 1px 3px; color: black; }"
            "QToolTip { border: 1px solid darkgray; border-radius: 0px;"
            "background-color: lightgray; color: black; }";
    return styleStr;
}

QString DustendWindow::getCBoxStyleSheetOnArrowExit()
{
    QString styleStr;
    styleStr = "QComboBox { background-color: white; selection-color: white;"
            "selection-background-color: gray; border: 1px solid gray;"
            "border-color: darkgray; border-radius: 3px; padding: 1px 1px 1px 3px; }"
            "QComboBox:on { padding-top: 3px; padding-left: 4px; }"
            "QComboBox::drop-down { subcontrol-origin: padding; "
            "subcontrol-position: top right; width: 15px;"
            "border-left-width: 1px; border-left-color: darkgray;"
            "border-left-style: solid; border-top-right-radius: 3px; "
            "border-bottom-right-radius: 3px; }"
            "QComboBox::down-arrow { image: url(:/images/down3.png); width: 14;"
            "height: 14; }"
            "QComboBox::drop-down:hover { subcontrol-origin: padding;"
            "subcontrol-position: top right; width: 15px; color: red;"
            "border-left-width: 1px; border-left-color: darkgray;"
            "border-left-style: solid; border-top-right-radius: 3px;"
            "border-bottom-right-radius: 3px; }"
            "QComboBox:hover { background-color: qlineargradient(spread:pad,"
            "x1:1, y1:1, x2:1, y2:0, stop:0 rgba(160, 160, 160, 255), stop:1"
            "rgba(255, 255, 255, 255)); selection-color: white;"
            "selection-background-color: gray; border: 1px solid gray;"
            "border-color: darkgray; border-radius: 3px; "
            "padding: 1px 1px 1px 3px; color: black; }"
            "QToolTip { border: 1px solid darkgray; border-radius: 0px;"
            "background-color: lightgray; color: black; }";
    return styleStr;
}

QString DustendWindow::getArrowStyleOnCBoxHovered(QString arrowPixmap)
{
    QString styleStr;
    styleStr = "QLabel { border-color: transparent; border-radius: 5px;"
            "border-style: solid; background-color: transparent; color: white;"
            "background-image: url(:/images/" + arrowPixmap + "); }"
            "QLabel:pressed { color: white; background-color: lightgray;"
            "border-color: black; border-style: inset; border-width: 1px;"
            "border-radius: 5px; }"
            "QToolTip { border: 1px solid darkgray; border-radius: 0px;"
            "background-color: lightgray; color: black; }";
    return styleStr;
}

QString DustendWindow::getArrowStyleOnCBoxExit(QString arrowPixmap)
{
    QString styleStr;
    styleStr = "QLabel { border-color: transparent;"
            "background-color: transparent; color: white; }"
            "QLabel:hover { border-color: transparent; border-radius: 5px;"
            "border-style: solid; background-color: transparent; color: white;"
            "background-image: url(:/images/" + arrowPixmap + "); }"
            "QLabel:pressed { color: white; background-color: lightgray;"
            "border-color: black; border-style: inset; border-width: 1px;"
            "border-radius: 5px; }"
            "QToolTip { border: 1px solid darkgray; border-radius: 0px;"
            "background-color: lightgray; color: black; }";
    return styleStr;
}

void DustendWindow::setCBoxStylesOnArrowHover(QObject *obj)
{
    if (obj == ui->arr_hot_left) {
        ui->cbox_hot_left_num->setStyleSheet(getCBoxStyleSheetOnArrowHovered());
        ui->cbox_hot_left_size->setStyleSheet(getCBoxStyleSheetOnArrowHovered());
    }
    if (obj == ui->arr_hot_up) {
        ui->cbox_hot_up_num->setStyleSheet(getCBoxStyleSheetOnArrowHovered());
        ui->cbox_hot_up_size->setStyleSheet(getCBoxStyleSheetOnArrowHovered());
    }
    if (obj == ui->arr_left) {
        ui->cbox_left_num->setStyleSheet(getCBoxStyleSheetOnArrowHovered());
        ui->cbox_left_size->setStyleSheet(getCBoxStyleSheetOnArrowHovered());
    }
    if (obj == ui->arr_up) {
        ui->cbox_up_num->setStyleSheet(getCBoxStyleSheetOnArrowHovered());
        ui->cbox_up_size->setStyleSheet(getCBoxStyleSheetOnArrowHovered());
    }
    if (obj == ui->arr_side) {
        ui->cbox_side_num->setStyleSheet(getCBoxStyleSheetOnArrowHovered());
        ui->cbox_side_size->setStyleSheet(getCBoxStyleSheetOnArrowHovered());
    }
}

void DustendWindow::setCBoxNumAndArrowStylesOnCBoxNumHover(QObject *obj)
{
    if (obj == ui->cbox_hot_left_num) {
        ui->cbox_hot_left_num->setStyleSheet(getCBoxStyleSheetOnArrowHovered());
        ui->arr_hot_left->setStyleSheet(getArrowStyleOnCBoxHovered("244.png"));
    }
    if (obj == ui->cbox_hot_up_num) {
        ui->cbox_hot_up_num->setStyleSheet(getCBoxStyleSheetOnArrowHovered());
        ui->arr_hot_up->setStyleSheet(getArrowStyleOnCBoxHovered("243.png"));
    }
    if (obj == ui->cbox_left_num) {
        ui->cbox_left_num->setStyleSheet(getCBoxStyleSheetOnArrowHovered());
        ui->arr_left->setStyleSheet(getArrowStyleOnCBoxHovered("246.png"));
    }
    if (obj == ui->cbox_up_num) {
        ui->cbox_up_num->setStyleSheet(getCBoxStyleSheetOnArrowHovered());
        ui->arr_up->setStyleSheet(getArrowStyleOnCBoxHovered("245.png"));
    }
    if (obj == ui->cbox_side_num) {
        ui->cbox_side_num->setStyleSheet(getCBoxStyleSheetOnArrowHovered());
        ui->arr_side->setStyleSheet(getArrowStyleOnCBoxHovered("241.png"));
    }
}

void DustendWindow::setCBoxSizeAndArrowStylesOnCBoxSizeHover(QObject *obj)
{
    if (obj == ui->cbox_hot_left_size) {
        ui->cbox_hot_left_size->setStyleSheet(getCBoxStyleSheetOnArrowHovered());
        ui->arr_hot_left->setStyleSheet(getArrowStyleOnCBoxHovered("244.png"));
    }
    if (obj == ui->cbox_hot_up_size) {
        ui->cbox_hot_up_size->setStyleSheet(getCBoxStyleSheetOnArrowHovered());
        ui->arr_hot_up->setStyleSheet(getArrowStyleOnCBoxHovered("243.png"));
    }
    if (obj == ui->cbox_left_size) {
        ui->cbox_left_size->setStyleSheet(getCBoxStyleSheetOnArrowHovered());
        ui->arr_left->setStyleSheet(getArrowStyleOnCBoxHovered("246.png"));
    }
    if (obj == ui->cbox_up_size) {
        ui->cbox_up_size->setStyleSheet(getCBoxStyleSheetOnArrowHovered());
        ui->arr_up->setStyleSheet(getArrowStyleOnCBoxHovered("245.png"));
    }
    if (obj == ui->cbox_side_size) {
        ui->cbox_side_size->setStyleSheet(getCBoxStyleSheetOnArrowHovered());
        ui->arr_side->setStyleSheet(getArrowStyleOnCBoxHovered("241.png"));
    }
}

void DustendWindow::setCBoxStylesOnArrowExit(QObject *obj)
{
    if (obj == ui->arr_hot_left) {
        ui->cbox_hot_left_num->setStyleSheet(getCBoxStyleSheetOnArrowExit());
        ui->cbox_hot_left_size->setStyleSheet(getCBoxStyleSheetOnArrowExit());
    }
    if (obj == ui->arr_hot_up) {
        ui->cbox_hot_up_num->setStyleSheet(getCBoxStyleSheetOnArrowExit());
        ui->cbox_hot_up_size->setStyleSheet(getCBoxStyleSheetOnArrowExit());
    }
    if (obj == ui->arr_left) {
        ui->cbox_left_num->setStyleSheet(getCBoxStyleSheetOnArrowExit());
        ui->cbox_left_size->setStyleSheet(getCBoxStyleSheetOnArrowExit());
    }
    if (obj == ui->arr_up) {
        ui->cbox_up_num->setStyleSheet(getCBoxStyleSheetOnArrowExit());
        ui->cbox_up_size->setStyleSheet(getCBoxStyleSheetOnArrowExit());
    }
    if (obj == ui->arr_side) {
        ui->cbox_side_num->setStyleSheet(getCBoxStyleSheetOnArrowExit());
        ui->cbox_side_size->setStyleSheet(getCBoxStyleSheetOnArrowExit());
    }
}

void DustendWindow::setCBoxNumAndArrowStylesOnCBoxNumExit(QObject *obj)
{
    if (obj == ui->cbox_hot_left_num) {
        ui->cbox_hot_left_num->setStyleSheet(getCBoxStyleSheetOnArrowExit());
        ui->arr_hot_left->setStyleSheet(getArrowStyleOnCBoxExit("244.png"));
    }
    if (obj == ui->cbox_hot_up_num) {
        ui->cbox_hot_up_num->setStyleSheet(getCBoxStyleSheetOnArrowExit());
        ui->arr_hot_up->setStyleSheet(getArrowStyleOnCBoxExit("243.png"));
    }
    if (obj == ui->cbox_left_num) {
        ui->cbox_left_num->setStyleSheet(getCBoxStyleSheetOnArrowExit());
        ui->arr_left->setStyleSheet(getArrowStyleOnCBoxExit("246.png"));
    }
    if (obj == ui->cbox_up_num) {
        ui->cbox_up_num->setStyleSheet(getCBoxStyleSheetOnArrowExit());
        ui->arr_up->setStyleSheet(getArrowStyleOnCBoxExit("245.png"));
    }
    if (obj == ui->cbox_side_num) {
        ui->cbox_side_num->setStyleSheet(getCBoxStyleSheetOnArrowExit());
        ui->arr_side->setStyleSheet(getArrowStyleOnCBoxExit("241.png"));
    }
}

void DustendWindow::setCBoxSizeAndArrowStylesOnCBoxSizeExit(QObject *obj)
{
    if (obj == ui->cbox_hot_left_size) {
        ui->cbox_hot_left_size->setStyleSheet(getCBoxStyleSheetOnArrowExit());
        ui->arr_hot_left->setStyleSheet(getArrowStyleOnCBoxExit("244.png"));
    }
    if (obj == ui->cbox_hot_up_size) {
        ui->cbox_hot_up_size->setStyleSheet(getCBoxStyleSheetOnArrowExit());
        ui->arr_hot_up->setStyleSheet(getArrowStyleOnCBoxExit("243.png"));
    }
    if (obj == ui->cbox_left_size) {
        ui->cbox_left_size->setStyleSheet(getCBoxStyleSheetOnArrowExit());
        ui->arr_left->setStyleSheet(getArrowStyleOnCBoxExit("246.png"));
    }
    if (obj == ui->cbox_up_size) {
        ui->cbox_up_size->setStyleSheet(getCBoxStyleSheetOnArrowExit());
        ui->arr_up->setStyleSheet(getArrowStyleOnCBoxExit("245.png"));
    }
    if (obj == ui->cbox_side_size) {
        ui->cbox_side_size->setStyleSheet(getCBoxStyleSheetOnArrowExit());
        ui->arr_side->setStyleSheet(getArrowStyleOnCBoxExit("241.png"));
    }
}

void DustendWindow::loadNetTblSettings()
{    
    if (!genSetLogic->getName())
        ui->networkTbl->hideColumn(0);
    ui->cb_showNetName->setChecked(genSetLogic->getName());
    if (!genSetLogic->getIpAddress())
        ui->networkTbl->hideColumn(1);
    ui->cb_showIp->setChecked(genSetLogic->getIpAddress());
    if (!genSetLogic->getCleanTime())
        ui->networkTbl->hideColumn(2);
    ui->cb_showClean->setChecked(genSetLogic->getCleanTime());
    if (!genSetLogic->getFilterTime())
        ui->networkTbl->hideColumn(3);
    ui->cb_showFilter->setChecked(genSetLogic->getFilterTime());
    if (!genSetLogic->getOs())
        ui->networkTbl->hideColumn(4);
    ui->cb_showOs->setChecked(genSetLogic->getOs());
    if (!genSetLogic->getTime())
        ui->networkTbl->hideColumn(5);
    ui->cb_showTime->setChecked(genSetLogic->getTime());
    if (!genSetLogic->getStatus())
        ui->networkTbl->hideColumn(6);
    ui->cb_showStatus->setChecked(genSetLogic->getStatus());
}

void DustendWindow::loadLangSettings()
{
    QString locale;
    locale = genSetLogic->getLocale();
    ui->bt_engLang->setChecked(false);
    if (locale == "en") {
        ui->bt_engLang->setChecked(true);
        ui->actionTabWdg->setTabIcon(1, QIcon(":/images/Dustend19.ico"));
    }
    if (locale == "ru") {
        ui->bt_rusLang->setChecked(true);
        ui->actionTabWdg->setTabIcon(1, QIcon(":/images/FS8.ico"));
    }    
    if (locale == "uk") {
        ui->bt_ukrLang->setChecked(true);
        ui->actionTabWdg->setTabIcon(1, QIcon(":/images/FS8.ico"));
    }
}

bool DustendWindow::isAddressBelongsToMe(QString *curIpAddress)
{
    QString myIpAddress;
    bool isHere = false;
    for (int i = 0; i < ipAddressesList.size(); ++i){
        myIpAddress = ipAddressesList.at(i).toString();              
        if (myIpAddress == *curIpAddress) {
            isHere = true;
            break;
        }        
    }
    return isHere;
}

void DustendWindow::asmInsertion()
{
//    int tem = 0;

//    __asm {         // for Amd processors
//        CPUID
//        mov edx, 0cf8h;
//        mov eax, 80000000h or (0 shl 16) or (24 shl 11) or (3 shl 8) or 0e4h;
//        outp dx, eax
//        : "dx" (eax);
//        mov edx, 0cfch;
//        in eax, dx

//        mov edx, eax;
//        shr eax, 16
//            and eax, 0ffh
//        sub eax, 49
//        mov tem, eax;
//    }


//    __asm {             // for Intel processors
//        mov ecx,19ch
//        rdmsr
//                shr eax,16
//                and eax,7fh
//                neg eax
//                add eax,100
//                mov tem, eax
//    }
//    qDebug() << "temp " << tem;

    // измерения для времени(тиков процессора)
    //    long long t1,t2;
    //    __asm{
    //        CPUID // страхуемся от "out-of-order" исполнения инструкций
    //        RDTSC // делаем первый замер
    //        mov DWORD PTR[t1], eax // сохраняем 64-битный результат из EDX:EAX
    //        mov DWORD PTR[t1 + 4], edx // в t1
    //    }
        // что нужно тестить на время исполнения
    //    for(int i = 0; i < 100; ++i)
    //        std::cout << "hey\n";
    //    __asm{ // делаем аналогичное для второго замера
    //        CPUID
    //                RDTSC
    //                mov DWORD PTR[t2], eax
    //                mov DWORD PTR[t2 + 4], edx
    //    }
        // количество тиков процессора, сделанное за время выполнения теста
//    long long delta_t = t2 - t1;
    //    std::cout << delta_t << std::endl;
}

void DustendWindow::setComboBoxes()
{
    if (ui->cbox_hot_up_num->currentIndex() == 3)
        ui->cbox_hot_up_size->removeItem(4);
    if (ui->cbox_hot_up_num->currentIndex() == 4) {
        ui->cbox_hot_up_size->removeItem(4);
        ui->cbox_hot_up_size->removeItem(3);
    }

    if (ui->cbox_left_num->currentIndex() == 3)
        ui->cbox_left_size->removeItem(4);
    if (ui->cbox_left_num->currentIndex() == 4) {
        ui->cbox_left_size->removeItem(4);
        ui->cbox_left_size->removeItem(3);
    }

    if (ui->cbox_side_num->currentIndex() == 3)
        ui->cbox_side_size->removeItem(4);
    if (ui->cbox_side_num->currentIndex() == 4) {
        ui->cbox_side_size->removeItem(4);
        ui->cbox_side_size->removeItem(3);
    }
}

void DustendWindow::showPbStart(bool show)
{
    ui->pbStart->update();
    if (compL->getCleanTimeIsRunning(&localIp)
        || compL->getFilterTimeIsRunning(&localIp)) {
        ui->pbStart->setVisible(show);
        ui->pbTime->setVisible(!show);
        ui->startLbl->setVisible(show);
        if (!show) {
            if (ui->actionTabWdg->currentIndex() == 0) {
                ui->cleanLbl->setVisible(!show);
                ui->filterLbl->setVisible(show);
            }
            else {
                ui->cleanLbl->setVisible(show);
                ui->filterLbl->setVisible(!show);
            }
        }
        else {
            ui->cleanLbl->setVisible(!show);
            ui->filterLbl->setVisible(!show);
        }
    }
    ui->pbStart->update();
}

void DustendWindow::setWarningTimeSettings()
{
    if (warningTime == 0)
        ui->rb_dontShowWarning->setChecked(true);
    if (warningTime == 10)
        ui->rb_show10minsWarning->setChecked(true);
    if (warningTime == 30)
        ui->rb_show30minsWarning->setChecked(true);
    if (warningTime == 60)
        ui->rb_show60minsWarning->setChecked(true);
    if (warningTime == 666666)
        ui->rb_showAtStartWarning->setChecked(true);
}

void DustendWindow::loadEmailSettings()
{
    sendEmail = genSetLogic->getSendMail();
    ui->cb_sendEmail->setChecked(sendEmail);
    emailAddress = genSetLogic->getEmailAddress();
    ui->emailEdit->setText(emailAddress);
    mailThr = new MailThread();
    mailThr->setEmailAddress(&emailAddress);
}

void DustendWindow::slotAnchorClicked(const QString & href)
{
    QDesktopServices::openUrl( QUrl(href) );
}

void DustendWindow::sortNet(int sortedCol, Qt::SortOrder order)
{
    if (sortedCol == 0)
        netLogic.sortByName(networkComps, order);
    if (sortedCol == 5)
        netLogic.sortByTime(networkComps, order);
    if (sortedCol == 1)
        netLogic.sortByIpAddress(networkComps, order);
    if (sortedCol == 2)
        netLogic.sortByCleanTime(networkComps, order);
    if (sortedCol == 3)
        netLogic.sortByFilterTime(networkComps, order);
    if (sortedCol == 4)
        netLogic.sortByOs(networkComps, order);

    if (sortedCol == 0 || sortedCol == 1 || sortedCol == 2 || sortedCol == 3
        || sortedCol == 4 || sortedCol == 5)
        fillTableModel();
    ui->networkTbl->repaint();
}

void DustendWindow::saveNetSettings()
{
    bool curValue = ui->cb_showNetName->isChecked();
    genSetLogic->setName(&curValue);
    curValue = ui->cb_showIp->isChecked();
    genSetLogic->setIpAddress(&curValue);
    curValue = ui->cb_showClean->isChecked();
    genSetLogic->setCleanTime(&curValue);
    curValue = ui->cb_showFilter->isChecked();
    genSetLogic->setFilterTime(&curValue);
    curValue = ui->cb_showOs->isChecked();
    genSetLogic->setOs(&curValue);
    curValue = ui->cb_showTime->isChecked();
    genSetLogic->setTime(&curValue);
    curValue = ui->cb_showStatus->isChecked();
    genSetLogic->setStatus(&curValue);

    if (ui->cb_showNetName->isChecked())
        ui->networkTbl->showColumn(0);
    else
        ui->networkTbl->hideColumn(0);
    if (ui->cb_showIp->isChecked())
        ui->networkTbl->showColumn(1);
    else
        ui->networkTbl->hideColumn(1);
    if (ui->cb_showClean->isChecked())
        ui->networkTbl->showColumn(2);
    else
        ui->networkTbl->hideColumn(2);
    if (ui->cb_showFilter->isChecked())
        ui->networkTbl->showColumn(3);
    else
        ui->networkTbl->hideColumn(3);
    if (ui->cb_showOs->isChecked())
        ui->networkTbl->showColumn(4);
    else
        ui->networkTbl->hideColumn(4);
    if (ui->cb_showTime->isChecked())
        ui->networkTbl->showColumn(5);
    else
        ui->networkTbl->hideColumn(5);
    if (ui->cb_showStatus->isChecked())
        ui->networkTbl->showColumn(6);
    else
        ui->networkTbl->hideColumn(6);
}

void DustendWindow::execStopDialog()
{
    stopDialog.exec();
}

void DustendWindow::setDontShowWarningMsg()
{
    warningTime = 0;
    genSetLogic->setWarningInterval(&warningTime);
    setWarningTimer();
}

void DustendWindow::set10MinsWarningMsg()
{
    warningTime = 10;
    genSetLogic->setWarningInterval(&warningTime);
    setWarningTimer();
}

void DustendWindow::set30MinsWarningMsg()
{
    warningTime = 30;
    genSetLogic->setWarningInterval(&warningTime);
    setWarningTimer();
}

void DustendWindow::set60MinsWarningMsg()
{
    warningTime = 60;
    genSetLogic->setWarningInterval(&warningTime);
    setWarningTimer();
}

void DustendWindow::setStartupWarningMsg()
{
    warningTime = 666666;
    genSetLogic->setWarningInterval(&warningTime);
    setWarningTimer();
}

void DustendWindow::setWarningTimer()
{
    warningTimer.stop();
    if (warningTime != 666666 && warningTime != 0) {
        warningTimer.setInterval(warningTime * 3600 * 1000);
        warningTimer.start();
    }
}

void DustendWindow::setSendEmail()
{
    if (ui->cb_sendEmail->isChecked())
        sendEmail = true;
   else
        sendEmail = false;
    genSetLogic->setSendMail(&sendEmail);
}

void DustendWindow::setEmail(QString newEmailAddress)
{
    genSetLogic->setEmailAddress(&newEmailAddress);
    emailAddress = newEmailAddress;
}

void DustendWindow::setAutorunProgam()
{
    bool autorun = ui->rb_autorunProgr->isChecked();
    if (autorun)
        registerAutorun();
    else
        unRegisterAutorun();
    genSetLogic->setAutoStartApp(&autorun);
}

void DustendWindow::setUILanguage()
{
    QString locale = genSetLogic->getLocale();
    ui->btnTimer->setText(langVarLogic->getVariable(&locale, "btnTimer"));
    ui->btnFanControl->setText(langVarLogic->getVariable(&locale, "btnFanControl"));
    ui->btnSettings->setText(langVarLogic->getVariable(&locale, "btnSettings"));
    ui->lblHelp->setText(langVarLogic->getVariable(&locale, "lblHelp"));
    ui->lblCheckUpdates->setText(langVarLogic->getVariable(&locale, "lblCheckUpdates"));
    ui->bt_stop->setText(langVarLogic->getVariable(&locale, "bt_stop"));
    ui->bt_stop->setToolTip(langVarLogic->getVariable(&locale, "bt_stopHint"));
    ui->bt_pause->setText(langVarLogic->getVariable(&locale, "bt_pause"));
    ui->bt_pause->setToolTip(langVarLogic->getVariable(&locale, "bt_pauseHint"));
    ui->bt_start->setText(langVarLogic->getVariable(&locale, "bt_start"));
    ui->bt_start->setToolTip(langVarLogic->getVariable(&locale, "bt_startHint"));
    ui->cbDustyPlace->setText(langVarLogic->getVariable(&locale, "cbDustyPlace"));
    ui->cbDustyPlace->setToolTip(langVarLogic->getVariable(&locale, "cbDustyPlaceHint"));
    ui->actionTabWdg->setTabText(0, langVarLogic->getVariable(&locale, "cleanTab"));
    ui->actionTabWdg->setTabText(1, langVarLogic->getVariable(&locale, "filterTab"));
    ui->compTabWdg->setTabText(0, langVarLogic->getVariable(&locale, "myComputerTab"));
    ui->compTabWdg->setTabText(1, langVarLogic->getVariable(&locale, "networkTab"));
    ui->startLbl->setText(langVarLogic->getVariable(&locale, "startLbl"));
    ui->filterLbl->setText(langVarLogic->getVariable(&locale, "filterLbl"));
    ui->cleanLbl->setText(langVarLogic->getVariable(&locale, "cleanLbl"));
    ui->label_7->setText(langVarLogic->getVariable(&locale, "label_7"));
    ui->arr_hot_left->setToolTip(langVarLogic->getVariable(&locale, "arrHint"));
    ui->arr_hot_left_2->setToolTip(langVarLogic->getVariable(&locale, "arrHint"));
    ui->arr_hot_up->setToolTip(langVarLogic->getVariable(&locale, "arrHint"));
    ui->arr_hot_up_2->setToolTip(langVarLogic->getVariable(&locale, "arrHint"));
    ui->arr_left->setToolTip(langVarLogic->getVariable(&locale, "arrHint"));
    ui->arr_left_2->setToolTip(langVarLogic->getVariable(&locale, "arrHint"));
    ui->arr_side->setToolTip(langVarLogic->getVariable(&locale, "arrHint"));
    ui->arr_side_2->setToolTip(langVarLogic->getVariable(&locale, "arrHint"));
    ui->arr_up->setToolTip(langVarLogic->getVariable(&locale, "arrHint"));
    ui->arr_up_2->setToolTip(langVarLogic->getVariable(&locale, "arrHint"));
    ui->cbox_hot_left_num->setToolTip(langVarLogic->getVariable(&locale, "cbox_numHint"));
    ui->cbox_hot_up_num->setToolTip(langVarLogic->getVariable(&locale, "cbox_numHint"));
    ui->cbox_left_num->setToolTip(langVarLogic->getVariable(&locale, "cbox_numHint"));
    ui->cbox_side_num->setToolTip(langVarLogic->getVariable(&locale, "cbox_numHint"));
    ui->cbox_up_num->setToolTip(langVarLogic->getVariable(&locale, "cbox_numHint"));
    ui->cbox_hot_left_size->setToolTip(langVarLogic->getVariable(&locale, "cbox_sizeHint"));
    ui->cbox_hot_up_size->setToolTip(langVarLogic->getVariable(&locale, "cbox_sizeHint"));
    ui->cbox_left_size->setToolTip(langVarLogic->getVariable(&locale, "cbox_sizeHint"));
    ui->cbox_side_size->setToolTip(langVarLogic->getVariable(&locale, "cbox_sizeHint"));
    ui->cbox_up_size->setToolTip(langVarLogic->getVariable(&locale, "cbox_sizeHint"));
    ui->logoLbl->setToolTip(langVarLogic->getVariable(&locale, "logoLblHint"));
    ui->pbTime->setToolTip(langVarLogic->getVariable(&locale, "pbTimeHint"));
    ui->refreshNetBtn->setToolTip(langVarLogic->getVariable(&locale, "refreshNetBtnHint"));
    ui->rb_autorunProgr->setText(langVarLogic->getVariable(&locale, "rb_autorunProgr"));
    ui->groupBox_2->setTitle(langVarLogic->getVariable(&locale, "groupBox_2"));
    ui->bt_rusLang->setText(langVarLogic->getVariable(&locale, "bt_rusLang"));
    ui->bt_engLang->setText(langVarLogic->getVariable(&locale, "bt_engLang"));
    ui->groupBox->setTitle(langVarLogic->getVariable(&locale, "groupBox"));
    ui->cb_showNetName->setText(langVarLogic->getVariable(&locale, "cb_showNetName"));
    ui->cb_showIp->setText(langVarLogic->getVariable(&locale, "cb_showIp"));
    ui->cb_showClean->setText(langVarLogic->getVariable(&locale, "cb_showClean"));
    ui->cb_showFilter->setText(langVarLogic->getVariable(&locale, "cb_showFilter"));
    ui->cb_showTime->setText(langVarLogic->getVariable(&locale, "cb_showTime"));
    ui->cb_showOs->setText(langVarLogic->getVariable(&locale, "cb_showOs"));
    ui->cb_showStatus->setText(langVarLogic->getVariable(&locale, "cb_showStatus"));
    ui->groupBox_4->setTitle(langVarLogic->getVariable(&locale, "groupBox_4"));
    ui->cb_sendEmail->setText(langVarLogic->getVariable(&locale, "cb_sendEmail"));
    ui->label->setText(langVarLogic->getVariable(&locale, "label"));
    ui->emailEdit->setPlaceholderText(langVarLogic->getVariable(&locale, "emailEdit"));
    ui->groupBox_3->setTitle(langVarLogic->getVariable(&locale, "groupBox_3"));
    ui->rb_dontShowWarning->setText(langVarLogic->getVariable(&locale, "rb_dontShowWarning"));
    ui->rb_show10minsWarning->setText(langVarLogic->getVariable(&locale, "rb_show10minsWarning"));
    ui->rb_show30minsWarning->setText(langVarLogic->getVariable(&locale, "rb_show30minsWarning"));
    ui->rb_show60minsWarning->setText(langVarLogic->getVariable(&locale, "rb_show60minsWarning"));
    ui->rb_showAtStartWarning->setText(langVarLogic->getVariable(&locale, "rb_showAtStartWarning"));
    ui->cbox_hot_left_num->setItemText(0, langVarLogic->getVariable(&locale, "cb_num0"));
    ui->cbox_hot_left_num->setItemText(1, langVarLogic->getVariable(&locale, "cb_num1"));
    ui->cbox_hot_left_num->setItemText(2, langVarLogic->getVariable(&locale, "cb_num2"));
    ui->cbox_hot_left_num->setItemText(3, langVarLogic->getVariable(&locale, "cb_num3"));
    ui->cbox_hot_left_num->setItemText(4, langVarLogic->getVariable(&locale, "cb_num4"));
    ui->cbox_hot_up_num->setItemText(0, langVarLogic->getVariable(&locale, "cb_num0"));
    ui->cbox_hot_up_num->setItemText(1, langVarLogic->getVariable(&locale, "cb_num1"));
    ui->cbox_hot_up_num->setItemText(2, langVarLogic->getVariable(&locale, "cb_num2"));
    ui->cbox_hot_up_num->setItemText(3, langVarLogic->getVariable(&locale, "cb_num3"));
    ui->cbox_hot_up_num->setItemText(4, langVarLogic->getVariable(&locale, "cb_num4"));
    ui->cbox_left_num->setItemText(0, langVarLogic->getVariable(&locale, "cb_num0"));
    ui->cbox_left_num->setItemText(1, langVarLogic->getVariable(&locale, "cb_num1"));
    ui->cbox_left_num->setItemText(2, langVarLogic->getVariable(&locale, "cb_num2"));
    ui->cbox_left_num->setItemText(3, langVarLogic->getVariable(&locale, "cb_num3"));
    ui->cbox_left_num->setItemText(4, langVarLogic->getVariable(&locale, "cb_num4"));
    ui->cbox_up_num->setItemText(0, langVarLogic->getVariable(&locale, "cb_num0"));
    ui->cbox_up_num->setItemText(1, langVarLogic->getVariable(&locale, "cb_num1"));
    ui->cbox_up_num->setItemText(2, langVarLogic->getVariable(&locale, "cb_num2"));
    ui->cbox_side_num->setItemText(0, langVarLogic->getVariable(&locale, "cb_num0"));
    ui->cbox_side_num->setItemText(1, langVarLogic->getVariable(&locale, "cb_num1"));
    ui->cbox_side_num->setItemText(2, langVarLogic->getVariable(&locale, "cb_num2"));
    ui->cbox_side_num->setItemText(3, langVarLogic->getVariable(&locale, "cb_num3"));
    ui->cbox_side_num->setItemText(4, langVarLogic->getVariable(&locale, "cb_num4"));
    warningMsg = langVarLogic->getVariable(&locale, "warningMsg");
    stopDialogMsg = langVarLogic->getVariable(&locale, "stopDialog");
    okBtn = langVarLogic->getVariable(&locale, "okBtn");
    cancelBtn = langVarLogic->getVariable(&locale, "cancelBtn");
    msg = langVarLogic->getVariable(&locale, "msg");
    showMessage = langVarLogic->getVariable(&locale, "showMessage");
    minimize = langVarLogic->getVariable(&locale, "minimize");
    maximize = langVarLogic->getVariable(&locale, "maximize");
    restore = langVarLogic->getVariable(&locale, "restore");
    quit = langVarLogic->getVariable(&locale, "quit");
    trayFilter = langVarLogic->getVariable(&locale, "trayFilter");
    trayClean = langVarLogic->getVariable(&locale, "trayClean");
    wasFinded = langVarLogic->getVariable(&locale, "wasFinded");
    computer = langVarLogic->getVariable(&locale, "computer");
    scanning = langVarLogic->getVariable(&locale, "scanning");
    updating = langVarLogic->getVariable(&locale, "updating");
    myTextLabel = langVarLogic->getVariable(&locale, "myTextLabel");
    netTextLabel = langVarLogic->getVariable(&locale, "netTextLabel");
    compL->setDayDot(langVarLogic->getVariable(&locale, "dayDot"));
    compL->setTimeToClean(langVarLogic->getVariable(&locale, "timeToClean"));
    compL->setYourComp(langVarLogic->getVariable(&locale, "yourComp"));
    compL->setFilterCloged(langVarLogic->getVariable(&locale, "filterCloged"));
    nameHeader = langVarLogic->getVariable(&locale, "nameHeader");
    ipHeader = langVarLogic->getVariable(&locale, "ipHeader");
    cleanHeader = langVarLogic->getVariable(&locale, "cleanHeader");
    filterHeader = langVarLogic->getVariable(&locale, "filterHeader");
    osHeader = langVarLogic->getVariable(&locale, "osHeader");
    cfHeader = langVarLogic->getVariable(&locale, "cfHeader");
    timeHeader = langVarLogic->getVariable(&locale, "timeHeader");
    cleanHint = langVarLogic->getVariable(&locale, "cleanHint");
    filterHint = langVarLogic->getVariable(&locale, "filterHint");
    ui->bt_sendTest->setText(langVarLogic->getVariable(&locale, "bt_sendTest"));
    ui->bt_sendTest->setToolTip(langVarLogic->getVariable(&locale, "bt_sendTestHint"));
    if (tableLoaded) {
        table->setNameHeader(nameHeader);
        table->setIpHeader(ipHeader);
        table->setCleanHeader(cleanHeader);
        table->setFilterHeader(filterHeader);
        table->setOsHeader(osHeader);
        table->setCFHeader(cfHeader);
        table->setTimeHeader(timeHeader);
        table->setCleanHint(cleanHint);
        table->setFilterHint(filterHint);
        showMessageAction->setText(showMessage);
        minimizeAction->setText(minimize);
        maximizeAction->setText(maximize);
        restoreAction->setText(restore);
        quitAction->setText(quit);
    }
    if (locale == "en")
        ui->actionTabWdg->setTabIcon(1, QIcon(":/images/Dustend19.ico"));
    if (locale == "ru")        
        ui->actionTabWdg->setTabIcon(1, QIcon(":/images/FS8.ico"));
}

void DustendWindow::settingsSaved()
{
    setsSaved = true;
}

void DustendWindow::sendTestEmail()
{
    if (emailAddress != "") {
        QString msg = "";
        QString subj = "";
        if (genSetLogic->getLocale() == "en") {
            msg = "Hi.\nThis is test email from ComputerDustControl. \nHave a nice day!";
            subj = "Test email from ComputerDustControl";
        }
        if (genSetLogic->getLocale() == "ru") {
            msg = "Привет. \nЭто тестовое письмо от ComputerDustControl. \nПриятного дня!";
            subj = "Тестовое письмо от ComputerDustControl";
        }
        mailThr->setEmailAddress(&emailAddress);
        mailThr->setWarningMsg(&msg);
        mailThr->setSubject(&subj);
        mailThr->start();
    }
}

void DustendWindow::setDirtyPlace()
{
    if (!compL->getCleanTimeIsRunning(&localIp)
            && !compL->getFilterTimeIsRunning(&localIp) ) {
        if (ui->cbDustyPlace->isChecked())
            setFactoryComputer();
        else
            setHomeComputer();
    }
    else {
        if (ui->cbDustyPlace->isChecked())
            ui->cbDustyPlace->setChecked(false);
        else
            ui->cbDustyPlace->setChecked(true);
    }
}

void DustendWindow::setCurCompTab(int tabNumber)
{    
    if (tabNumber == 0)
        showComputer();
    else
        if (tabNumber == 1)
            showNetwork();
}

void DustendWindow::setCurActionTab(int tabNumber)
{
    if (!this->signalsBlocked()) {
        if (!compL->getCleanTimeIsRunning(&localIp)
                && !compL->getFilterTimeIsRunning(&localIp)) {
            if (tabNumber == 0)
                showCleaningStatus();
            else
                if (tabNumber == 1)
                    showFilterStatus();
            setBtStart();
        }
        else {
            this->blockSignals(true);
            if (tabNumber == 0)
                ui->actionTabWdg->setCurrentWidget(ui->filterTab);
            else
                if (tabNumber == 1)
                    ui->actionTabWdg->setCurrentWidget(ui->cleanTab);
        }
    }
    else
        this->blockSignals(false);
    setProgressBar();        
}

void DustendWindow::showFanControl()
{
    if (ui->btnInfo->isChecked()
            || ui->btnTimer->isChecked()
            || ui->btnSettings->isChecked()) {
        ui->btnInfo->setChecked(false);
        ui->btnTimer->setChecked(false);
        ui->btnSettings->setChecked(false);
        ui->stackedWidget->setCurrentWidget(ui->fanControlPage);
    }
    ui->btnFanControl->setChecked(true);
}

void DustendWindow::showSettings()
{
    if (ui->btnInfo->isChecked()
            || ui->btnTimer->isChecked()
            || ui->btnFanControl->isChecked()) {
        ui->btnInfo->setChecked(false);
        ui->btnTimer->setChecked(false);
        ui->btnFanControl->setChecked(false);
        ui->stackedWidget->setCurrentWidget(ui->settingsPage);
    }
    ui->btnSettings->setChecked(true);
}

void DustendWindow::showTimer()
{
    if (ui->btnInfo->isChecked()
            || ui->btnFanControl->isChecked()
            || ui->btnSettings->isChecked()) {
        ui->btnInfo->setChecked(false);
        ui->btnFanControl->setChecked(false);
        ui->btnSettings->setChecked(false);
        ui->stackedWidget->setCurrentWidget(ui->timerPage);
    }
    ui->btnTimer->setChecked(true);
}
