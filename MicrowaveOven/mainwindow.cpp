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
    ui->dial->setEnabled(false);

    // States:
    stateWorking = new QState();
    stateStop = new QState();
    stateCooking = new QState(stateWorking); //count down
    stateIdle = new QState(stateWorking); // display current time
    stateDefrost = new QState(stateWorking);
    //stateClock = new QState(stateWorking);
    stateSetHour = new QState(stateWorking);
    stateSetMinute = new QState(stateWorking);
    stateSetPower = new QState(stateWorking);
    stateSetMode = new QState(stateWorking);
    stateSetDuration = new QState(stateWorking);

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
    connect(defaultTimer, SIGNAL(timeout()), this, SLOT(showCurrentTime()));
    connect(settingTimer, SIGNAL(timeout()), this, SLOT(tik_tok_up()));
    connect(countTimer, SIGNAL(timeout()), this, SLOT(tik_tok_down()));
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
    addTrans(stateIdle, stateSetHour, ui->btnClock, SIGNAL(clicked()), this, SLOT(slotIdle2Clock()));       // Hours setting
    addTrans(stateSetHour, stateSetMinute, ui->btnClock, SIGNAL(clicked()), this, SLOT(slotClock2Clock())); // Minutes setting
    addTrans(stateSetMinute, stateIdle, ui->btnClock, SIGNAL(clicked()), this, SLOT(slotClock2Idle()));     // Validation
    // Power:
    addTrans(stateIdle, stateSetPower, ui->btnPower, SIGNAL(clicked()), this, SLOT(slotIdle2Power()));
    addTrans(stateSetPower, stateSetDuration, ui->btnPower, SIGNAL(clicked()), this, SLOT(slotPower2Duration()));
    addTrans(stateSetDuration, stateCooking, ui->btnStart, SIGNAL(clicked()), this, SLOT(slotDuration2Cooking()));
    // Mode:
    addTrans(stateIdle, stateSetMode, ui->btnMode, SIGNAL(clicked()), this, SLOT(slotIdle2Mode()));
    addTrans(stateSetMode, stateSetDuration, ui->btnMode, SIGNAL(clicked()), this, SLOT(slotMode2Duration()));
    addTrans(stateSetDuration, stateCooking, ui->btnStart, SIGNAL(clicked()), this, SLOT(slotDuration2Cooking()));

    mac->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Duration
void MainWindow::slotDuration2Cooking() {
    m_countSeconds = ui->dial->value();
    ui->label->setText("cooking");
    qDebug() << m_countSeconds << " m_countSeconds" << endl;
    countTimer->start(1000);
    m_blockDisplay = false;
    ui->dial->setEnabled(false);
}

// Mode Branch:
void MainWindow::slotIdle2Mode() {
    ui->dial->setEnabled(true);
    m_setMode = true;
    ui->lcdClock->display("3/3");
    ui->label->setText("modes");
    ui->dial->setMinimum(1);
    ui->dial->setMaximum(3);
    if(defaultTimer->isActive() || settingTimer->isActive()) {
        defaultTimer->stop();
        settingTimer->stop();
        m_blockDisplay = true;
    }
}
void MainWindow::slotMode2Duration() {
    m_setMode = false;
    ui->label->setText("seconds");
    if(ui->dial->value() == 1)
        m_mode = microwave;
    else if(ui->dial->value() == 2)
        m_mode = grill;
    else if (ui->dial->value() == 3)
        m_mode = microwave_grill;
    else
        m_mode = error;
    ui->dial->setMinimum(0);
    ui->dial->setMaximum(3600);
    qDebug() << m_mode << " mode" << endl;
}


// Power Branch:
void MainWindow::slotIdle2Power() {
    ui->dial->setEnabled(true);
    ui->lcdClock->display("00.");
    ui->label->setText("%");
    ui->dial->setMinimum(0);
    ui->dial->setMaximum(100);
    if(defaultTimer->isActive() || settingTimer->isActive()) {
        defaultTimer->stop();
        settingTimer->stop();
        m_blockDisplay = true;
    }
}
void MainWindow::slotPower2Duration() {
    m_power = ui->dial->value();
    ui->label->setText("seconds");
    ui->dial->setMinimum(0);
    ui->dial->setMaximum(3600);
    qDebug() << m_power << " % powers" << endl;
}

/* Clock Setting old version
// Clock Branch:
void MainWindow::slotIdle2Clock() {
    ui->dial->setEnabled(true);
    ui->lcdClock->display("00:");
    ui->label->setText("hour(s)");
    ui->dial->setMinimum(0);
    ui->dial->setMaximum(23);
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
    ui->dial->setEnabled(false);
    m_setClock = true; // stop can block the setting
}
*/

