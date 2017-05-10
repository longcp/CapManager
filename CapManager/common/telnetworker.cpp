#include "telnetworker.h"

#define LOG_TAG                         "TELNET_WORKER"
#include "utils/Log.h"

TelnetWorker::TelnetWorker(QList<VILLAGEINFO> &targetVillageInfoList) :
    mTelnet(new QtTelnet()),
    mVillageList(new QList<VILLAGEINFO>)
{
    mVillageList->swap(targetVillageInfoList);

    connect(mTelnet, SIGNAL(message(const QString &)),
            this, SLOT(telnetReturnMessage(const QString &)));

    connect(mTelnet, SIGNAL(loginRequired()),
            this, SLOT(telnetLoginRequired()));

    connect(mTelnet, SIGNAL(loginFailed()),
            this, SLOT(telnetLoginFailed()));

    connect(mTelnet, SIGNAL(loggedOut()),
            this, SLOT(telnetLoggedOut()));

    connect(mTelnet, SIGNAL(loggedIn()),
            this, SLOT(telnetLoggedIn()));

    connect(mTelnet, SIGNAL(connectionError(QAbstractSocket::SocketError)),
            this, SLOT(telnetConnectionError(QAbstractSocket::SocketError)));
}

TelnetWorker::~TelnetWorker() {}

void
TelnetWorker::stop()
{

}

void
TelnetWorker::run()
{
    ALOGDTRACE();
    for (int i = 0; i < mVillageList->size(); i++) {
        VILLAGEINFO villageInfo = mVillageList->at(i);
        ALOGD("city = %s, country = %s, zoningName = %s, "
              "zoningCode = %s, raisecomIp = %s, cap1.ipAddr = %s, "
              "cap2.ipAddr = %s, cap3.ipAddr = %s, cap4.ipAddr = %s",
              villageInfo.city.toStdString().data(),
              villageInfo.country.toStdString().data(),
              villageInfo.zoningName.toStdString().data(),
              villageInfo.zoningCode.toStdString().data(),
              villageInfo.raisecomIp.toStdString().data(),
              villageInfo.cap1.ipAddr.toStdString().data(),
              villageInfo.cap2.ipAddr.toStdString().data(),
              villageInfo.cap3.ipAddr.toStdString().data(),
              villageInfo.cap4.ipAddr.toStdString().data());
    }
}

void
TelnetWorker::handleServerRequestMsg(const ServerRequest req)
{
    switch (req) {
    case ServerRequest::REQ_RAISECOM_LOGIN_USERNAME:
        mTelnet->sendData("admin");
        break;

    case ServerRequest::REQ_RAISECOM_LOGIN_PASSWORD:
        mTelnet->sendData("!iK9u5!!fUkjKou!9klIUf!23mjJD123r!87k!");
        break;

    case ServerRequest::REQ_CAP_LOGIN_USERNAME:
        mTelnet->sendData("root");
        break;

    default:
        break;
    }
}

ServerRequest
TelnetWorker::parseServerRequestMsg(const QString &msg)
{
    if (msg.contains("Username:"))
        return ServerRequest::REQ_RAISECOM_LOGIN_USERNAME;

    else if (msg.contains("Password:"))
        return ServerRequest::REQ_RAISECOM_LOGIN_PASSWORD;

    else if (msg.contains("nufront login:"))
        return ServerRequest::REQ_CAP_LOGIN_USERNAME;

    else
        return ServerRequest::REQ_UNKNOWN;
}

void
TelnetWorker::telnetReturnMessage(const QString &msg)
{
    handleServerRequestMsg(parseServerRequestMsg(stripCR(msg)));
    //QScrollBar *s = ui->textEdit->verticalScrollBar();
    //s->setValue(s->maximum());
}

void
TelnetWorker::telnetLoginRequired()
{
    ALOGDTRACE();
}

void
TelnetWorker::telnetLoginFailed()
{
    ALOGDTRACE();
}

void
TelnetWorker::telnetLoggedOut()
{
    ALOGDTRACE();
}

void
TelnetWorker::telnetLoggedIn()
{
    ALOGDTRACE();
}

void
TelnetWorker::telnetConnectionError(QAbstractSocket::SocketError error)
{
//    ui->lineEdit_status->setText(QString("Connection error: %1").arg(error));
    ALOGE("%s, error[%s]", __FUNCTION__,
          QString("Connection error: %1").arg(error).toStdString().data());
}

QString
TelnetWorker::stripCR(const QString &msg)
{
    QString nmsg(msg);
    nmsg.remove('\r');
    nmsg.remove(QRegExp("\033\\[[0-9;]*[A-Za-z]")); // Also remove terminal control codes
    return nmsg;
}

void
TelnetWorker::setTargetDeviceList(QList<VILLAGEINFO> &villageList)
{
    mVillageList->clear();
    mVillageList->swap(villageList);
}
