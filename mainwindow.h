#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QPlainTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTime>
#include <QFrame>
#include "DispatchEngine.h"
#include "Citydj.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleRequestRide();
    void updateDashboard();

private:
    City* city;
    DispatchEngine* engine;
    int riderIdCounter = 500;

    // UI Components
    QLineEdit *pickupInput;
    QLineEdit *dropoffInput;
    QTableWidget *mainTable;
    QPlainTextEdit *terminalLog;
    QLabel *driverCountLabel;
    QLabel *tripCountLabel;

    void setupUI();
    void applyTechStyles();
    void logToTerminal(QString msg, QString type = "INFO");
    void initializeData();
    QFrame* createGlassPanel(QString title, QLayout* layout);
};

#endif