// Clock Branch: keep the setting time right.
void MainWindow::slotIdle2Clock() {
    ui->dial->setEnabled(true);
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
    ui->dial->setEnabled(false);
}

// Defrost Branch:
void MainWindow::slotDefrost2Cooking() {
    ui->label->setText("Defrosting..");
    qDebug() << "slotDefrost2Cooking" << endl;
    countTimer->start(1000);
}

// Idle:
void MainWindow::slotIdle2Cooking() {
    ui->label->setText("Start Cooking");
    qDebug() << "slotIdle2Cooking" << endl;
    m_countSeconds = SIXTYSECONDS;
    if(defaultTimer->isActive())
        defaultTimer->stop();
    countTimer->start(1000);
}
void MainWindow::slotIdle2Defrost() {
    ui->label->setText("Defrost");
    qDebug() << "slotIdle2Defrost" << endl;
    if(defaultTimer->isActive() || settingTimer->isActive()) {
        defaultTimer->stop();
        settingTimer->stop();
    }
    m_countSeconds = getRandom(30, 120);
    ui->lcdClock->showRestofTime(m_countSeconds);
}


// Working
void MainWindow::slotWorking2Stop() {
    ui->label->setText("Stop");
    qDebug() << "slotWorking2Stop" << endl;
    countTimer->stop();
    m_countSeconds = 0;
    //m_settingSeconds = 0;
    if(m_setClock)
        settingTimer->start(1000);
    else {
        defaultTimer->start(1000);
    }

    /*
    qDebug() << " emit StopToIdle SIGNAL" << endl;
    qDebug() << "working:" << stateWorking->active() << endl;
    qDebug() << "Stop:" << stateStop->active() << endl;
    qDebug() << "Idle:" << stateIdle->active() << endl;
    qDebug() << "mac:" << stateIdle->active() << endl;
    emit StopToIdle();
    */
}

/*
// Stop
void MainWindow::slotStop2Idle() { //
    ui->label->setText("Standby");
    qDebug() << "slotStop2Idle" << endl;
    m_setClock == false ? defaultTimer->start(1000) : settingTimer->start(1000);
    m_blockDisplay = false;
    ui->dial->setEnabled(false);
}*/

// Cooking
void MainWindow::slotCooking2Idle() {
    ui->label->setText("Standby");
    qDebug() << "slotCooking2Idle" << endl;
    if(countTimer->isActive()) {
        countTimer->stop();
    }
    m_setClock == false ? defaultTimer->start(1000) : settingTimer->start(1000);
    m_countSeconds = 0;
}
void MainWindow::slotCooking2Cooking() {
    ui->label->setText("Cooking.");
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
    if(m_setMode) {
        if(ui->dial->value() == 1)
            ui->label->setText("microwave");
        else if(ui->dial->value() == 2)
            ui->label->setText("grill");
        else if(ui->dial->value() == 3)
            ui->label->setText("wave+grill");
        else
            ui->label->setText("mode error");
    }
}

void MainWindow::tik_tok_up() {
    //qDebug() << "tik_tok_up" << endl;
    if(defaultTimer->isActive() || countTimer->isActive()) {
        if(m_setClock)
            m_settingSeconds++;
        return;
    }
    if(m_setClock) {
        m_settingSeconds++;
        m_settingSeconds %= 86400;// 24 hours;
        qDebug() << m_settingSeconds << "seconds abs current" << endl;
    }
    if(!m_blockDisplay) {
        ui->lcdClock->showSettingTime(m_settingSeconds);
    }
}

void MainWindow::tik_tok_down() {
    //qDebug() << "tik_tok_down" << endl;
    if(m_countSeconds < 0) {
        m_countSeconds = 0;
        countTimer->stop();
        if(m_setClock)
            settingTimer->start(1000);
        else
            defaultTimer->start(1000);
        emit FinishedCooking();
    }
    m_countSeconds--;
    qDebug() << m_countSeconds << "seconds count down" << endl;
    if(!m_blockDisplay) {
        ui->lcdClock->showRestofTime(m_countSeconds);
    }
}

int MainWindow::getRandom(int min,int max)
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    int num = qrand() % (max - min);
    //qDebug() << num + min;
    return num + min;
}

void MainWindow::showCurrentTime() {
    QTime time = QTime::currentTime();
    //qDebug() << time.hour() << "h" << time.minute() << "m" << time.second() << "s" << endl;
    ui->lcdClock->showDefaultTime(time);
}

void MainWindow::showDebug() {
    qDebug() << "It is working!\n";
}
