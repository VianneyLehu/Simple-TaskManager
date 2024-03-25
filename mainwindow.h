#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QTimer>
#include "process.h" // Include your process.h file

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateProcessInfo();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTableWidget *tableWidget;
    std::vector<ProcessInfo> processInfoList;

    void populateTable();
};

#endif // MAINWINDOW_H
