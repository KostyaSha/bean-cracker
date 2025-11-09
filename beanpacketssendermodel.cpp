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
    packets = new QList<QSharedPointer<BeanPacket>>;
}

int BeanPacketsSenderModel::appendPacket(QSharedPointer<BeanPacket> packet)
{
    packet->ro = false;
    emit layoutAboutToBeChanged();
    packets->append(packet);
    emit layoutChanged();
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
    Q_UNUSED(parent);
    return packets->size();
}

int BeanPacketsSenderModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return COUNTER +1;
}

QVariant BeanPacketsSenderModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (index.row() <= packets->size()) {
            auto packet = packets->at(index.row());
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
                return QVariant::fromValue(Qt::AlignLeft | Qt::AlignVCenter);
            case PRIO:
            case DLC:
            case CRC:
                return Qt::AlignCenter;
            case MSGID:
            case DSTID:
            default:
                return QVariant::fromValue(Qt::AlignRight | Qt::AlignVCenter);
        }
    }
    return QVariant();
}

QSharedPointer<BeanPacket> BeanPacketsSenderModel::getPacketAt(int row) {
    if (row >= 0 && row < packets->size()) {
        return packets->at(row);
    }
    QSharedPointer<BeanPacket> ptr(nullptr);
    return ptr;
}

void BeanPacketsSenderModel::removePacketAt(int row) {
    if (row > 0 && row <= packets->size()) {
        auto tmpPacket = getPacketAt(row);
        packets->removeAt(row);
        // delete(tmpPacket);
        emit layoutChanged();
    }
}
