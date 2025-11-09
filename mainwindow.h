#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "serialworker.h"

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void setConnected(bool value);


signals:
    void configureSerial(const SerialSettings &settings);
    void startSerial();
    void stopSerial();

public slots:
    void onSerialError(const QString &error);
    void onSerialStart(const QString &error);

private slots:
    void fillDevices();
    void on_btnRefresh_clicked();
    void on_btnConnect_clicked();

    void processBeanPacket(QSharedPointer<BeanPacket> packet);

    void on_actionOpen_Logger_triggered();

    void on_actionOpen_Player_triggered();

    void on_actioncopyToTransmit_triggered();

    void onUpdateTimer();
private:
    void setupSerialWorker();

    Ui::MainWindow *ui;

    SerialWorker *worker = nullptr;
    QThread *workerThread = nullptr;
    int counter = 0;

};

#endif // MAINWINDOW_H
