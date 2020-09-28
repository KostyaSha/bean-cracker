#include "beanpacketsloggermodel.h"
#include "beanpacketsloggerheaderview.h"

#include <QApplication>
#include <QFile>
#include <QSize>
#include <QTextStream>
#include <qtableview.h>
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


BeanPacketsLoggerModel::BeanPacketsLoggerModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    packets = new QList<BeanPacket*>;

    //QTableView *tableLog = parent->findChild<QTableView *>("tableLog");
    //QTableView* tableLog = dynamic_cast<QTableView*>(parent);
}

BeanPacketsLoggerModel::~BeanPacketsLoggerModel() = default;

void BeanPacketsLoggerModel::appendPacket(BeanPacket *packet)
{
    qDebug() << "Adding packet to beanlogger";
    packets->append(packet);
    layoutChanged();
}

QVariant BeanPacketsLoggerModel::headerData(int section, Qt::Orientation orientation, int role) const
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

int BeanPacketsLoggerModel::rowCount(const QModelIndex &parent) const
{
    return packets->size();
}

int BeanPacketsLoggerModel::columnCount(const QModelIndex &parent) const
{
    return PRIO + 1;
}

QVariant BeanPacketsLoggerModel::data(const QModelIndex &index, int role) const
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
                    const auto prevPacket = packets->at(index.row() - 1);
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



void BeanPacketsLoggerModel::clearPackets()
{
//    qDeleteAll(packets);
    packets->clear();
    layoutChanged();
}


void BeanPacketsLoggerModel::saveAsCSV(const QString& fileName)
{
    if (fileName.isEmpty()) {
        return;
    }

    QFile f(fileName);

    if (f.open(QIODevice::WriteOnly)) {
        QTextStream ts(&f);
        QStringList strList;

        ts << " ,,Time,Dst,Msg,DLC,Data,CRC,Prio,Debug\n";

        for (auto packet : *packets) {
            strList << "\"  \""; // 0
            strList << "\"  \""; // 1
            strList << "\"" << QString::number(packet->timeEpoch) << "\"";
            strList << "\"" << packet->getDstIdStr() << "\"";
            strList << "\"" << packet->getMsgIdStr() << "\"";
            strList << "\"" << packet->getDlcStr() << "\"";
            strList << "\"" << packet->getDataStr() << "\"";
            strList << "\"" << packet->getCrcStr() << "\"";
            strList << "\"" << packet->getPrioStr() << "\"";
            strList << "\"" << packet->debug << "\"";
            ts << strList.join(",") + "\n";
            strList.clear();
        }
        f.close();
    } else {
        qWarning() << "Can't open file";
    }
}
