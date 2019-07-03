QT       += core widgets gui network sql

TARGET = ComputerDustControl
TEMPLATE = app
QMAKE_TARGET_DESCRIPTION = description


SOURCES += main.cpp\
        dustendwindow.cpp \
    cooler.cpp \
    computer.cpp \
    computerlogic.cpp \
    network.cpp \
    netcomp.cpp \
    mytextedit.cpp \
    tablemodel.cpp \
    progressdelegate.cpp \
    deviceinfo.cpp \
    netcomplogic.cpp \
    email/smtpclient.cpp \
    email/quotedprintable.cpp \
    email/mimetext.cpp \
    email/mimepart.cpp \
    email/mimemultipart.cpp \
    email/mimemessage.cpp \
    email/mimeinlinefile.cpp \
    email/mimehtml.cpp \
    email/mimefile.cpp \
    email/mimecontentformatter.cpp \
    email/mimeattachment.cpp \
    email/emailaddress.cpp \
    DB/QueryObj.cpp \
    DB/MySQLConnection.cpp \
    DB/DataBase.cpp \
    DB/Criteria.cpp \
    DB/AbstractConnection.cpp \
    generalsettingsgateway.cpp \
    generalsettings.cpp \
    generalsettingslogic.cpp \
    DB/newsqltablefield.cpp \
    computergateway.cpp \
    languagevariables.cpp \
    languagevariablesgateway.cpp \
    languagevariableslogic.cpp \
    savecompthread.cpp \
    mailthread.cpp \
    entryexittimes.cpp \
    entryexittimesgateway.cpp

HEADERS  += dustendwindow.h \
    cooler.h \
    computer.h \
    computerlogic.h \
    network.h \
    netcomp.h \
    predicates.h \
    mytextedit.h \
    tablemodel.h \
    progressdelegate.h \
    deviceinfo.h \
    netcomplogic.h \
    email/SmtpMime \
    email/smtpclient.h \
    email/quotedprintable.h \
    email/mimetext.h \
    email/mimepart.h \
    email/mimemultipart.h \
    email/mimemessage.h \
    email/mimeinlinefile.h \
    email/mimehtml.h \
    email/mimefile.h \
    email/mimecontentformatter.h \
    email/mimeattachment.h \
    email/emailaddress.h \
    DB/QueryObj.h \
    DB/MySQLConnection.h \
    DB/DataBase.h \
    DB/Criteria.h \
    DB/AbstractConnection.h \
    generalsettingsgateway.h \
    generalsettings.h \
    generalsettingslogic.h \
    DB/newsqltablefield.h \
    computergateway.h \
    languagevariables.h \
    languagevariablesgateway.h \
    languagevariableslogic.h \
    savecompthread.h \
    mailthread.h \
    entryexittimes.h \
    entryexittimesgateway.h

FORMS    += \
    mainwindow.ui

LIBS += netapi32.lib
LIBS += ws2_32.lib
LIBS += mpr.lib

LIBS += Qt5Network.lib
LIBS += Qt5Sql.lib
#INCLUDEPATH += c:/boost
INCLUDEPATH += d:/boost1480

RESOURCES += \
    rc.qrc
RC_FILE = myicon.rc

CODEC = UTF-8
