#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "beanpacketsloggerwindow.h"
#include "beanpacketsplayerwindow.h"

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

    static QSerialPort* getSerialPort();

    void setConnected(bool value);

private slots:
    void fillDevices();
    void on_btnRefresh_clicked();
    void on_btnConnect_clicked();
    void serialReceived();

    void on_actionOpen_Logger_triggered();

    void on_actionOpen_Player_triggered();

    void on_actioncopyToTransmit_triggered();

private:
    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
