#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <datatype.h>

class TableModel;
class QFileDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_chooseSrcToolButton_clicked();

    void on_mainStartBtn_clicked();

    void appendRow(TableRowInfo rowInfo);

private:
    void    initView();
    void    clearTableView();
    void    setViewEnable(bool enable);
    bool    initDeviceTableView(QList<QList<QVariant>> &datas);
    void    initTargetVillageDatas(QList<QList<QVariant>> &datas);
    bool    assertFile(QString path);
    bool    initSrcColumns(QList<QList<QVariant>> &datas);

    TableModel*     mModel;
    QFileDialog*    mFileDialog;
    QString         mSourceFilePath;

    bool mIsDeviceTableInit;

    int mIpTableColumnCity;
    int mIpTableColumnCountry;
    int mIpTableColumnZoningName;
    int mIpTableColumnZongingCode;
    int mIpTableColumnRaiseComIp;
    int mIpTableColumnCapIp;

    const static int mRowIpTableHeader = 0;
    const static int mColumnCity = 0;
    const static int mColumnCountry = 1;
    const static int mColumnZoningName = 2;
    const static int mColumnZoningCode = 3;
    const static int mColumnRaiseComIp = 4;
    const static int mColumnRaiseComStat = 5;
    const static int mColumnCapIp = 6;
    const static int mColumnCapNetworkStat = 7;
    const static int mColumnCapSoftVer = 8;
    const static int mColumnCapTxPower = 9;
    const static int mColumnCapChannel = 10;
    const static int mColumnRemarks = 11;

    QList<VILLAGEINFO>*     mVillageInfos;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
