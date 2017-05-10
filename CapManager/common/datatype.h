#ifndef DATATYPE_H
#define DATATYPE_H

#include <QObject>

enum ServerRequest {
    REQ_RAISECOM_LOGIN_USERNAME,
    REQ_RAISECOM_LOGIN_PASSWORD,
    REQ_CAP_LOGIN_USERNAME,
    REQ_CAP_LOGIN_PASSWORD,
    REQ_UNKNOWN
};

typedef struct CapInfo {
    QString ipAddr;
    bool isAccessed;
    QString oldSoftVer;
    QString curSoftVer;
    QString txPower;
    QString channel;
} CAPINFO;

typedef struct VillageInfo {
    QString city;
    QString country;
    QString zoningName;
    QString zoningCode;
    QString raisecomIp;
    bool isRaiseComAccessed;
    CAPINFO cap1;
    CAPINFO cap2;
    CAPINFO cap3;
    CAPINFO cap4;
} VILLAGEINFO;

typedef struct TableRowInfo {
    int number;
    QString city;
    QString country;
    QString zoningName;
    QString zoningCode;
    QString raisecomIp;
    QString raisecomStat;
    QString capIp;
    QString capNetworkStat;
    QString capSoftVer;
    QString capTxPower;
    QString capChannel;
    QString remarks;
} TABLEROWINFO;

#endif // DATATYPE_H
