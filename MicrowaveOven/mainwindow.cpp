#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize:
    currentTimer = new QTimer(this);
    countTimer = new QTimer(this);
    mac = new QStateMachine();

    // States:
    stateWorking = new QState();
    stateStop = new QState();
    stateCooking = new QState(stateWorking); //count down
    stateIdle = new QState(stateWorking); // display current time
    stateDefrost = new QState(stateWorking);

    // StateMachine:
    mac->addState(stateWorking);
    mac->addState(stateStop);
    mac->setInitialState(stateWorking);
    stateWorking->setInitialState(stateIdle);

    // InitialState Idle:
    countTimer->stop();
    currentTimer->start(1000);
    connect(currentTimer, SIGNAL(timeout()), ui->lcdClock, SLOT(showCurrentTime()));
    connect(countTimer, SIGNAL(timeout()), this, SLOT(tik_tok()));
    connect(ui->dial, SIGNAL(valueChanged(int)), ui->dial, SLOT(setValue(int)));
    connect(ui->dial, SIGNAL(valueChanged(int)), this, SLOT(showDial()));
    connect(ui->dial, SIGNAL(valueChanged(int)), ui->lcdClock, SLOT(showCount(int)));

    // Transitions:
    addTrans(stateIdle, stateCooking, ui->btnStart, SIGNAL(clicked()), this, SLOT(slotIdle2Cooking()));
    addTrans(stateCooking, stateIdle, this, SIGNAL(FinishedCooking()), this, SLOT(slotCooking2Idle()));
    addTrans(stateCooking, stateCooking, ui->btnStart, SIGNAL(clicked()), this, SLOT(slotCooking2Cooking()));
    addTrans(stateWorking, stateStop, ui->btnStop, SIGNAL(clicked()), this, SLOT(slotWorking2Stop()));
    stateStop->addTransition(stateWorking);
    //addTrans(stateStop, stateIdle, this, SIGNAL(StopToIdle()), this, SLOT(slotStop2Idle()));
    addTrans(stateIdle, stateDefrost, ui->btnDefrost, SIGNAL(clicked()), this, SLOT(slotIdle2Defrost()));
    addTrans(stateDefrost, stateCooking, ui->btnStart, SIGNAL(clicked()), this, SLOT(slotDefrost2Cooking()));
    // new:

    mac->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::slotDefrost2Cooking() {
    qDebug() << "slotDefrost2Cooking" << endl;
    countTimer->start(1000);
}

void MainWindow::slotIdle2Cooking() {
    qDebug() << "slotStart2Cooking" << endl;
    m_secondCount = SIXTYSECONDS;
    if(currentTimer->isActive())
        currentTimer->stop();
    countTimer->start(1000);
}

void MainWindow::slotIdle2Defrost() {
    qDebug() << "slotIdle2Defrost" << endl;
    if(currentTimer->isActive())
        currentTimer->stop();
    m_secondCount = getRandom(30, 120);
    ui->lcdClock->showRestofTime(m_secondCount);
}

void MainWindow::slotWorking2Stop() {
    qDebug() << "slotWorking2Stop" << endl;
    countTimer->stop();
    m_secondCount = 0;
    currentTimer->start(1000);
    /*
    qDebug() << " emit StopToIdle SIGNAL" << endl;
    qDebug() << "working:" << stateWorking->active() << endl;
    qDebug() << "Stop:" << stateStop->active() << endl;
    qDebug() << "Idle:" << stateIdle->active() << endl;
    qDebug() << "mac:" << stateIdle->active() << endl;
    emit StopToIdle();
    */
}

void MainWindow::slotStop2Idle() {
    qDebug() << "slotStop2Idle" << endl;
    currentTimer->start(1000);
}

void MainWindow::slotCooking2Idle() {
    qDebug() << "slotCooking2Idle" << endl;
    if(countTimer->isActive()) {
        countTimer->stop();
    }
    currentTimer->start(1000);
    m_secondCount = 0;
}

void MainWindow::slotCooking2Cooking() {
    qDebug() << "slotCooking2Cooking" << endl;
    if(countTimer->isActive() && !currentTimer->isActive())
        qDebug() << m_secondCount << "+" << SIXTYSECONDS << endl;
        m_secondCount += SIXTYSECONDS;
}

void MainWindow::showDial() {
    qDebug() << "showDial" << ui->dial->value() << endl;
    if(currentTimer->isActive())
        currentTimer->stop();
}

void MainWindow::tik_tok() {
    qDebug() << "tik_tok" << endl;
    if(m_secondCount < 0) {
        m_secondCount = 0;
        countTimer->stop();
        currentTimer->start(1000);
        emit FinishedCooking();
    }
    m_secondCount--;
    ui->lcdClock->showRestofTime(m_secondCount);
}

int MainWindow::getRandom(int min,int max)
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    int num = qrand() % (max - min);
    //qDebug() << num + min;
    return num + min;
}

void MainWindow::showDebug() {
    qDebug() << "It is working!\n";
}
