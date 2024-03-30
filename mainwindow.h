#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QChartView>
#include <QTimer>
#include <vector>
#include "process.h"
#include "pdh.h"
#include "cpu.h"
#include "pch.h"

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
    void updateDetails();


private:
    void setupCpuGraph();
    void createMainWindow();
    void addCpuGraphToPerformanceTab();
    void addProcessInfoToProcessusTab();
    void populateTable();
    void displayPsystem(PSYSTEM_PROCESS_INFORMATION p);
    std::wstring convertToSize(SIZE_T size);
    int generatePsystemInfo();
    void initializeDetailsTab();
    void addDetailsTab();







    QTabWidget *tabWidget;
    QWidget *processusTab;
    QWidget *performanceTab;
    QWidget *detailsTab;
    QTableWidget *processtableWidget;
    QTableWidget *detailstableWidget;
    QTimer *timer;
    std::vector<ProcessInfo> processInfoList;
    QChart *chart;
    QLineSeries *series;
    QValueAxis *axisX;
    QValueAxis *axisY;
    int count = 0;
};


#endif // MAINWINDOW_H
