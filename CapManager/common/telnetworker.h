#ifndef TELNETWORKER_H
#define TELNETWORKER_H

#include <QObject>
#include <QThread>
#include <qttelnet.h>
#include <datatype.h>

class QtTelnet;

class TelnetWorker : public QThread
{
public:
    TelnetWorker(QList<QList<QVariant>> &targetDevList);
    ~TelnetWorker();

    void run();
    void stop();

private slots:
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
    void        setTargetDeviceList(QList<QList<QVariant>> &devList);

    QtTelnet* mTelnet;
    QList<QList<QVariant>>*     mDeviceList;
};

#endif // TELNETWORKER_H
