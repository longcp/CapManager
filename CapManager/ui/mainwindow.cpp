#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <datatype.h>
#include <tablemodel.h>
#include <QFileDialog>
#include <QMessageBox>
#include <exceloperate.h>
#include <QStandardItem>
#include <QDebug>
#include <telnetworker.h>

#define LOG_TAG                         "MAIN_WINDOW"
#include <utils/Log.h>

#define HEADER_STR_CITY                 "市"
#define HEADER_STR_COUNTRY              "市县"
#define HEADER_STR_ZONING_NAME          "区划名称"
#define HEADER_STR_ZONING_CODE          "区划代码"
#define HEADER_STR_RAISECOM_IP          "村路由器IP地址"
#define HEADER_STR_CAP_IP               "中心设备IP地址"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mSourceFilePath(""),
    mIsDeviceTableInit(false),
    mFileDialog(new QFileDialog()),
    mVillageInfos(new QList<VILLAGEINFO>),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("CapManager");
    initView();
    setWindowState(Qt::WindowMaximized);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::initView()
{
    QStringList headerList;
    headerList << "序号" << "市" << "县" << "区划名称" << "区划代码"
               << "村路由器IP地址" << "村路由器状态" << "基站IP地址"
               << "基站网络状态" << "基站版本" << "发射功率"
               << "工作频点" << "备注";
    mModel = new TableModel(0, headerList.size());
    ui->deviceInfoTableView->setModel(mModel);
    mModel->setHorizontalHeaderLabels(headerList);
    // 行宽
//    ui->deviceInfoTableView->setColumnWidth(1, 300);
    ui->deviceInfoTableView->setColumnWidth(3, 250);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->deviceInfoTableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->deviceInfoTableView->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->deviceInfoTableView->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->deviceInfoTableView->horizontalHeader()
            ->setSectionResizeMode(headerList.size()-1, QHeaderView::Stretch);

    ui->deviceInfoTableView->verticalHeader()->setVisible(false);       //隐藏行表头
    ui->deviceInfoTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->deviceInfoTableView->setAlternatingRowColors(true);
    ui->deviceInfoTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

}

void
MainWindow::clearTableView()
{
    if (mModel->rowCount() > 0)
        mModel->removeRows(0, mModel->rowCount());
}

void
MainWindow::setViewEnable(bool enable)
{
    ui->chooseSrcToolButton->setEnabled(enable);
    ui->mainStartBtn->setEnabled(enable);
}

bool
MainWindow::initDeviceTableView(QList<QList<QVariant>> &datas)
{
    ALOGDTRACE();

    if (!initSrcColumns(datas)) {
        mIsDeviceTableInit = false;
        return false;
    }
    mIsDeviceTableInit = true;

    int size = datas.size();
    for (int i = mRowIpTableHeader+1; i < size; i++) {
        TableRowInfo rowInfo;
        rowInfo.number = i-1;
        rowInfo.city = datas.at(i).at(mIpTableColumnCity).toString();
        rowInfo.country = datas.at(i).at(mIpTableColumnCountry).toString();
        rowInfo.zoningName = datas.at(i).at(mIpTableColumnZoningName).toString();
        rowInfo.zoningCode = datas.at(i).at(mIpTableColumnZongingCode).toString();
        rowInfo.raisecomIp = datas.at(i).at(mIpTableColumnRaiseComIp).toString();
        rowInfo.capIp = datas.at(i).at(mIpTableColumnCapIp).toString();
        rowInfo.capNetworkStat = "";
        rowInfo.capSoftVer = "";
        rowInfo.capTxPower = "";
        rowInfo.capChannel = "";
        rowInfo.remarks = "";

        if (rowInfo.city == "" || rowInfo.country == "")
            continue;

        appendRow(rowInfo);
    }

    return true;
}

