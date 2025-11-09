#ifndef BEANPACKETSMONITOR_H
#define BEANPACKETSMONITOR_H

#include "beanpacket.h"
#include "beanpacketsmonitormodel.h"

#include <QFrame>

namespace Ui {
class BeanPacketsMonitor;
}

class BeanPacketsMonitor : public QFrame
{
    Q_OBJECT

public:
    explicit BeanPacketsMonitor(QWidget *parent = nullptr);
    ~BeanPacketsMonitor() override;

    void initializeBeanTable();
    QSharedPointer<BeanPacket> getSelectedPacket();
    void receiveSerialLine(QSharedPointer<BeanPacket> packet);
private:
    Ui::BeanPacketsMonitor *ui;
    BeanPacketsMonitorModel *monitorModel;

    void rowChanged(int row);

private slots:
    void onUpdateTimer();

    void on_btnClearTable_clicked();
    void on_tableMonitor_clicked(const QModelIndex &index);
    void rowChanged(const QModelIndex &current, const QModelIndex &previous);
};

#endif // BEANPACKETSMONITOR_H
