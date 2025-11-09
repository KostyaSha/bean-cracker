#ifndef BEANPACKETSSENDER_H
#define BEANPACKETSSENDER_H

#include "beanpacketsloggermodel.h"
#include "beanpacketssendermodel.h"

#include <QWidget>

namespace Ui {
class BeanPacketsSender;
}

class BeanPacketsSender : public QWidget
{
    Q_OBJECT



public:
    explicit BeanPacketsSender(QWidget *parent = nullptr);
    ~BeanPacketsSender() override;

    bool isLiveSend();
    void deviceSend();

    bool getConnected() const;
    void setConnected(bool value);

    void packetReceived();
    void appendPacket(QSharedPointer<BeanPacket> pPacket);
    void dataChanged();

private slots:

    void on_btnAdd_clicked();

    void on_tablePacketsSend_clicked(const QModelIndex &index);

    void on_btnRun_clicked();

    void on_btnDel_clicked();

    void rowChanged(const QModelIndex &current, const QModelIndex &previous);

    void on_btnShot_clicked();

    void on_btnStop_clicked();

private:
    Ui::BeanPacketsSender *ui;
    bool connected;
    BeanPacketsSenderModel *senderModel;

    void rowChanged(int row) const;

};

#endif // BEANPACKETSSENDER_H
