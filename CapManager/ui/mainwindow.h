#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TableModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void    initView();
    void    clearTableView();
    void    setViewEnable(bool enable);

    TableModel*     mModel;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
