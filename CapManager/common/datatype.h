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
    bool isAccessed;
    QString oldSoftVer;
    QString curSoftVer;
    QString txPower;
} CAPINFO;

typedef struct CountryInfo {
    QString ipAddr;
    QString zoningCode;
    bool isRaiseComAccessed;
    CAPINFO cap1;
    CAPINFO cap2;
    CAPINFO cap3;
} COUNTRYINFO;

#endif // DATATYPE_H
