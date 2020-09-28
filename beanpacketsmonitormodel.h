#ifndef BEANPACKETSMONITORMODEL_H
#define BEANPACKETSMONITORMODEL_H

#include <QAbstractTableModel>
#include "beanpacket.h"

class BeanPacketsMonitorModel : public QAbstractTableModel
{
    Q_OBJECT



public:
    explicit BeanPacketsMonitorModel(QObject *parent = nullptr);
    void clearPackets();
    void appendPacket(BeanPacket *packet, bool groupMsg);
    BeanPacket *getPacketAt(int i);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QList<BeanPacket *> *packets;
};

#endif // BEANPACKETSMONITORMODEL_H
