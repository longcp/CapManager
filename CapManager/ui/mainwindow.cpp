#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <datatype.h>
#include <tablemodel.h>

#define LOG_TAG                         "MAIN_WINDOW"
#include <utils/Log.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("CapManager");
    initView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::initView()
{
    QStringList headerList;
    headerList << "市" << "县" << "区划名称" << "区划代码"
               << "村路由器IP地址" << "村路由器状态" << "基站IP地址"
               << "基站网络状态" << "基站版本" << "发射功率"
               << "工作频点" << "备注";
    mModel = new TableModel(0, headerList.size());
    ui->deviceInfoTableView->setModel(mModel);
    mModel->setHorizontalHeaderLabels(headerList);
    // 行宽
//    ui->deviceInfoTableView->setColumnWidth(1, 300);
//    ui->deviceInfoTableView->setColumnWidth(3, 300);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->deviceInfoTableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->deviceInfoTableView->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->deviceInfoTableView->setSelectionMode(
                QAbstractItemView::SingleSelection);

//    ui->deviceInfoTableView->horizontalHeader()
//            ->setSectionResizeMode(3, QHeaderView::Stretch);

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
