#ifndef BEANPACKETSPLAYERMODEL_H
#define BEANPACKETSPLAYERMODEL_H

#include "beanpacket.h"

#include <QAbstractTableModel>



class BeanPacketsPlayerModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit BeanPacketsPlayerModel(QObject *parent = nullptr);
    ~BeanPacketsPlayerModel() override;

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void loadFromCSV(const QString &fileName);

    QList<BeanPacket*> getPackets() const;
    void setPackets(const QList<BeanPacket*> &value);

private:
    QList<BeanPacket*> *packets;
};

#endif // BEANPACKETSPLAYERMODEL_H
