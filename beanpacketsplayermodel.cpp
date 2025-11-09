#include "beanpacket.h"
#include "beanpacketsplayermodel.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

// columns order
#define CBOX 0
#define TIME 1
#define DSTID 2
#define MSGID 3
#define DLC 4
#define DATA 5
#define CRC 6
#define PRIO 7
#define DEBUG 8

BeanPacketsPlayerModel::~BeanPacketsPlayerModel() = default;

BeanPacketsPlayerModel::BeanPacketsPlayerModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    packets = new QList<BeanPacket*>;
}

QVariant BeanPacketsPlayerModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    if (role != Qt::DisplayRole || orientation != Qt::Horizontal )
        return QVariant();

    switch (section) {
    case TIME:
        return QString("Time");
    case DSTID:
        return QString("Dst");
    case MSGID:
        return QString("Msg");
    case DLC:
        return QString("DLC");
    case DATA:
        return QString("Data");
    case CRC:
        return QString("CRC");
    case PRIO:
        return QString("PRI");
    case DEBUG:
        return QString("Debug");
    default:
        return QVariant();
    }
}

int BeanPacketsPlayerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return packets->size();
}

int BeanPacketsPlayerModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return DEBUG;
}

QVariant BeanPacketsPlayerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::CheckStateRole && index.column() == 0){
           return false;
    }

    if (role == Qt::DisplayRole) {
        if (index.row() <= packets->size()) {
            auto packet = packets->at(index.row());
            switch (index.column()) {
            case TIME: {
                qint64 time = 0;
                if (index.row() >= 1) {
                    const auto prevPacket = packets->at(0);
                    time = packet->timeEpoch - prevPacket->timeEpoch;
                }

                return QVariant(time);
            }
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

void BeanPacketsPlayerModel::loadFromCSV(const QString &fileName)
{
    emit layoutAboutToBeChanged();
    packets->clear();
    QFile file(fileName);
    if ( file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        QTextStream in(&file);

        int lineNum = 0;
        while (!in.atEnd()) {
            QString line = in.readLine();
            qDebug() << line;
            if (lineNum == 0) {
                lineNum++;
                continue;
            }
            auto localFromCSVLine = BeanPacket::fromCSVLine(line);
            if (localFromCSVLine->bad) {
                packets->append(localFromCSVLine);
            }
            lineNum++;
        }
        qDebug() << "Closing file. Packets size: " << packets->size();

        file.close();
    }


    emit layoutChanged();
}

QList<BeanPacket*> BeanPacketsPlayerModel::getPackets() const
{
    return *packets;
}

void BeanPacketsPlayerModel::setPackets(const QList<BeanPacket*> &value)
{
    packets = new QList<BeanPacket*>(value);
}
