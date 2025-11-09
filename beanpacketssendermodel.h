#ifndef BEANPACKETSSENDERMODEL_H
#define BEANPACKETSSENDERMODEL_H

#include "beanpacket.h"

#include <QAbstractTableModel>

class BeanPacketsSenderModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit BeanPacketsSenderModel(QObject *parent = nullptr);

    void removePacketAt(int i);
    int appendPacket(QSharedPointer<BeanPacket> packet);
    QSharedPointer<BeanPacket> getPacketAt(int row);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QList<QSharedPointer<BeanPacket>> *packets;
};

#endif // BEANPACKETSSENDERMODEL_H
