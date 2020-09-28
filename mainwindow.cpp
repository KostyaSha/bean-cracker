#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QList>
#include <QTimer>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QCommonStyle>

#include "beanpacket.h"
#include "beanpacketsloggerwindow.h"
#include "beanpacketsplayerwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

static QSerialPort *serialPort;
static BeanPacketsPlayerWindow *beanPlayerWindow;
static BeanPacketsLoggerWindow *beanLoggerWindow;

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
}

MainWindow::~MainWindow() {
    delete ui;
}

QSerialPort *MainWindow::getSerialPort() {
    return serialPort;
}

void MainWindow::on_btnRefresh_clicked() {
    qDebug("Refreshing devices...");
    fillDevices();
}

void MainWindow::on_btnConnect_clicked() {
    ui->btnConnect->setEnabled(false);
    if (!serialPort) {
        auto tmpPort = new QSerialPort(this);
        if (ui->cbDevice->count() > 0) {
            tmpPort->setPortName(ui->cbDevice->currentText());
            tmpPort->setBaudRate(QSerialPort::Baud115200);
            tmpPort->setDataBits(QSerialPort::Data8);
            tmpPort->setParity(QSerialPort::NoParity);
            tmpPort->setStopBits(QSerialPort::OneStop);
            tmpPort->setFlowControl(QSerialPort::NoFlowControl);
            if (tmpPort->open(QSerialPort::ReadWrite)) {
                setConnected(true);
                statusBar()->showMessage(tr("Connected"));
                ui->btnConnect->setText("Disconnect");
                ui->cbDevice->setEnabled(false);
                serialPort = tmpPort;
                connect(serialPort, SIGNAL(readyRead()), this, SLOT(serialReceived()));
                if (ui->beanPacketsSender->isLiveSend()) {
                    ui->beanPacketsSender->deviceSend();
                }
            } else {
                setConnected(false);
                statusBar()->showMessage(tr("Disconnected"));
                QMessageBox::warning(this, "Serial port error", "Could not open serial port");
            }
        } else {
            QMessageBox::warning(this, "Serial port error", "Could not open serial port");
        }
    } else { // disconnect
        if (serialPort) {
            serialPort->close();
            disconnect(serialPort, SIGNAL(readyRead()));
            delete serialPort;
            serialPort = nullptr;
        }
        ui->cbDevice->setEnabled(true);
        ui->btnConnect->setText(tr("Connect"));
    }

    ui->btnConnect->setEnabled(true);
}

void MainWindow::serialReceived() {
    QByteArray ba;
    ba = serialPort->readLine();
    qDebug() << ba;
    statusBar()->showMessage(tr(ba), 2000);
    QString line = QString::fromStdString(ba.toStdString());
    auto packet = BeanPacket::fromSerialMsg(line);

    ui->beanPacketsMonitor->receiveSerialLine(packet);
    ui->beanPacketsSender->packetReceived();
    if (beanLoggerWindow->isEnabled()) {
        qDebug() << "bean logger window is visible";
        beanLoggerWindow->appendPacket(packet);
    } else {
        qDebug() << "bean logger window is not visible";
    }

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
    beanPlayerWindow->setConnected(serialPort);
    ui->beanPacketsSender->setConnected(serialPort);
}

void MainWindow::on_actioncopyToTransmit_triggered() {
    auto packet = ui->beanPacketsMonitor->getSelectedPacket();
    if (packet) {
        auto packetCopy = new BeanPacket(packet);
        ui->beanPacketsSender->appendPacket(packetCopy);
    }
}
