#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize:
    defaultTimer = new QTimer(this);
    countTimer = new QTimer(this);
    settingTimer = new QTimer(this);
    mac = new QStateMachine();

    // States:
    stateWorking = new QState();
    stateStop = new QState();
    stateCooking = new QState(stateWorking); //count down
    stateIdle = new QState(stateWorking); // display current time
    stateDefrost = new QState(stateWorking);
    stateClock = new QState(stateWorking);
    stateSetHour = new QState(stateWorking);
    stateSetMinute = new QState(stateWorking);

    // StateMachine:
    mac->addState(stateWorking);
    mac->addState(stateStop);
    mac->setInitialState(stateWorking);
    stateWorking->setInitialState(stateIdle);
    ui->dial->setMaximum(23);

    // InitialState Idle:
    settingTimer->stop();
    countTimer->stop();
    defaultTimer->start(1000);
    connect(defaultTimer, SIGNAL(timeout()), ui->lcdClock, SLOT(showDefaultTime()));
    connect(settingTimer, SIGNAL(timeout()), this, SLOT(tik_tok()));
    connect(countTimer, SIGNAL(timeout()), this, SLOT(tik_tok()));
    connect(ui->dial, SIGNAL(valueChanged(int)), ui->dial, SLOT(setValue(int)));
    connect(ui->dial, SIGNAL(valueChanged(int)), ui->lcdClock, SLOT(showCount(int)));
    connect(ui->dial, SIGNAL(valueChanged(int)), this, SLOT(showDial()));

    // Transitions:
    addTrans(stateIdle, stateCooking, ui->btnStart, SIGNAL(clicked()), this, SLOT(slotIdle2Cooking()));
    addTrans(stateCooking, stateIdle, this, SIGNAL(FinishedCooking()), this, SLOT(slotCooking2Idle()));
    addTrans(stateCooking, stateCooking, ui->btnStart, SIGNAL(clicked()), this, SLOT(slotCooking2Cooking()));
    addTrans(stateWorking, stateStop, ui->btnStop, SIGNAL(clicked()), this, SLOT(slotWorking2Stop()));
    stateStop->addTransition(stateWorking);
    //addTrans(stateStop, stateIdle, this, SIGNAL(StopToIdle()), this, SLOT(slotStop2Idle()));
    addTrans(stateIdle, stateDefrost, ui->btnDefrost, SIGNAL(clicked()), this, SLOT(slotIdle2Defrost()));
    addTrans(stateDefrost, stateCooking, ui->btnStart, SIGNAL(clicked()), this, SLOT(slotDefrost2Cooking()));
    // Clock:
    addTrans(stateIdle, stateSetHour, ui->btnClock, SIGNAL(clicked()), this, SLOT(slotIdle2Clock()));  // Hours setting
    addTrans(stateSetHour, stateSetMinute, ui->btnClock, SIGNAL(clicked()), this, SLOT(slotClock2Clock()));// Minutes setting
    addTrans(stateSetMinute, stateIdle, ui->btnClock, SIGNAL(clicked()), this, SLOT(slotClock2Idle()));  // Validation

    mac->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}


// Clock Branch:
void MainWindow::slotIdle2Clock() {
    ui->lcdClock->display("00:");
    ui->label->setText("hour(s)");
    ui->dial->setMinimum(0);
    ui->dial->setMaximum(23);
    m_setClock = true;
    if(defaultTimer->isActive())
        defaultTimer->stop();
}
void MainWindow::slotClock2Clock() {
    ui->lcdClock->display(":00");
    ui->label->setText("miniute(s)");
    ui->dial->setMinimum(0);
    ui->dial->setMaximum(60);
    int hours = ui->dial->value();
    m_settingSeconds = hours * 60;
    qDebug() << hours << " hours = " << m_settingSeconds << " minutes" << endl;
}
void MainWindow::slotClock2Idle() {
    ui->label->setText("Validation");
    int minutes = ui->dial->value();
    qDebug() << minutes << " minutes = " << endl;
    m_settingSeconds = ((m_settingSeconds + minutes) * 60);
    m_settingSeconds %= 86400;// 24 * 3600
    m_settingSeconds += QTime::currentTime().second();
    qDebug() << m_settingSeconds << " total seconds";
    ui->lcdClock->showSettingTime(m_settingSeconds);
    if(!settingTimer->isActive())
        settingTimer->start(1000);
}

