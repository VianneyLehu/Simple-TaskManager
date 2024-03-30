// Include necessary headers
#include "mainwindow.h"

#define KB 1024
#define MB (KB*1024)
#define GB (MB*1024)
#define TB (GB *1024)


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{

    resize(1000,800);
    // Set up main window and tabs
    createMainWindow();

    addDetailsTab();

    addCpuGraphToPerformanceTab();

    addProcessInfoToProcessusTab();

}

// ---------- CPU TAB -------------


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

// ---------- PROCESS TAB -------------


void MainWindow::addProcessInfoToProcessusTab()
{
    // Create table widget for process info
    processtableWidget = new QTableWidget(processusTab);
    processtableWidget->setColumnCount(3);
    processtableWidget->setHorizontalHeaderLabels(QStringList() << "PID" << "Name" << "Memory Usage (MB)");
    processtableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Add table widget to the "Processus" tab
    QVBoxLayout *processusLayout = new QVBoxLayout(processusTab);
    processusLayout->addWidget(processtableWidget);

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
    processtableWidget->clearContents();
    processtableWidget->setRowCount(0);

    // Populate the table with updated process info
    processtableWidget->setRowCount(processInfoList.size());
    for (int i = 0; i < processInfoList.size(); ++i) {
        QTableWidgetItem *pidItem = new QTableWidgetItem(QString::number(processInfoList[i].GetPID()));
        QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString(processInfoList[i].GetName()));
        QTableWidgetItem *memoryItem = new QTableWidgetItem(QString::number(processInfoList[i].GetMemoryUsageMB()));

        processtableWidget->setItem(i, 0, pidItem);
        processtableWidget->setItem(i, 1, nameItem);
        processtableWidget->setItem(i, 2, memoryItem);
    }
}




// ---------- DETAILS TAB -------------


void MainWindow::initializeDetailsTab() {
    // Create table widget for process info
    detailstableWidget = new QTableWidget(detailsTab);
    detailstableWidget->setColumnCount(4); // Added 4 columns for PID, Name, VirtualSize, WorkingSetSize
    detailstableWidget->setHorizontalHeaderLabels(QStringList() << "PID" << "Name" << "VirtualSize" << "WorkingSetSize");
    detailstableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Add table widget to the "Details" tab
    QVBoxLayout *detailsLayout = new QVBoxLayout(detailsTab);
    detailsLayout->addWidget(detailstableWidget);
}


void MainWindow::addDetailsTab()
{
    // Initialize the details tab
    initializeDetailsTab();

    // Start a timer to update details periodically
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateDetails()));
    timer->start(1000); // 1000 milliseconds = 1 second

    // Initial population of the table
    updateDetails();
}

void MainWindow::updateDetails() {
    // Clear the previous details
    detailstableWidget->clearContents();
    detailstableWidget->setRowCount(0);

    // Generate details information
    generatePsystemInfo();
    // Populate the table with updated details
}

std::wstring MainWindow::convertToSize(SIZE_T size) {
    WCHAR buffer[MAX_PATH];
    if (StrFormatByteSizeW(size, buffer, MAX_PATH) == NULL) {
        return L"";
    }
    return std::wstring(buffer);
}

int MainWindow::generatePsystemInfo(){
    DWORD dwRet;
    DWORD dwSize = 0x0;
    NTSTATUS dwstatus;
    PSYSTEM_PROCESS_INFORMATION p = nullptr;

    while (true) {
        if (p != nullptr) VirtualFree(p, 0x0, MEM_RELEASE);
        p = (PSYSTEM_PROCESS_INFORMATION)VirtualAlloc(nullptr, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        dwstatus = NtQuerySystemInformation(SystemProcessInformation, (PVOID)p, (ULONG)dwSize, &dwRet);

        if (dwstatus == STATUS_SUCCESS) break;
        else if (dwstatus != STATUS_INFO_LENGTH_MISMATCH) {
            VirtualFree(p, 0x0, MEM_RELEASE);
            p = nullptr;
            std::cerr << "[!] Error fetching process information" << std::endl;
            return 0x1;
        }
        dwSize = dwRet + (2 << 12);
    }

    do {
        int row = detailstableWidget->rowCount();
        detailstableWidget->insertRow(row);

        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(HandleToULong(p->UniqueProcessId)));
        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromWCharArray(p->ImageName.Buffer ? p->ImageName.Buffer : L""));
        QTableWidgetItem* virtualSizeItem = new QTableWidgetItem(QString::fromStdWString(convertToSize(p->VirtualMemoryCounters.VirtualSize)));
        QTableWidgetItem* workingSetSizeItem = new QTableWidgetItem(QString::fromStdWString(convertToSize(p->VirtualMemoryCounters.WorkingSetSize)));

        detailstableWidget->setItem(row, 0, idItem);
        detailstableWidget->setItem(row, 1, nameItem);
        detailstableWidget->setItem(row, 2, virtualSizeItem);
        detailstableWidget->setItem(row, 3, workingSetSizeItem);

        p = (PSYSTEM_PROCESS_INFORMATION)((PBYTE)p + p->NextEntryOffset);
    } while (p->NextEntryOffset != 0);

    VirtualFree(p, 0x0, MEM_RELEASE);

    return 0x0;
}



MainWindow::~MainWindow()
{

    delete detailstableWidget;
    delete processtableWidget;
    delete timer;
}
