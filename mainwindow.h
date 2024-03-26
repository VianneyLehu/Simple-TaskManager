#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QChartView>
#include <QTimer>
#include "process.h"


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
    void updateCpuGraph();


private:
    void setupCpuGraph();
    void createMainWindow();
    void addCpuGraphToPerformanceTab();
    void addProcessInfoToProcessusTab();
    void populateTable();

    QTabWidget *tabWidget;
    QWidget *processusTab;
    QWidget *performanceTab;
    QWidget *detailsTab;
    QTableWidget *tableWidget;
    QTimer *timer;
    std::vector<ProcessInfo> processInfoList;
    QChart *chart;
    QLineSeries *series;
    QValueAxis *axisX;
    QValueAxis *axisY;
    int count = 0;
};


#endif // MAINWINDOW_H
