#ifndef BEANPACKETSLOGGERWINDOW_H
#define BEANPACKETSLOGGERWINDOW_H

#include "beanpacketsloggermodel.h"
#include "beanpacket.h"

#include <QMainWindow>

namespace Ui {
class BeanPacketsLoggerWindow;
}

class BeanPacketsLoggerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BeanPacketsLoggerWindow(QWidget *parent = nullptr);
    ~BeanPacketsLoggerWindow() override;

    void appendPacket(QSharedPointer<BeanPacket> packet);

//    void saveAsCSV(const QString filename);

private slots:
    void on_pushButton_clicked();

    void on_actionSaveAll_triggered();

private:
    Ui::BeanPacketsLoggerWindow *ui;
    BeanPacketsLoggerModel *logModel;

};

#endif // BEANPACKETSLOGGERWINDOW_H
