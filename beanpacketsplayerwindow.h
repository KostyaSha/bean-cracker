#ifndef BEANPACKETSPLAYERWINDOW_H
#define BEANPACKETSPLAYERWINDOW_H

#include "beanpacketsplayermodel.h"

#include <QMainWindow>

namespace Ui {
class BeanPacketsPlayerWindow;
}

class BeanPacketsPlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BeanPacketsPlayerWindow(QWidget *parent = nullptr);
    ~BeanPacketsPlayerWindow() override;

    bool getConnected();
    void setConnected(bool value);

    static QSerialPort *getSerialPort();
private slots:
    void on_actionLoadFile_triggered();

    void on_cbDelay_stateChanged(int arg1);

    void on_actionPlayPause_triggered();

private:
    Ui::BeanPacketsPlayerWindow *ui;

    BeanPacketsPlayerModel *playModel;
    bool connected;

//    MainWindow *getMainWindow();
};

#endif // BEANPACKETSPLAYERWINDOW_H