void
MainWindow::initTargetVillageDatas(QList<QList<QVariant> > &datas)
{
    if (datas.isEmpty())
        return;

    mVillageInfos->clear();
    VILLAGEINFO villageInfo;

    int size = datas.size();
    for (int i = mRowIpTableHeader+1; i < size; i++) {
        villageInfo.city = datas.at(i).at(mIpTableColumnCity).toString();
        villageInfo.country = datas.at(i).at(mIpTableColumnCountry).toString();
        villageInfo.zoningName = datas.at(i).at(mIpTableColumnZoningName).toString();
        villageInfo.zoningCode = datas.at(i).at(mIpTableColumnZongingCode).toString();
        villageInfo.raisecomIp = datas.at(i).at(mIpTableColumnRaiseComIp).toString();
        villageInfo.cap1.ipAddr = datas.at(i).at(mIpTableColumnCapIp).toString();
        villageInfo.cap2.ipAddr = datas.at(++i).at(mIpTableColumnCapIp).toString();
        villageInfo.cap3.ipAddr = datas.at(++i).at(mIpTableColumnCapIp).toString();
        villageInfo.cap4.ipAddr = datas.at(++i).at(mIpTableColumnCapIp).toString();

        if (villageInfo.city == "" || villageInfo.country == "")
            continue;

        mVillageInfos->push_back(villageInfo);
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
MainWindow::appendRow(TableRowInfo rowInfo)
{
    QStandardItem *num = new QStandardItem(QString::number(rowInfo.number));
    QStandardItem *city = new QStandardItem(rowInfo.city);
    QStandardItem *country = new QStandardItem(rowInfo.country);
    QStandardItem *zoningName = new QStandardItem(rowInfo.zoningName);
    QStandardItem *zoningCode = new QStandardItem(rowInfo.zoningCode);
    QStandardItem *raisecomIp = new QStandardItem(rowInfo.raisecomIp);
    QStandardItem *raisecomStat = new QStandardItem(rowInfo.raisecomStat);
    QStandardItem *capIp = new QStandardItem(rowInfo.capIp);
    QStandardItem *capNetworkStat = new QStandardItem(rowInfo.capNetworkStat);
    QStandardItem *capSoftVer = new QStandardItem(rowInfo.capSoftVer);
    QStandardItem *capTxPower = new QStandardItem(rowInfo.capTxPower);
    QStandardItem *capChannel = new QStandardItem(rowInfo.capChannel);
    QStandardItem *remarks = new QStandardItem(rowInfo.remarks);

    QList<QStandardItem*> items;
    items << num << city << country << zoningName << zoningCode
          << raisecomIp << raisecomStat << capIp << capNetworkStat << capSoftVer
          << capTxPower << capChannel << remarks;

    mModel->appendRow(items);
}

bool
MainWindow::assertFile(QString path)
{
    if (!QFile::exists(path)) {
        QString errLog;
        errLog = "文件[" + path + "] 不存在！";
        QMessageBox::critical(this,
                              tr("出错"),
                              errLog,
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return false;
    }

    return true;
}

bool
MainWindow::initSrcColumns(QList<QList<QVariant>> &datas)
{
    mIpTableColumnCity =
            datas.at(mRowIpTableHeader).indexOf(HEADER_STR_CITY);
    mIpTableColumnCountry =
            datas.at(mRowIpTableHeader).indexOf(HEADER_STR_COUNTRY);
    mIpTableColumnZoningName =
            datas.at(mRowIpTableHeader).indexOf(HEADER_STR_ZONING_NAME);
    mIpTableColumnZongingCode =
            datas.at(mRowIpTableHeader).indexOf(HEADER_STR_ZONING_CODE);
    mIpTableColumnRaiseComIp =
            datas.at(mRowIpTableHeader).indexOf(HEADER_STR_RAISECOM_IP);
    mIpTableColumnCapIp =
            datas.at(mRowIpTableHeader).indexOf(HEADER_STR_CAP_IP);
    if (mIpTableColumnCity < 0 ||
            mIpTableColumnCountry < 0 ||
            mIpTableColumnZoningName < 0 ||
            mIpTableColumnZongingCode < 0 ||
            mIpTableColumnRaiseComIp < 0 ||
            mIpTableColumnCapIp < 0) {
        return false;
    }

    qDebug() << "mIpTableColumnCity = " << mIpTableColumnCity << ", "
             << "mIpTableColumnCountry = " << mIpTableColumnCountry << ", "
             << "mIpTableColumnZoningName = " << mIpTableColumnZoningName << ", "
             << "mIpTableColumnZongingCode = " << mIpTableColumnZongingCode << ", "
             << "mIpTableColumnRaiseComIp = " << mIpTableColumnRaiseComIp
             << "mIpTableColumnCapIp = " << mIpTableColumnCapIp;

    return true;
}

void
MainWindow::on_chooseSrcToolButton_clicked()
{
    int ret;
    QList<QList<QVariant>> srcVillageDatas;
    mFileDialog->setWindowTitle(tr("导入IP信息表"));
    mFileDialog->setDirectory(".");                                     //默认打开目录为当前目录
    mFileDialog->setFileMode(QFileDialog::ExistingFile);                //选择存在的文件
    mFileDialog->setNameFilter(QString("所有excel文件(*.xls *.xlsx);"));

    if (mFileDialog->exec() == QDialog::Accepted) {
        qDebug("on_chooseSrcToolButton_clicked selected");
        mSourceFilePath = mFileDialog->selectedFiles()[0];
        ui->srcPathLineEdit->setText(mSourceFilePath);
        if (!assertFile(mSourceFilePath))
            return;

        ret = ExcelOperate::readSheet(mSourceFilePath, 1, srcVillageDatas);
        ALOGD("ret = %d\n", ret);
        initDeviceTableView(srcVillageDatas);
        initTargetVillageDatas(srcVillageDatas);
    }
}

void
MainWindow::on_mainStartBtn_clicked()
{
    if (!mIsDeviceTableInit)
        return;

    mTelnetWorker = new TelnetWorker(*mVillageInfos);
    mTelnetWorker->start();

    mIsTelnetThreadWorking = true;
    setViewEnable(false);
}
