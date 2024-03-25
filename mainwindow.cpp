#include "mainwindow.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout> // Include QVBoxLayout header
#include <QTimer>
#include <vector>
#include "process.h" // Include your process.h file

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    resize(1000, 800);
    // Create a central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Create a layout for the central widget
    QVBoxLayout *layout = new QVBoxLayout(centralWidget); // Include QVBoxLayout

    // Create table widget
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(3);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "PID" << "Name" << "Memory Usage (MB)");
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(tableWidget);

    // Create a timer to update process info every second
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
