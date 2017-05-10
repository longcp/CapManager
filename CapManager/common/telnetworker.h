#ifndef TELNETWORKER_H
#define TELNETWORKER_H

#include <QObject>
#include <QThread>
#include <qttelnet.h>
#include <datatype.h>

class TelnetWorker : public QThread
{
    Q_OBJECT
public:
    TelnetWorker(QList<VILLAGEINFO> &targetVillageInfoList);
    ~TelnetWorker();

    void run();
    void stop();

    QtTelnet* mTelnet;

public slots:
    void telnetReturnMessage(const QString &msg);
    void telnetLoginRequired();
    void telnetLoginFailed();
    void telnetLoggedOut();
    void telnetLoggedIn();
    void telnetConnectionError(QAbstractSocket::SocketError error);

private:
    QString     stripCR(const QString &msg);
    void        handleServerRequestMsg(const ServerRequest req);
    ServerRequest parseServerRequestMsg(const QString &msg);
    void        setTargetDeviceList(QList<VILLAGEINFO> &devList);

    QList<VILLAGEINFO>*     mVillageList;
};

#endif // TELNETWORKER_H
