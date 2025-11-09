#include "beanpacketsmonitormodel.h"
#include "beanpacket.h"
#include <QDebug>

#define MARK 0
#define DSTID 1
#define MSGID 2
#define DLC 3
#define DATA 4
#define CRC 5
#define PRIO 6
#define REPEAT 7
#define COUNTER 8
#define COMMENT 9

BeanPacketsMonitorModel::BeanPacketsMonitorModel(QObject *parent)
        : QAbstractTableModel(parent) {
    packets = new QList<QSharedPointer<BeanPacket>>;
}


QVariant BeanPacketsMonitorModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    switch (section) {
        case MARK:
            return QString("");
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
            return QString("Repeat");
        case COUNTER:
            return QString("Counter");
        case COMMENT:
            return QString("Comment");
        default:
            return QVariant();
    }
}

int BeanPacketsMonitorModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return packets->size();
}

int BeanPacketsMonitorModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return COMMENT + 1;
}

QVariant BeanPacketsMonitorModel::data(const QModelIndex &index, int role) const {
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
                case COMMENT:
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

void BeanPacketsMonitorModel::clearPackets() {
    // qDeleteAll(*packets);

    packets->clear();
    emit layoutChanged();
}

void BeanPacketsMonitorModel::appendPacket(QSharedPointer<BeanPacket> packet, bool groupMsg) {
    // either add, either change existing
    // qDebug() << "Monitor: packets size before:" << packets->size();


    QSharedPointer<BeanPacket> targetPacket = nullptr;

    // search for existing dst
    for (int i = 0; i < packets->size(); i++) {
        auto curPacket = packets->at(i);
        // qDebug() << packet->dstId << "? local item text:" << curPacket;
        if (curPacket->dstId == packet->dstId) { // dst matches
            bool matched = true;
            if (groupMsg) {
                if (curPacket->msgId != packet->msgId) { // msg matches
                    matched = false;
                }
            }
            if (matched) {
                // qDebug() << "row " << i;
                targetPacket = curPacket;

                if (targetPacket) { // update existing
                    targetPacket->msgId = packet->msgId;
                    targetPacket->crc = packet->crc;
                    targetPacket->prio = packet->prio;
                    targetPacket->dlc = packet->dlc;
                    targetPacket->parsedDlc = packet->parsedDlc;
                    memcpy(&(targetPacket->data), &(packet->data), 11);

                    if (packet->timeEpoch >= targetPacket->timeEpoch) {
                        targetPacket->repeat = packet->timeEpoch - targetPacket->timeEpoch;
                    }
                    targetPacket->timeEpoch = packet->timeEpoch;
                    targetPacket->counter++;

                    int lastCol = columnCount() - 1;
                    QModelIndex topLeft     = index(i, 0);
                    QModelIndex bottomRight = index(i, lastCol);
                    emit dataChanged(topLeft, bottomRight);

                    // emit layoutChanged();
                    return;
                }
                break;
            }
        }
    }


        QSharedPointer<BeanPacket> copyPacket = QSharedPointer<BeanPacket>::create(packet);
        // auto copyPacket = new BeanPacket(packet);
        emit layoutAboutToBeChanged();
        packets->append(copyPacket);
        emit layoutChanged();


}

QSharedPointer<BeanPacket> BeanPacketsMonitorModel::getPacketAt(int i) {
    if (packets) {
        return packets->at(i);
    }
    QSharedPointer<BeanPacket> ptr(nullptr);
    return ptr;
}
