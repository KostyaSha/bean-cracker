#include "beanpacket.h"
#include "mainwindow.h"
#include "beanpacketssender.h"

#include <QApplication>
#include <QRegularExpressionMatch>
#include <QDateTime>
#include <QDebug>
#include <QTimer>

#include "bean.h"

BeanPacket::BeanPacket() = default;

BeanPacket::~BeanPacket() = default;

BeanPacket::BeanPacket(BeanPacket *srcPacket) {
    dstId = srcPacket->dstId;
    msgId = srcPacket->msgId;
    dlc = srcPacket->dlc;
    memcpy(&data, &srcPacket->data, 11);
    prio = srcPacket->prio;
    parsedDlc = srcPacket->parsedDlc;
    crc = srcPacket->crc;
    bad = srcPacket->bad;
    debug = srcPacket->debug;
    timeEpoch = srcPacket->timeEpoch;
    counter = srcPacket->counter;
    repeat = srcPacket->repeat;
    ro = srcPacket->ro;
}

BeanPacket::BeanPacket(QSharedPointer<BeanPacket> srcPacket) {
    dstId = srcPacket->dstId;
    msgId = srcPacket->msgId;
    dlc = srcPacket->dlc;
    memcpy(&data, &srcPacket->data, 11);
    prio = srcPacket->prio;
    parsedDlc = srcPacket->parsedDlc;
    crc = srcPacket->crc;
    bad = srcPacket->bad;
    debug = srcPacket->debug;
    timeEpoch = srcPacket->timeEpoch;
    counter = srcPacket->counter;
    repeat = srcPacket->repeat;
    ro = srcPacket->ro;
}

// TODO replace with QSharedPointer
BeanPacket* BeanPacket::fromBeanFrame(const BeanFrame *frame) {
    auto beanPacket = new BeanPacket();
    beanPacket->timeEpoch = QDateTime::currentMSecsSinceEpoch();
    beanPacket->prio = (frame->pri_ml >> 4) & 0x0F;  // high
    beanPacket->dstId = frame->dst_id;
    beanPacket->msgId = frame->mes_id;
    beanPacket->dlc = (frame->pri_ml) & 0x0F; // low
    memcpy(beanPacket->data, frame->data, 11);
    beanPacket->crc = frame->crc;

    return beanPacket;
}

// TODO replace with QSharedPointer
QSharedPointer<BeanPacket> BeanPacket::fromCobsDecoded(const uint8_t *cobs_buffer) {
    // auto beanPacket = new BeanPacket();

    QSharedPointer<BeanPacket> beanPacket(new BeanPacket(), &QObject::deleteLater);

    beanPacket->timeEpoch = QDateTime::currentMSecsSinceEpoch();

    beanPacket->prio = (cobs_buffer[1] >> 4) & 0x0F;  // high
    beanPacket->dlc = cobs_buffer[1] & 0x0F; // low
    beanPacket->dstId = cobs_buffer[2];
    beanPacket->msgId = cobs_buffer[3];

    memcpy(beanPacket->data, &cobs_buffer[4], 11);

    beanPacket->crc = cobs_buffer[14];
    beanPacket->ack = cobs_buffer[15];
    return beanPacket;
}



BeanPacket *BeanPacket::fromSerialMsg(const QString &serialMsg) {
    auto beanPacket = new BeanPacket();
    beanPacket->timeEpoch = QDateTime::currentMSecsSinceEpoch();

    static QRegularExpression re(
            ".*Bytes:\\s(?<prio>[a-f0-9]?)(?<len>[a-f0-9])\\s(?<dst>[a-f0-9]+)\\s(?<msg>[a-f0-9]+)\\s(?<data>.*)\\s"
            "(?<tail>(?:[0-9a-f]+\\s){3})CRC:\\s"
            "(?<crc2>[a-f0-9]+)\\s+\\(?(?<count>[0-9]+\\/[0-9]+)\\)?.*");
    if (!re.isValid()) {
        qWarning("Wrong regexp!");
    }

    static QRegularExpressionMatch match = re.match(serialMsg);
    if (!match.hasMatch()) {
        beanPacket->bad = false;
        return beanPacket;
    }
    //QString capCount = match.captured("count");
    beanPacket->parsedDlc = match.captured("len");
    beanPacket->debug = serialMsg;

    bool finalOk = true;
    bool ok;

    beanPacket->prio = match.captured("prio").toUInt(&ok, 16);
    if (!ok) finalOk = false;

    beanPacket->dstId = match.captured("dst").toUInt(&ok, 16);
    if (!ok) finalOk = false;

    beanPacket->msgId = match.captured("msg").toUInt(&ok, 16);
    if (!ok) finalOk = false;

//    QString captData = ;
//    qDebug() << "Captured data: " << captData;
//    QString parsedData = parseData(captData);
//    qDebug() << "Parsed data: " << parsedData.data();
    beanPacket->dataFrom(match.captured("data"));

    beanPacket->crc = match.captured("crc2").toUInt(&ok, 16);
    if (!ok) finalOk = false;

    if (finalOk) {
        beanPacket->bad = false;
    }

    return beanPacket;
}

