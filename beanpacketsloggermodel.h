#ifndef BEANPACKETSLOGGERMODEL_H
#define BEANPACKETSLOGGERMODEL_H

#include "beanpacket.h"

#include <QAbstractTableModel>

class BeanPacketsLoggerModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit BeanPacketsLoggerModel(QObject *parent = nullptr);
    ~BeanPacketsLoggerModel() override;

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clearPackets();
    void saveAsCSV(const QString& fileName);

    void appendPacket(QSharedPointer<BeanPacket> packet);

private:
    QList<QSharedPointer<BeanPacket>> *packets;
};

#endif // BEANPACKETSLOGGERMODEL_H
