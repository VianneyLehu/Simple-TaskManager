// Include necessary headers
#include "mainwindow.h"
#include <vector>
#include "process.h"
#include "pdh.h"
#include "cpu.h"

#define KB 1024
#define MB (KB*1024)
#define GB (MB*1024)
#define TB (GB *1024)


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{

    resize(1000,800);
    // Set up main window and tabs
    createMainWindow();

    addCpuGraphToPerformanceTab();

    addProcessInfoToProcessusTab();

}



void MainWindow::setupCpuGraph()
{
    chart = new QChart();
    chart->legend()->hide();
    chart->setTitle("CPU Usage Over Time");

    series = new QLineSeries();
    chart->addSeries(series);

    axisX = new QValueAxis;
    axisX->setRange(0, 60); // Assuming 60 seconds
    axisX->setLabelFormat("%i");
    axisX->setTitleText("Time (s)");

    axisY = new QValueAxis;
    axisY->setRange(0, 100); // CPU usage in percentage
    axisY->setLabelFormat("%i");
    axisY->setTitleText("CPU Usage (%)");


    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    chart->addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *performanceLayout = new QVBoxLayout(performanceTab);
    performanceLayout->addWidget(chartView);

}


void MainWindow::updateCpuGraph()
{

    double cpuUsage = getCurrentValue();

    count ++;

    if(count >= 60){
        series->remove(0);
        axisX->setRange(count - 59, count);
    }

    if (cpuUsage != -1.0) {
        series->append(count, cpuUsage);
        ++count;
        axisX->setMax(count > 60 ? count : 60); // Adjust maximum X value if needed
    } else {
        std::cerr << "Failed to retrieve CPU usage." << std::endl;
    }

}



void MainWindow::createMainWindow()
{
    // Create and set up tab widget
    tabWidget = new QTabWidget(this);

    // Set the layout for the main window
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);

    // Set the main layout for the main window
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Create "Processus" tab
    processusTab = new QWidget(this);
    tabWidget->addTab(processusTab, "Processus");

    // Create "Performance" tab
    performanceTab = new QWidget(this);
    tabWidget->addTab(performanceTab, "Performance");

    // Create "Details" tab (currently empty)
    detailsTab = new QWidget(this);
    tabWidget->addTab(detailsTab, "Details");
}



void MainWindow::addCpuGraphToPerformanceTab()
{

    setupCpuGraph();

    init();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCpuGraph()));
    timer->start(1000); // 1000 milliseconds = 1 second


}



void MainWindow::addProcessInfoToProcessusTab()
{
    // Create table widget for process info
    tableWidget = new QTableWidget(processusTab);
    tableWidget->setColumnCount(3);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "PID" << "Name" << "Memory Usage (MB)");
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Add table widget to the "Processus" tab
    QVBoxLayout *processusLayout = new QVBoxLayout(processusTab);
    processusLayout->addWidget(tableWidget);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateProcessInfo()));
    timer->start(1000); // 1000 milliseconds = 1 second

    // Initial population of the table
    updateProcessInfo();
}


void MainWindow::updateProcessInfo() {
    // Clear the previous process info
    processInfoList.clear();

    // Generate process information
    GenerateProcessInfo(processInfoList);

    // Populate the table with updated process info
    populateTable();
}

void MainWindow::populateTable() {
    // Clear existing table contents
    tableWidget->clearContents();
    tableWidget->setRowCount(0);

    // Populate the table with updated process info
    tableWidget->setRowCount(processInfoList.size());
    for (int i = 0; i < processInfoList.size(); ++i) {
        QTableWidgetItem *pidItem = new QTableWidgetItem(QString::number(processInfoList[i].GetPID()));
        QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString(processInfoList[i].GetName()));
        QTableWidgetItem *memoryItem = new QTableWidgetItem(QString::number(processInfoList[i].GetMemoryUsageMB()));

        tableWidget->setItem(i, 0, pidItem);
        tableWidget->setItem(i, 1, nameItem);
        tableWidget->setItem(i, 2, memoryItem);
    }
}

MainWindow::~MainWindow()
{

    delete tableWidget;
    delete timer;
}