BeanPacket *BeanPacket::fromCSVLine(const QString &csvLine) {
    QStringList localSplit = csvLine.split(";");

    auto beanPacket = new BeanPacket();
    qDebug() << "split size: " << localSplit.size();
    if (localSplit.size() == 9) {
        beanPacket->bad = true;
    } else {
        return beanPacket;
    }
    bool ok;
    beanPacket->dstId = trimQuotes(localSplit[3]).toUInt(&ok, 16);
    if (!ok) beanPacket->bad = false;

    beanPacket->msgId = trimQuotes(localSplit[4]).toUInt(&ok, 16);
    if (!ok) beanPacket->bad = false;
    beanPacket->dlc = trimQuotes(localSplit[5]).toUInt(&ok, 16);
    if (!ok) beanPacket->bad = false;

    beanPacket->dataFrom(parseData(trimQuotes(localSplit[6])));
    beanPacket->crc = trimQuotes(localSplit[7]).toUInt(&ok, 16);
    if (!ok) beanPacket->bad = false;

    QString prioStr = "0x0" + trimQuotes(localSplit[8]);
    qDebug() << "Prio: " << prioStr;
    beanPacket->prio = prioStr.toUInt(&ok, 16);
    if (!ok) beanPacket->bad = false;

    beanPacket->timeEpoch = trimQuotes(localSplit[2]).toUInt(&ok, 16);
    if (!ok) beanPacket->bad = false;

    return beanPacket;
}


QByteArray BeanPacket::cobsEncode(const QByteArray& input) {
    QByteArray output;
    output.resize(input.size() + 2);  // Worst case

    int read = 0, write = 1;
    int code_index = 0;
    uint8_t code = 1;

    while (read < input.size()) {
        if (input[read] == 0) {
            output[code_index] = code;
            code_index = write++;
            code = 1;
            ++read;
        } else {
            output[write++] = input[read++];
            ++code;
            if (code == 0xFF) {
                output[code_index] = code;
                code_index = write++;
                code = 1;
            }
        }
    }

    output[code_index] = code;
    output.resize(write);
    output.append('\0');

    return output;
}

void BeanPacket::sendToSerialBin(QSerialPort *seriall) {
    qDebug() << "Sending to serial " << this;

    BeanFrame* tmpFrame = new BeanFrame();
    tmpFrame->pri_ml = (prio << 4) + dlc;
    memcpy(tmpFrame->data, data, 11);
    tmpFrame->dst_id = dstId;
    tmpFrame->mes_id = msgId;

    QByteArray raw(sizeof(BeanFrame), Qt::Uninitialized);
    memcpy(raw.data(), &tmpFrame, sizeof(BeanFrame));


    QByteArray encoded = cobsEncode(raw);
    seriall->write(encoded);

    // uint8_t payload[14];
    // payload[0] = prio;
    // payload[1] = msgId;
    // payload[2] = dlc;
    // uint8_t len = dlc + 3;

    // memcpy(&payload[3], &data[0], dlc);
    // seriall->write("Test\n");
    // seriall->write((const char *) payload, sizeof(len));
    bool sent = seriall->waitForBytesWritten();
    qDebug() << "sent packet: " << sent;
}
// void BeanPacket::sendToSerialBin(QSerialPort *seriall) {
//     qDebug() << "Sending to serial " << this;
//     uint8_t payload[14];
//     payload[0] = prio;
//     payload[1] = msgId;
//     payload[2] = dlc;
//     uint8_t len = dlc + 3;

