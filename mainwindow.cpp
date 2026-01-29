#include "MainWindow.h"
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    initializeData();
    setupUI();
    applyTechStyles();
    updateDashboard();
    logToTerminal("DISPATCH ENGINE v2.0.4 ONLINE", "SYSTEM");
}

MainWindow::~MainWindow() {
    delete engine;
    delete city;
}

void MainWindow::initializeData() {
    city = new City();
    for(int i=0; i<=5; i++) city->addLocation(i);
    city->addRoad(0, 1, 5); city->addRoad(1, 3, 3);
    city->addRoad(3, 4, 4); city->addRoad(4, 5, 6);
    city->addRoad(0, 2, 10); city->addRoad(2, 3, 7);

    engine = new DispatchEngine(city);
    engine->registerDriver(new Driver(101, 0, 1));
    engine->registerDriver(new Driver(102, 2, 2));
    engine->registerDriver(new Driver(103, 4, 3));
}

void MainWindow::setupUI() {
    auto *central = new QWidget();
    auto *layout = new QVBoxLayout(central);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    // --- TOP HEADER: STATS ---
    auto *header = new QHBoxLayout();
    QLabel *logo = new QLabel("CORE_DISPATCH //");
    logo->setStyleSheet("font: bold 22px 'Consolas'; color: #00f2ff; letter-spacing: 2px;");

    driverCountLabel = new QLabel("DRIVERS: 03");
    tripCountLabel = new QLabel("ACTIVE_TRIPS: 00");

    header->addWidget(logo);
    header->addStretch();
    header->addWidget(driverCountLabel);
    header->addSpacing(30);
    header->addWidget(tripCountLabel);
    layout->addLayout(header);

    // --- MAIN CONTENT ---
    auto *content = new QHBoxLayout();

    // Left: Control Panel
    auto *ctrlLayout = new QVBoxLayout();
    pickupInput = new QLineEdit();
    pickupInput->setPlaceholderText(">> SRC_NODE");
    dropoffInput = new QLineEdit();
    dropoffInput->setPlaceholderText(">> DEST_NODE");

    auto *btn = new QPushButton("EXECUTE_REQUEST");
    connect(btn, &QPushButton::clicked, this, &MainWindow::handleRequestRide);

    ctrlLayout->addWidget(new QLabel("INPUT_PARAMETERS"));
    ctrlLayout->addWidget(pickupInput);
    ctrlLayout->addWidget(dropoffInput);
    ctrlLayout->addSpacing(10);
    ctrlLayout->addWidget(btn);
    ctrlLayout->addStretch();
    content->addWidget(createGlassPanel("COMMAND_MODULE", ctrlLayout), 1);

    // Center: Live Data Table
    auto *tableLayout = new QVBoxLayout();
    mainTable = new QTableWidget(0, 4);
    mainTable->setHorizontalHeaderLabels({"TRIP_ID", "NODE_A", "NODE_B", "STATUS"});
    mainTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mainTable->verticalHeader()->setVisible(false);
    tableLayout->addWidget(mainTable);
    content->addWidget(createGlassPanel("LIVE_DATA_FEED", tableLayout), 2);

    // Right: Terminal Log
    auto *logLayout = new QVBoxLayout();
    terminalLog = new QPlainTextEdit();
    terminalLog->setReadOnly(true);
    logLayout->addWidget(terminalLog);
    content->addWidget(createGlassPanel("SYSTEM_LOG", logLayout), 1);

    layout->addLayout(content);
    setCentralWidget(central);
    resize(1100, 700);
}

QFrame* MainWindow::createGlassPanel(QString title, QLayout* contentLayout) {
    auto *frame = new QFrame();
    frame->setObjectName("panel");
    auto *l = new QVBoxLayout(frame);
    auto *t = new QLabel(title);
    t->setStyleSheet("color: #465161; font: bold 10px;");
    l->addWidget(t);
    l->addLayout(contentLayout);
    return frame;
}

void MainWindow::applyTechStyles() {
    this->setStyleSheet(R"(
        QMainWindow { background-color: #0a0e14; }

        QLabel { color: #00f2ff; font-family: 'Consolas'; }

        #panel {
            background-color: #11151c;
            border: 1px solid #1c2431;
            border-radius: 4px;
        }

        QLineEdit {
            background-color: #0a0e14;
            border: 1px solid #00f2ff;
            color: #00f2ff;
            padding: 10px;
            font-family: 'Consolas';
            border-radius: 2px;
        }

        QPushButton {
            background-color: #00f2ff;
            color: #0a0e14;
            border-radius: 2px;
            padding: 12px;
            font-weight: bold;
            font-family: 'Consolas';
        }
        QPushButton:hover { background-color: #ffffff; }

        QTableWidget {
            background-color: transparent;
            border: none;
            gridline-color: #1c2431;
            color: #e0e0e0;
            font-family: 'Consolas';
        }

        QHeaderView::section {
            background-color: #161b22;
            color: #465161;
            border: 1px solid #0a0e14;
            padding: 5px;
        }

        QPlainTextEdit {
            background-color: #05070a;
            border: none;
            color: #00f2ff;
            font-family: 'Consolas';
            font-size: 11px;
        }
    )");
}

void MainWindow::handleRequestRide() {
    int p = pickupInput->text().toInt();
    int d = dropoffInput->text().toInt();

    logToTerminal(QString("INITIATING PATH_FIND %1 -> %2").arg(p).arg(d), "CALC");

    Rider rider(riderIdCounter++, p, d);
    Trip* trip = engine->requestTrip(rider);

    if (trip) {
        logToTerminal(QString("TRIP_%1 AUTH_SUCCESS").arg(trip->getId()), "OK");
        logToTerminal(QString("> ASSIGNED_DRIVER: %1").arg(trip->getDriverId()), "DISPATCH");
    } else {
        logToTerminal("PATHFINDING_ERROR: DESTINATION_UNREACHABLE", "CRITICAL");
    }
    updateDashboard();
}

void MainWindow::updateDashboard() {
    tripCountLabel->setText(QString("ACTIVE_TRIPS: %1").arg(engine->getActiveTripCount()));

    mainTable->setRowCount(0);
    for(int i=1000; i<1020; i++) {
        Trip* t = engine->findTripById(i);
        if(!t) continue;
        int row = mainTable->rowCount();
        mainTable->insertRow(row);
        mainTable->setItem(row, 0, new QTableWidgetItem(QString::number(t->getId())));
        mainTable->setItem(row, 1, new QTableWidgetItem(QString::number(t->getPickupLocation())));
        mainTable->setItem(row, 2, new QTableWidgetItem(QString::number(t->getDropoffLocation())));

        auto *item = new QTableWidgetItem(Trip::stateToString(t->getState()));
        item->setForeground(QBrush(QColor("#00f2ff")));
        mainTable->setItem(row, 3, item);
    }
}

void MainWindow::logToTerminal(QString msg, QString type) {
    QString timestamp = QTime::currentTime().toString("hh:mm:ss");
    terminalLog->appendPlainText(QString("[%1] %2 :: %3").arg(timestamp).arg(type).arg(msg));
}