/* Clock Setting
void MainWindow::slotIdle2Clock() {
    ui->label->setText("hour(s)");
    ui->dial->setMinimum(0);
    ui->dial->setMaximum(23);
    m_setClock = true;
    if(defaultTimer->isActive())
        defaultTimer->stop();
}
void MainWindow::slotClock2Clock() {
    ui->lcdClock->display("00");
    ui->label->setText("miniute(s)");
    ui->dial->setMinimum(0);
    ui->dial->setMaximum(60);
    int hours = ui->dial->value() - QTime::currentTime().hour();
    m_settingSeconds = hours * 60;
    qDebug() << hours << "hours = " << m_settingSeconds << "minutes" << endl;
}
void MainWindow::slotClock2Idle() {
    ui->lcdClock->display("00");
    ui->label->setText("Validation");
    int minutes = ui->dial->value() - QTime::currentTime().minute();
    qDebug() << minutes << "minutes = " << endl;
    m_settingSeconds = ((m_settingSeconds + minutes) * 60);
    m_settingSeconds += ((QTime::currentTime().hour() * 60) + QTime::currentTime().minute()) * 60;
    m_settingSeconds %= 86400;// 24 * 3600
    m_settingSeconds += QTime::currentTime().second();
    ui->lcdClock->showSettingTime(m_settingSeconds);
    if(!settingTimer->isActive())
        settingTimer->start(1000);
}
*/

// Defrost Branch:
void MainWindow::slotDefrost2Cooking() {
    ui->label->setText("Defrost2Cooking");
    qDebug() << "slotDefrost2Cooking" << endl;
    countTimer->start(1000);
}

// Idle:
void MainWindow::slotIdle2Cooking() {
    ui->label->setText("Idle2Cooking");
    qDebug() << "slotIdle2Cooking" << endl;
    m_countSeconds = SIXTYSECONDS;
    if(defaultTimer->isActive())
        defaultTimer->stop();
    countTimer->start(1000);
}
void MainWindow::slotIdle2Defrost() {
    ui->label->setText("Idle2Defrost");
    qDebug() << "slotIdle2Defrost" << endl;
    if(defaultTimer->isActive())
        defaultTimer->stop();
    m_countSeconds = getRandom(30, 120);
    ui->lcdClock->showSettingTime(m_countSeconds);
}

// Working
void MainWindow::slotWorking2Stop() {
    ui->label->setText("Working2Stop");
    qDebug() << "slotWorking2Stop" << endl;
    countTimer->stop();
    m_countSeconds = 0;
    //m_settingSeconds = 0;
    m_setClock == false ? defaultTimer->start(1000) : settingTimer->start(1000);
    /*
    qDebug() << " emit StopToIdle SIGNAL" << endl;
    qDebug() << "working:" << stateWorking->active() << endl;
    qDebug() << "Stop:" << stateStop->active() << endl;
    qDebug() << "Idle:" << stateIdle->active() << endl;
    qDebug() << "mac:" << stateIdle->active() << endl;
    emit StopToIdle();
    */
}
// Stop
void MainWindow::slotStop2Idle() { //
    ui->label->setText("Stop2Idle");
    qDebug() << "slotStop2Idle" << endl;
    m_setClock == false ? defaultTimer->start(1000) : settingTimer->start(1000);
}
// Cooking
void MainWindow::slotCooking2Idle() {
    ui->label->setText("Cooking2Idle");
    qDebug() << "slotCooking2Idle" << endl;
    if(countTimer->isActive()) {
        countTimer->stop();
    }
    defaultTimer->start(1000);
    m_countSeconds = 0;
}
void MainWindow::slotCooking2Cooking() {
    ui->label->setText("Cooking2Cooking");
    qDebug() << "slotCooking2Cooking" << endl;
    if(countTimer->isActive() && !defaultTimer->isActive())
        qDebug() << m_countSeconds << "+" << SIXTYSECONDS << endl;
        m_countSeconds += SIXTYSECONDS;
}

//-----------------------------------------------------------------------------------
void MainWindow::showDial() {
    //qDebug() << "showDial" << ui->dial->value() << endl;
    if(defaultTimer->isActive() || settingTimer->isActive()) {
        settingTimer->stop();
        defaultTimer->stop();
    }
}

void MainWindow::tik_tok() {
    //qDebug() << "tik_tok" << endl;
    if(m_countSeconds < 0) {
        m_countSeconds = 0;
        countTimer->stop();
        if(m_setClock)
            settingTimer->start(1000);
        else
            defaultTimer->start(1000);
        emit FinishedCooking();
    }
    if(m_setClock) {
        m_settingSeconds++; m_settingSeconds %= 86400; // 24 hours;
        ui->lcdClock->showSettingTime(m_settingSeconds);
    }
    else {
        m_countSeconds--;
        ui->lcdClock->showSettingTime(m_countSeconds);
    }
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