//     memcpy(&payload[3], &data[0], dlc);
//         seriall->write("Test\n");
//         seriall->write((const char *) payload, sizeof(len));
//     bool sent = seriall->waitForBytesWritten();
//     qDebug() << "sent packet: " << sent;
// }



void BeanPacket::sendPacket() {
    // auto seriall = MainWindow::getSerialPort();
    // if (seriall) {
    //     sendToSerialBin(seriall);
    //     counter++;
    //     notifyModel();
    // } else {
    //     qWarning() << "Serial is not connected!";
    // }
}

void BeanPacket::notifyModel() {
     foreach(QWidget *widget, qApp->topLevelWidgets()) {
        if (auto *mainWindow = dynamic_cast<MainWindow *>(widget)) {
            auto senderWidget = mainWindow->findChild<BeanPacketsSender *>("beanPacketsSender");
            if (senderWidget) {
                senderWidget->dataChanged();
            }
            break;
        }
     }
}

//QSerialPort *BeanPacket::getSerialPort(void) {
//    MainWindow window;
//    foreach(QWidget *widget, qApp->topLevelWidgets()) {
//        if (MainWindow *mainWindow = qobject_cast<MainWindow*>(widget)) {
//            return  mainWindow->getSerialPort();
//            break;
//        }
//    }

//    return NULL;
//}

void BeanPacket::dataFrom(const QString &str) {
    dataFrom(parseData(str));
}


void BeanPacket::dataFrom(const QByteArray &arr) {
    dlc = (uint8_t) arr.size();
    qDebug() << "Array size: " << dlc;
    for (int i = 0; i < arr.size(); i++) {
        data[i] = (uint8_t) arr.at(i);
    }
}

QByteArray BeanPacket::parseData(const QString &dataStr) {
    QByteArray octets;
    QStringList localSplit = dataStr.split(" ");
    int len = 0;
    for (const auto &srcOctet : localSplit) {
        len++;
        QString tempStr = "0x";
        if (srcOctet.length() == 1) {
            tempStr.append("0");
        }
        tempStr.append(srcOctet);
        octets.resize(len);

        qDebug() << " Tempstr: " << tempStr << " > " << tempStr.toUInt();
        bool ok;
        octets[len - 1] = tempStr.toUInt(&ok, 16);
    }

    return octets;
}


QString BeanPacket::parseDataStr(const QString &dataStr) {
    QStringList octets;
    QStringList localSplit = dataStr.split(" ");
    for (const auto &srcOctet : localSplit) {
        if (srcOctet.length() == 1) {
            QString tempStr = "0";
            tempStr.append(srcOctet);
            octets.append(tempStr);
        } else {
            octets.append(srcOctet);
        }

    }

    return octets.join(" ");
}

QString BeanPacket::getParsedDlc() const {
    return QString::number(parsedDlc.toInt() - 2);
}

QString BeanPacket::getPrioStr() const {
    return QString::number(prio);
}

QString BeanPacket::trimQuotes(QString &arg) {
    QString stripped = arg;
    if (arg.startsWith("\"") && arg.endsWith("\"")) {
        stripped = arg.mid(1, arg.size() - 2);
    }
    qDebug() << "Final line: " << stripped;
    return stripped;
}

void BeanPacket::sendOnce() {
    QTimer::singleShot(0, this, SLOT(sendPacket()));
}

void BeanPacket::runPacketSending(const int millis) {

}

void BeanPacket::stopPacketSending() {

}

QString BeanPacket::getDataStr() const {
    QString tempStr;
    for (int i = 0; i < dlc; i++) {
        tempStr.append(QString("%1")
                               .arg(data[i], 2, 16, QChar('0'))
                               .toUpper());
        if (i != dlc - 1) {
            tempStr.append(" ");
        }
    }
    return tempStr;
}

QString BeanPacket::getDlcStr() const {
    return QString("%1").arg(dlc).toUpper();
}

QString BeanPacket::getMsgIdStr() const {
    return QString("%1").arg(msgId, 0, 16).toUpper();
}

QString BeanPacket::getDstIdStr() const {
    return QString("%1").arg(dstId, 0, 16).toUpper();
}

QString BeanPacket::getCrcStr() const {
    return QString("%1")
            .arg(crc, 2, 16, QChar('0'))
            .toUpper();
}

QString BeanPacket::getRepeatStr() const {
    return QString("%1").arg(repeat).toUpper();
}

QString BeanPacket::getCounterStr() const {
    return QString::number(counter);
}



