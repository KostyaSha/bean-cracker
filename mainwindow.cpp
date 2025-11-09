#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QList>
#include <QTimer>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QCommonStyle>
#include <QThread>
#include <SerialWorker.h>

#include "beanpacket.h"
#include "beanpacketsloggerwindow.h"
#include "beanpacketsplayerwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


static BeanPacketsPlayerWindow *beanPlayerWindow;
static BeanPacketsLoggerWindow *beanLoggerWindow;



MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QCommonStyle style;
    ui->actionOpen_Logger->setIcon(style.standardIcon(QStyle::SP_FileDialogContentsView));
    ui->actionOpen_Player->setIcon(QIcon::fromTheme("multimedia-player"));

    ui->toolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    fillDevices();
    // initialise additional windows
    beanPlayerWindow = new BeanPacketsPlayerWindow(this);
    beanPlayerWindow->setEnabled(false);
    beanLoggerWindow = new BeanPacketsLoggerWindow(this);
    beanLoggerWindow->setEnabled(false);
    setupSerialWorker();

    auto* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onUpdateTimer);
    timer->start(200);
}

MainWindow::~MainWindow() {
    if (workerThread && workerThread->isRunning()) {
        workerThread->quit();
        workerThread->wait();
    }
    delete ui;
}

void MainWindow::onUpdateTimer()
{
    ui->beanPacketsMonitor->update();
    update();
}

void MainWindow::fillDevices() {
    QList<QSerialPortInfo> portList;

    portList = QSerialPortInfo::availablePorts();

    QList<QSerialPortInfo>::iterator i;

    ui->cbDevice->clear();

    // For local debug/testing:
    // `socat -d -d pty,raw,echo=0,link=/tmp/ttyBean pty,raw,echo=0,link=/tmp/ttyBeanIn`
    //
    ui->cbDevice->addItem(QString("/tmp/ttyBean"));

    for (i = portList.begin(); i != portList.end(); ++i) {
        ui->cbDevice->addItem(i->portName());
    }


}


void MainWindow::on_btnRefresh_clicked() {
    qDebug("Refreshing devices...");
    fillDevices();
}

void MainWindow::setupSerialWorker()
{
    worker = new SerialWorker();
    workerThread = new QThread(this);
    workerThread->setObjectName("Serial Worker Thread");
    worker->moveToThread(workerThread);

    connect(worker, &SerialWorker::beanPacketReceived, this, &MainWindow::processBeanPacket);

    connect(worker, &SerialWorker::errorOccurred, this, &MainWindow::onSerialError);
    // connect(worker, &SerialWorker::connected, this, &MainWindow::onConnected);

    connect(this, &MainWindow::configureSerial, worker, &SerialWorker::configure);
    connect(this, &MainWindow::startSerial, worker, &SerialWorker::start);
    connect(this, &MainWindow::stopSerial, worker, &SerialWorker::stop);


    // connect(this, &MainWindow::destroyed, worker, &SerialWorker::stop);
    // connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
    // connect(workerThread, &QThread::finished, workerThread, &QObject::deleteLater);

    // connect(workerThread)

    // connect(worker, &SerialWorker::error, this, &MainWindow::handleError);
    // connect(worker, &SerialWorker::dataReceived, this, &MainWindow::handleData);
    workerThread->start();
}

void MainWindow::onSerialError(const QString &error) {
    statusBar()->showMessage("Ошибка: " + error, 5000);  // Сообщение исчезнет через 5 секунд
}

void MainWindow::onSerialStart(const QString &error) {
    statusBar()->showMessage("Started: " + error, 5000);  // Сообщение исчезнет через 5 секунд
}

void MainWindow::on_btnConnect_clicked() {
    // ui->btnConnect->setEnabled(false);
    SerialSettings settings;
    settings.portName = ui->cbDevice->currentText();
    settings.baudRate = QSerialPort::Baud115200;
    // settings.baudRate = ui->comboBoxBaud->currentData().value<QSerialPort::BaudRate>();

    // Передаём настройки и запускаем
    emit configureSerial(settings);
    emit startSerial();


    // if (!serialPort) {

    //     SerialSettings settings;
    //     settings.portName = ui->cbDevice->currentText();
    //     // settings.baudRate = ui->comboBoxBaud->currentData().value<QSerialPort::BaudRate>();

    //     // Передаём настройки и запускаем
    //     emit configureSerial(settings);
    //     emit startSerial();


    //     // auto tmpPort = new QSerialPort(this);
    //     // if (ui->cbDevice->count() > 0) {
    //     //     tmpPort->setPortName(ui->cbDevice->currentText());
    //     //     tmpPort->setBaudRate(QSerialPort::Baud115200);
    //     //     tmpPort->setDataBits(QSerialPort::Data8);
    //     //     tmpPort->setParity(QSerialPort::NoParity);
    //     //     tmpPort->setStopBits(QSerialPort::OneStop);
    //     //     tmpPort->setFlowControl(QSerialPort::NoFlowControl);
    //     //     if (tmpPort->open(QSerialPort::ReadWrite)) {
    //     //         setConnected(true);
    //     //         statusBar()->showMessage(tr("Connected"));
    //     //         ui->btnConnect->setText("Disconnect");
    //     //         ui->cbDevice->setEnabled(false);
    //     //         serialPort = tmpPort;
    //     //         connect(serialPort, SIGNAL(readyRead()), this, SLOT(serialReceived()));
    //     //         if (ui->beanPacketsSender->isLiveSend()) {
    //     //             ui->beanPacketsSender->deviceSend();
    //     //         }
    //     //     } else {
    //     //         setConnected(false);
    //     //         statusBar()->showMessage(tr("Disconnected"));
    //     //         QMessageBox::warning(this, "Serial port error", "Could not open serial port");
    //     //     }
    //     // } else {
    //     //     QMessageBox::warning(this, "Serial port error", "Could not open serial port");
    //     // }
    // } else { // disconnect
    //     if (serialPort) {
    //         serialPort->close();
    //         disconnect(serialPort, SIGNAL(readyRead()));
    //         delete serialPort;
    //         serialPort = nullptr;
    //     }
    //     ui->cbDevice->setEnabled(true);
    //     ui->btnConnect->setText(tr("Connect"));
    // }

    // ui->btnConnect->setEnabled(false);
}


void MainWindow::processBeanPacket(QSharedPointer<BeanPacket> packet) {
    ui->beanPacketsMonitor->receiveSerialLine(packet);
    //ui->beanPacketsSender->packetReceived();
    if (beanLoggerWindow->isEnabled()) {
        // qDebug() << "bean logger window is visible";
        beanLoggerWindow->appendPacket(packet);
    } else {
        // qDebug() << "bean logger window is not visible";
    }
    counter++;

    statusBar()->showMessage(QString::number(counter), 2000);
}


void MainWindow::on_actionOpen_Logger_triggered() {
    beanLoggerWindow->show();
    beanLoggerWindow->setEnabled(true);
    beanLoggerWindow->activateWindow();
    beanLoggerWindow->raise();
}

void MainWindow::on_actionOpen_Player_triggered() {
    beanPlayerWindow->show();
    beanPlayerWindow->setEnabled(true);
}

void MainWindow::setConnected(bool value) {
    beanPlayerWindow->setConnected(value);
    ui->beanPacketsSender->setConnected(value);
}

void MainWindow::on_actioncopyToTransmit_triggered() {
    auto packet = ui->beanPacketsMonitor->getSelectedPacket();
    if (packet) {
        auto packetCopy = QSharedPointer<BeanPacket>::create(packet);
        ui->beanPacketsSender->appendPacket(packetCopy);
    }
}




