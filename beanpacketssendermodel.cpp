#include "beanpacketssendermodel.h"
#include <QApplication>
#include <QVariant>


#define DSTID 0
#define MSGID 1
#define DLC 2
#define DATA 3
#define CRC 4
#define PRIO 5
#define DEBUG 6
#define REPEAT 7
#define COUNTER 8

BeanPacketsSenderModel::BeanPacketsSenderModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    packets = new QList<BeanPacket*>;
}

int BeanPacketsSenderModel::appendPacket(BeanPacket *packet)
{
    packet->ro = false;
    packets->append(packet);
    layoutChanged();
    return packets->size();
}

QVariant BeanPacketsSenderModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal )
        return QVariant();

    switch (section) {
        case DSTID:
            return QString("Dst");
        case MSGID:
            return QString("Msg");
        case DLC:
            return QString("Len");
        case DATA:
            return QString("Data");
        case CRC:
            return QString("CRC");
        case PRIO:
            return QString("Pri");
        case REPEAT:
            return QString("Delay");
        case COUNTER:
            return QString("Counter");
        default:
            return QVariant();
    }
}

int BeanPacketsSenderModel::rowCount(const QModelIndex &parent) const
{
    return packets->size();
}

int BeanPacketsSenderModel::columnCount(const QModelIndex &parent) const
{
    return COUNTER +1;
}

QVariant BeanPacketsSenderModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (index.row() <= packets->size()) {
            const BeanPacket *packet = packets->at(index.row());
            switch (index.column()) {
                case DSTID:
                    return QVariant(packet->getDstIdStr());
                case MSGID:
                    return QVariant(packet->getMsgIdStr());
                case PRIO:
                    return QVariant(packet->getPrioStr());
                case DATA:
                    return QVariant(packet->getDataStr());
                case DLC:
                    return QVariant(packet->getDlcStr());
                case CRC:
                    return QVariant(packet->getCrcStr());
                case REPEAT:
                    return QVariant(packet->getRepeatStr());
                case COUNTER:
                    return QVariant(packet->getCounterStr());
                case DEBUG:
                    return QVariant(packet->debug);
                default:
                    return QVariant();
            }
        }
    } else if (role == Qt::TextAlignmentRole) {
        switch (index.column()) {
            case DATA:
                return Qt::AlignLeft + Qt::AlignVCenter;
            case PRIO:
            case DLC:
            case CRC:
                return Qt::AlignCenter;
            case MSGID:
            case DSTID:
            default:
                return Qt::AlignRight + Qt::AlignVCenter;
        }
    }
    return QVariant();
}

BeanPacket* BeanPacketsSenderModel::packetAt(int row) {
    if (row >= 0 && row < packets->size()) {
        return packets->at(row);
    }
    return nullptr;
}

void BeanPacketsSenderModel::removePacketAt(int row) {
    if (row > 0 && row <= packets->size()) {
        auto tmpPacket = packetAt(row);
        packets->removeAt(row);
        delete(tmpPacket);
        layoutChanged();
    }
}
