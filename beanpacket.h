#ifndef BEANPACKET_H
#define BEANPACKET_H

#include <QDateTime>
#include <QtSerialPort/QSerialPort>
#include <QString>
#include <QTimer>

#include <stdint.h>
#include "bean.h"

class BeanPacket : public QObject {
    Q_OBJECT

public:
    BeanPacket();

    ~BeanPacket() override;

    explicit BeanPacket(BeanPacket *srcPacket);

    explicit BeanPacket(QSharedPointer<BeanPacket> srcPacket);

    qint64 timeEpoch{}; // time recieved by app from serial or previous in monitor
    uint8_t dstId{0x00};
    uint8_t msgId{0x00};
    QString parsedDlc;
    uint8_t dlc{8};
    uint8_t data[11]{};
    uint8_t prio{0};
    uint8_t crc{0};
    bool ack;

    // for monitor
    uint counter{1};
    uint repeat{0};
    bool ro{true}; // read-only

    QString debug;
    bool bad{}; // packet is bad parsed, etc

    static BeanPacket* fromBeanFrame(const BeanFrame *frame);
    static QSharedPointer<BeanPacket> fromCobsDecoded(const uint8_t *cobs_buffer);
    static BeanPacket* fromSerialMsg(const QString &serialMsg);

    static BeanPacket* fromCSVLine(const QString &csvLine);

    void sendToSerialBin(QSerialPort *seriall);


    void runPacketSending(int millis);

    void stopPacketSending();

    QString getPrioStr() const;

    QString getParsedDlc() const;

    QString getDataStr() const;

    QString getDlcStr() const;

    QString getMsgIdStr() const;

    QString getDstIdStr() const;

    QString getCrcStr() const;

    QString getRepeatStr() const;

    QString getCounterStr() const;

    void dataFrom(const QByteArray &arr);

    QByteArray cobsEncode(const QByteArray& input);

    void sendOnce();

private  slots:
    void sendPacket();

private:
    static QString trimQuotes(QString &arg);

    static QString parseDataStr(const QString &dataStr);

    static QByteArray parseData(const QString &dataStr);

    QTimer *timer = nullptr;

    void dataFrom(const QString &str);

    void notifyModel();
};

Q_DECLARE_METATYPE(BeanPacket*)

#endif // BEANPACKET_H
