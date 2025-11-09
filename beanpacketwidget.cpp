#include "beanpacketwidget.h"

#include "ui_beanpacketwidget.h"
#include <QDebug>

uint8_t bytes[11];
bool isSending = false;
bool liveSend = false;

#define CB_TOGGLE(a, b, c) c ? bytes[a] |= 1UL << b : bytes[a] &= ~(1UL << b);

BeanPacketWidget::BeanPacketWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::BeanPacketWidget) {
    ui->setupUi(this);
    beanPacket = nullptr;
    model = nullptr;
    fillPriority();
    fillDestIds();
    fillMessageIds();
    updateBytes();
    refreshEnabled();
}

BeanPacketWidget::~BeanPacketWidget() {
    delete ui;
}

void BeanPacketWidget::setPacket(QSharedPointer<BeanPacket> packet, QAbstractTableModel *packetModel) {
    beanPacket = packet;
    model = packetModel;
    loadPacket();
}

void BeanPacketWidget::loadPacket() {
    if (beanPacket) {
        qDebug() << "Loading packet into widget";
        ui->cbDestId->setCurrentIndex(beanPacket->dstId);
        ui->cbMsgId->setCurrentIndex(beanPacket->msgId);
        ui->sbBytesCount->setValue(beanPacket->dlc);
        ui->cbPriority->setCurrentIndex(beanPacket->prio);
        memcpy(&bytes, &(beanPacket->data), 11);
        updateBytes();
        refreshEnabled();
    }
}

bool BeanPacketWidget::isLiveSend() {
    return liveSend;
}

void BeanPacketWidget::fillPriority() {
    for (uint8_t i = 0; i <= 0xf; i++) {
        QString str;
        str = QString("%1").arg(i, 0, 10);
        ui->cbPriority->addItem(str);
    }

    ui->cbPriority->setCurrentIndex(0);
}

void BeanPacketWidget::fillDestIds() {
    for (uint8_t i = 0; i < 0xff; i++) {
        QString str;
        str = QString("0x%1").arg(i, 0, 16);
        ui->cbDestId->addItem(str);
    }

    ui->cbDestId->addItem(QString("0x%1").arg(0xff, 0, 16));

    ui->cbDestId->setCurrentIndex(0xfe);
}

void BeanPacketWidget::fillMessageIds() {
    for (uint8_t i = 0; i < 0xff; i++) {
        QString str;
        str = QString("0x%1").arg(i, 0, 16);
        ui->cbMsgId->addItem(str);
    }
    ui->cbMsgId->addItem(QString("0x%1").arg(0xff, 0, 16));

    ui->cbMsgId->setCurrentIndex(0x83);
}

void BeanPacketWidget::updateByte(uint8_t byte_num) {
    if (beanPacket) {
        updateBits(byte_num);
        memcpy(&(beanPacket->data), &bytes, 11);
    }
    updatePacketModel();
    switch (byte_num) {
        case 0:
            ui->lbByte1->setText(QString("%1").arg(bytes[0], 2, 16, QLatin1Char('0')).toUpper());
            break;
        case 1:
            ui->lbByte2->setText(QString("%1").arg(bytes[1], 2, 16, QLatin1Char('0')).toUpper());
            break;
        case 2:
            ui->lbByte3->setText(QString("%1").arg(bytes[2], 2, 16, QLatin1Char('0')).toUpper());
            break;
        case 3:
            ui->lbByte4->setText(QString("%1").arg(bytes[3], 2, 16, QLatin1Char('0')).toUpper());
            break;
        case 4:
            ui->lbByte5->setText(QString("%1").arg(bytes[4], 2, 16, QLatin1Char('0')).toUpper());
            break;
        case 5:
            ui->lbByte6->setText(QString("%1").arg(bytes[5], 2, 16, QLatin1Char('0')).toUpper());
            break;
        case 6:
            ui->lbByte7->setText(QString("%1").arg(bytes[6], 2, 16, QLatin1Char('0')).toUpper());
            break;
        case 7:
            ui->lbByte8->setText(QString("%1").arg(bytes[7], 2, 16, QLatin1Char('0')).toUpper());
            break;
        case 8:
            ui->lbByte9->setText(QString("%1").arg(bytes[8], 2, 16, QLatin1Char('0')).toUpper());
            break;
        case 9:
            ui->lbByte10->setText(QString("%1").arg(bytes[9], 2, 16, QLatin1Char('0')).toUpper());
            break;
        case 10:
            ui->lbByte11->setText(QString("%1").arg(bytes[10], 2, 16, QLatin1Char('0')).toUpper());
            break;
        default:
            break;
    }
}

void BeanPacketWidget::updatePacketModel() const {
    if (model) {
        emit model->layoutChanged();
    }
}

void BeanPacketWidget::updateBits(uint8_t byte_num) {
    uint8_t *byte = &(bytes[byte_num]);
    switch (byte_num) {
        case 0:
            ui->cbByte1_0->setChecked(getBit(byte, 0));
            ui->cbByte1_1->setChecked(getBit(byte, 1));
            ui->cbByte1_2->setChecked(getBit(byte, 2));
            ui->cbByte1_3->setChecked(getBit(byte, 3));
            ui->cbByte1_4->setChecked(getBit(byte, 4));
            ui->cbByte1_5->setChecked(getBit(byte, 5));
            ui->cbByte1_6->setChecked(getBit(byte, 6));
            ui->cbByte1_7->setChecked(getBit(byte, 7));
            break;
        case 1:
            ui->cbByte2_0->setChecked(getBit(byte, 0));
            ui->cbByte2_1->setChecked(getBit(byte, 1));
            ui->cbByte2_2->setChecked(getBit(byte, 2));
            ui->cbByte2_3->setChecked(getBit(byte, 3));
            ui->cbByte2_4->setChecked(getBit(byte, 4));
            ui->cbByte2_5->setChecked(getBit(byte, 5));
            ui->cbByte2_6->setChecked(getBit(byte, 6));
            ui->cbByte2_7->setChecked(getBit(byte, 7));
            break;
        case 2:
            ui->cbByte3_0->setChecked(getBit(byte, 0));
            ui->cbByte3_1->setChecked(getBit(byte, 1));
            ui->cbByte3_2->setChecked(getBit(byte, 2));
            ui->cbByte3_3->setChecked(getBit(byte, 3));
            ui->cbByte3_4->setChecked(getBit(byte, 4));
            ui->cbByte3_5->setChecked(getBit(byte, 5));
            ui->cbByte3_6->setChecked(getBit(byte, 6));
            ui->cbByte3_7->setChecked(getBit(byte, 7));
            break;
        case 3:
            ui->cbByte4_0->setChecked(getBit(byte, 0));
            ui->cbByte4_1->setChecked(getBit(byte, 1));
            ui->cbByte4_2->setChecked(getBit(byte, 2));
            ui->cbByte4_3->setChecked(getBit(byte, 3));
            ui->cbByte4_4->setChecked(getBit(byte, 4));
            ui->cbByte4_5->setChecked(getBit(byte, 5));
            ui->cbByte4_6->setChecked(getBit(byte, 6));
            ui->cbByte4_7->setChecked(getBit(byte, 7));
            break;
        case 4:
            ui->cbByte5_0->setChecked(getBit(byte, 0));
            ui->cbByte5_1->setChecked(getBit(byte, 1));
            ui->cbByte5_2->setChecked(getBit(byte, 2));
            ui->cbByte5_3->setChecked(getBit(byte, 3));
            ui->cbByte5_4->setChecked(getBit(byte, 4));
            ui->cbByte5_5->setChecked(getBit(byte, 5));
            ui->cbByte5_6->setChecked(getBit(byte, 6));
            ui->cbByte5_7->setChecked(getBit(byte, 7));
            break;
        case 5:
            ui->cbByte6_0->setChecked(getBit(byte, 0));
            ui->cbByte6_1->setChecked(getBit(byte, 1));
            ui->cbByte6_2->setChecked(getBit(byte, 2));
            ui->cbByte6_3->setChecked(getBit(byte, 3));
            ui->cbByte6_4->setChecked(getBit(byte, 4));
            ui->cbByte6_5->setChecked(getBit(byte, 5));
            ui->cbByte6_6->setChecked(getBit(byte, 6));
            ui->cbByte6_7->setChecked(getBit(byte, 7));
            break;
        case 6:
            ui->cbByte7_0->setChecked(getBit(byte, 0));
            ui->cbByte7_1->setChecked(getBit(byte, 1));
            ui->cbByte7_2->setChecked(getBit(byte, 2));
            ui->cbByte7_3->setChecked(getBit(byte, 3));
            ui->cbByte7_4->setChecked(getBit(byte, 4));
            ui->cbByte7_5->setChecked(getBit(byte, 5));
            ui->cbByte7_6->setChecked(getBit(byte, 6));
            ui->cbByte7_7->setChecked(getBit(byte, 7));
            break;
        case 7:
            ui->cbByte8_0->setChecked(getBit(byte, 0));
            ui->cbByte8_1->setChecked(getBit(byte, 1));
            ui->cbByte8_2->setChecked(getBit(byte, 2));
            ui->cbByte8_3->setChecked(getBit(byte, 3));
            ui->cbByte8_4->setChecked(getBit(byte, 4));
            ui->cbByte8_5->setChecked(getBit(byte, 5));
            ui->cbByte8_6->setChecked(getBit(byte, 6));
            ui->cbByte8_7->setChecked(getBit(byte, 7));
            break;
        case 8:
            ui->cbByte9_0->setChecked(getBit(byte, 0));
            ui->cbByte9_1->setChecked(getBit(byte, 1));
            ui->cbByte9_2->setChecked(getBit(byte, 2));
            ui->cbByte9_3->setChecked(getBit(byte, 3));
            ui->cbByte9_4->setChecked(getBit(byte, 4));
            ui->cbByte9_5->setChecked(getBit(byte, 5));
            ui->cbByte9_6->setChecked(getBit(byte, 6));
            ui->cbByte9_7->setChecked(getBit(byte, 7));
            break;
        case 9:
            ui->cbByte10_0->setChecked(getBit(byte, 0));
            ui->cbByte10_1->setChecked(getBit(byte, 1));
            ui->cbByte10_2->setChecked(getBit(byte, 2));
            ui->cbByte10_3->setChecked(getBit(byte, 3));
            ui->cbByte10_4->setChecked(getBit(byte, 4));
            ui->cbByte10_5->setChecked(getBit(byte, 5));
            ui->cbByte10_6->setChecked(getBit(byte, 6));
            ui->cbByte10_7->setChecked(getBit(byte, 7));
            break;
        case 10:
            ui->cbByte11_0->setChecked(getBit(byte, 0));
            ui->cbByte11_1->setChecked(getBit(byte, 1));
            ui->cbByte11_2->setChecked(getBit(byte, 2));
            ui->cbByte11_3->setChecked(getBit(byte, 3));
            ui->cbByte11_4->setChecked(getBit(byte, 4));
            ui->cbByte11_5->setChecked(getBit(byte, 5));
            ui->cbByte11_6->setChecked(getBit(byte, 6));
            ui->cbByte11_7->setChecked(getBit(byte, 7));
            break;
        default:
            break;
    }
}

bool BeanPacketWidget::getBit(uint8_t *byte, int bitNumber) {
    return (*byte & (1 << bitNumber)) != 0;
}


void BeanPacketWidget::updateBytes() {
    for (uint8_t i = 0; i < 11; i++) {
        updateByte(i);
    }
}

void BeanPacketWidget::deviceSend() {
    // QString str;
    // if (isSending) {
    //     return;
    // }
    // isSending = true;
    // str = QString("%1,%2,%3,%4,")
    //         .arg(0) //.arg(ui->cbRetransmission->isChecked() ? 1 : 0)
    //         .arg(ui->cbPriority->currentIndex() + 1)
    //         .arg(ui->cbDestId->currentIndex())
    //         .arg(ui->cbMsgId->currentIndex());
    // auto bytes_count = (uint8_t) ui->sbBytesCount->value();

    // str.append(QString("%1,").arg(bytes_count));
    // for (uint8_t i = 0; i < bytes_count; i++) {
    //     str.append(QString("%1,").arg(bytes[i]));
    // }
    // str.append(char(10));
    // qDebug() << str;

    // QSerialPort *serial = MainWindow::getSerialPort();
    // serial->write(str.toStdString().c_str(), str.size());
    // serial->waitForBytesWritten(-1);
    // isSending = false;
}


void BeanPacketWidget::on_cbDestId_currentIndexChanged(int index) {
    if (beanPacket) {
        beanPacket->dstId = index;
    }
    updatePacketModel();
}

void BeanPacketWidget::on_cbMsgId_currentIndexChanged(int index) {
    if (beanPacket) {
        beanPacket->msgId = index;
    }
    updatePacketModel();
}

void BeanPacketWidget::on_cbPriority_currentIndexChanged(int index) {
    if (beanPacket) {
        beanPacket->prio = index;
    }
    updatePacketModel();
}

void BeanPacketWidget::on_sbBytesCount_valueChanged(int dlc) {
    if (beanPacket) {
        beanPacket->dlc = dlc;
    }
    updatePacketModel();
    refreshEnabled();
    refreshBytesVisible(dlc);
}

void BeanPacketWidget::refreshBytesVisible(int dlc) {
    //1
    ui->cbFill1->setVisible(1 <= dlc);
    ui->lbByte1->setVisible(1 <= dlc);
    ui->label_1->setVisible(1 <= dlc);
    ui->cbByte1_7->setVisible(1 <= dlc);
    ui->cbByte1_6->setVisible(1 <= dlc);
    ui->cbByte1_5->setVisible(1 <= dlc);
    ui->cbByte1_4->setVisible(1 <= dlc);
    ui->cbByte1_3->setVisible(1 <= dlc);
    ui->cbByte1_2->setVisible(1 <= dlc);
    ui->cbByte1_1->setVisible(1 <= dlc);
    ui->cbByte1_0->setVisible(1 <= dlc);
    //2
    ui->cbFill2->setVisible(2 <= dlc);
    ui->lbByte2->setVisible(2 <= dlc);
    ui->label_2->setVisible(2 <= dlc);
    ui->cbByte2_7->setVisible(2 <= dlc);
    ui->cbByte2_6->setVisible(2 <= dlc);
    ui->cbByte2_5->setVisible(2 <= dlc);
    ui->cbByte2_4->setVisible(2 <= dlc);
    ui->cbByte2_3->setVisible(2 <= dlc);
    ui->cbByte2_2->setVisible(2 <= dlc);
    ui->cbByte2_1->setVisible(2 <= dlc);
    ui->cbByte2_0->setVisible(2 <= dlc);
    //3
    ui->cbFill3->setVisible(3 <= dlc);
    ui->lbByte3->setVisible(3 <= dlc);
    ui->label_3->setVisible(3 <= dlc);
    ui->cbByte3_7->setVisible(3 <= dlc);
    ui->cbByte3_6->setVisible(3 <= dlc);
    ui->cbByte3_5->setVisible(3 <= dlc);
    ui->cbByte3_4->setVisible(3 <= dlc);
    ui->cbByte3_3->setVisible(3 <= dlc);
    ui->cbByte3_2->setVisible(3 <= dlc);
    ui->cbByte3_1->setVisible(3 <= dlc);
    ui->cbByte3_0->setVisible(3 <= dlc);
    //4
    ui->cbFill4->setVisible(4 <= dlc);
    ui->lbByte4->setVisible(4 <= dlc);
    ui->label_4->setVisible(4 <= dlc);
    ui->cbByte4_7->setVisible(4 <= dlc);
    ui->cbByte4_6->setVisible(4 <= dlc);
    ui->cbByte4_5->setVisible(4 <= dlc);
    ui->cbByte4_4->setVisible(4 <= dlc);
    ui->cbByte4_3->setVisible(4 <= dlc);
    ui->cbByte4_2->setVisible(4 <= dlc);
    ui->cbByte4_1->setVisible(4 <= dlc);
    ui->cbByte4_0->setVisible(4 <= dlc);
    //5
    ui->cbFill5->setVisible(5 <= dlc);
    ui->lbByte5->setVisible(5 <= dlc);
    ui->label_5->setVisible(5 <= dlc);
    ui->cbByte5_7->setVisible(5 <= dlc);
    ui->cbByte5_6->setVisible(5 <= dlc);
    ui->cbByte5_5->setVisible(5 <= dlc);
    ui->cbByte5_4->setVisible(5 <= dlc);
    ui->cbByte5_3->setVisible(5 <= dlc);
    ui->cbByte5_2->setVisible(5 <= dlc);
    ui->cbByte5_1->setVisible(5 <= dlc);
    ui->cbByte5_0->setVisible(5 <= dlc);
    //6
    ui->cbFill6->setVisible(6 <= dlc);
    ui->lbByte6->setVisible(6 <= dlc);
    ui->label_6->setVisible(6 <= dlc);
    ui->cbByte6_7->setVisible(6 <= dlc);
    ui->cbByte6_6->setVisible(6 <= dlc);
    ui->cbByte6_5->setVisible(6 <= dlc);
    ui->cbByte6_4->setVisible(6 <= dlc);
    ui->cbByte6_3->setVisible(6 <= dlc);
    ui->cbByte6_2->setVisible(6 <= dlc);
    ui->cbByte6_1->setVisible(6 <= dlc);
    ui->cbByte6_0->setVisible(6 <= dlc);

    //7
    ui->cbFill7->setVisible(7 <= dlc);
    ui->lbByte7->setVisible(7 <= dlc);
    ui->label_7->setVisible(7 <= dlc);
    ui->cbByte7_7->setVisible(7 <= dlc);
    ui->cbByte7_6->setVisible(7 <= dlc);
    ui->cbByte7_5->setVisible(7 <= dlc);
    ui->cbByte7_4->setVisible(7 <= dlc);
    ui->cbByte7_3->setVisible(7 <= dlc);
    ui->cbByte7_2->setVisible(7 <= dlc);
    ui->cbByte7_1->setVisible(7 <= dlc);
    ui->cbByte7_0->setVisible(7 <= dlc);

    //8
    ui->cbFill8->setVisible(8 <= dlc);
    ui->lbByte8->setVisible(8 <= dlc);
    ui->label_8->setVisible(8 <= dlc);
    ui->cbByte8_7->setVisible(8 <= dlc);
    ui->cbByte8_6->setVisible(8 <= dlc);
    ui->cbByte8_5->setVisible(8 <= dlc);
    ui->cbByte8_4->setVisible(8 <= dlc);
    ui->cbByte8_3->setVisible(8 <= dlc);
    ui->cbByte8_2->setVisible(8 <= dlc);
    ui->cbByte8_1->setVisible(8 <= dlc);
    ui->cbByte8_0->setVisible(8 <= dlc);
    //9
    ui->cbFill9->setVisible(9 <= dlc);
    ui->lbByte9->setVisible(9 <= dlc);
    ui->label_9->setVisible(9 <= dlc);
    ui->cbByte9_7->setVisible(9 <= dlc);
    ui->cbByte9_6->setVisible(9 <= dlc);
    ui->cbByte9_5->setVisible(9 <= dlc);
    ui->cbByte9_4->setVisible(9 <= dlc);
    ui->cbByte9_3->setVisible(9 <= dlc);
    ui->cbByte9_2->setVisible(9 <= dlc);
    ui->cbByte9_1->setVisible(9 <= dlc);
    ui->cbByte9_0->setVisible(9 <= dlc);
    //10
    ui->cbFill10->setVisible(10 <= dlc);
    ui->lbByte10->setVisible(10 <= dlc);
    ui->label_10->setVisible(10 <= dlc);
    ui->cbByte10_7->setVisible(10 <= dlc);
    ui->cbByte10_6->setVisible(10 <= dlc);
    ui->cbByte10_5->setVisible(10 <= dlc);
    ui->cbByte10_4->setVisible(10 <= dlc);
    ui->cbByte10_3->setVisible(10 <= dlc);
    ui->cbByte10_2->setVisible(10 <= dlc);
    ui->cbByte10_1->setVisible(10 <= dlc);
    ui->cbByte10_0->setVisible(10 <= dlc);

    //11
    ui->cbFill11->setVisible(11 <= dlc);
    ui->lbByte11->setVisible(11 <= dlc);
    ui->label_11->setVisible(11 <= dlc);
    ui->cbByte11_7->setVisible(11 <= dlc);
    ui->cbByte11_6->setVisible(11 <= dlc);
    ui->cbByte11_5->setVisible(11 <= dlc);
    ui->cbByte11_4->setVisible(11 <= dlc);
    ui->cbByte11_3->setVisible(11 <= dlc);
    ui->cbByte11_2->setVisible(11 <= dlc);
    ui->cbByte11_1->setVisible(11 <= dlc);
    ui->cbByte11_0->setVisible(11 <= dlc);
}

void BeanPacketWidget::on_btnSetBytes_clicked() {
    ui->cbByte1_7->setChecked(true);
    ui->cbByte1_6->setChecked(true);
    ui->cbByte1_5->setChecked(true);
    ui->cbByte1_4->setChecked(true);
    ui->cbByte1_3->setChecked(true);
    ui->cbByte1_2->setChecked(true);
    ui->cbByte1_1->setChecked(true);
    ui->cbByte1_0->setChecked(true);

    ui->cbByte2_7->setChecked(true);
    ui->cbByte2_6->setChecked(true);
    ui->cbByte2_5->setChecked(true);
    ui->cbByte2_4->setChecked(true);
    ui->cbByte2_3->setChecked(true);
    ui->cbByte2_2->setChecked(true);
    ui->cbByte2_1->setChecked(true);
    ui->cbByte2_0->setChecked(true);

    ui->cbByte3_7->setChecked(true);
    ui->cbByte3_6->setChecked(true);
    ui->cbByte3_5->setChecked(true);
    ui->cbByte3_4->setChecked(true);
    ui->cbByte3_3->setChecked(true);
    ui->cbByte3_2->setChecked(true);
    ui->cbByte3_1->setChecked(true);
    ui->cbByte3_0->setChecked(true);

    ui->cbByte4_7->setChecked(true);
    ui->cbByte4_6->setChecked(true);
    ui->cbByte4_5->setChecked(true);
    ui->cbByte4_4->setChecked(true);
    ui->cbByte4_3->setChecked(true);
    ui->cbByte4_2->setChecked(true);
    ui->cbByte4_1->setChecked(true);
    ui->cbByte4_0->setChecked(true);

    ui->cbByte5_7->setChecked(true);
    ui->cbByte5_6->setChecked(true);
    ui->cbByte5_5->setChecked(true);
    ui->cbByte5_4->setChecked(true);
    ui->cbByte5_3->setChecked(true);
    ui->cbByte5_2->setChecked(true);
    ui->cbByte5_1->setChecked(true);
    ui->cbByte5_0->setChecked(true);

    ui->cbByte6_7->setChecked(true);
    ui->cbByte6_6->setChecked(true);
    ui->cbByte6_5->setChecked(true);
    ui->cbByte6_4->setChecked(true);
    ui->cbByte6_3->setChecked(true);
    ui->cbByte6_2->setChecked(true);
    ui->cbByte6_1->setChecked(true);
    ui->cbByte6_0->setChecked(true);


    ui->cbByte7_7->setChecked(true);
    ui->cbByte7_6->setChecked(true);
    ui->cbByte7_5->setChecked(true);
    ui->cbByte7_4->setChecked(true);
    ui->cbByte7_3->setChecked(true);
    ui->cbByte7_2->setChecked(true);
    ui->cbByte7_1->setChecked(true);
    ui->cbByte7_0->setChecked(true);

    ui->cbByte8_7->setChecked(true);
    ui->cbByte8_6->setChecked(true);
    ui->cbByte8_5->setChecked(true);
    ui->cbByte8_4->setChecked(true);
    ui->cbByte8_3->setChecked(true);
    ui->cbByte8_2->setChecked(true);
    ui->cbByte8_1->setChecked(true);
    ui->cbByte8_0->setChecked(true);
    //
    ui->cbByte9_7->setChecked(true);
    ui->cbByte9_6->setChecked(true);
    ui->cbByte9_5->setChecked(true);
    ui->cbByte9_4->setChecked(true);
    ui->cbByte9_3->setChecked(true);
    ui->cbByte9_2->setChecked(true);
    ui->cbByte9_1->setChecked(true);
    ui->cbByte9_0->setChecked(true);
    //10
    ui->cbByte10_7->setChecked(true);
    ui->cbByte10_6->setChecked(true);
    ui->cbByte10_5->setChecked(true);
    ui->cbByte10_4->setChecked(true);
    ui->cbByte10_3->setChecked(true);
    ui->cbByte10_2->setChecked(true);
    ui->cbByte10_1->setChecked(true);
    ui->cbByte10_0->setChecked(true);
    //11
    ui->cbByte11_7->setChecked(true);
    ui->cbByte11_6->setChecked(true);
    ui->cbByte11_5->setChecked(true);
    ui->cbByte11_4->setChecked(true);
    ui->cbByte11_3->setChecked(true);
    ui->cbByte11_2->setChecked(true);
    ui->cbByte11_1->setChecked(true);
    ui->cbByte11_0->setChecked(true);
}

void BeanPacketWidget::on_btnSetBits1_clicked() {
    ui->cbByte1_7->setChecked(true);
    ui->cbByte1_6->setChecked(true);
    ui->cbByte1_5->setChecked(true);
    ui->cbByte1_4->setChecked(true);
    ui->cbByte1_3->setChecked(true);
    ui->cbByte1_2->setChecked(true);
    ui->cbByte1_1->setChecked(true);
    ui->cbByte1_0->setChecked(true);
}

void BeanPacketWidget::on_btnSetBits2_clicked() {
    ui->cbByte2_7->setChecked(true);
    ui->cbByte2_6->setChecked(true);
    ui->cbByte2_5->setChecked(true);
    ui->cbByte2_4->setChecked(true);
    ui->cbByte2_3->setChecked(true);
    ui->cbByte2_2->setChecked(true);
    ui->cbByte2_1->setChecked(true);
    ui->cbByte2_0->setChecked(true);
}

void BeanPacketWidget::on_btnSetBits3_clicked() {
    ui->cbByte3_7->setChecked(true);
    ui->cbByte3_6->setChecked(true);
    ui->cbByte3_5->setChecked(true);
    ui->cbByte3_4->setChecked(true);
    ui->cbByte3_3->setChecked(true);
    ui->cbByte3_2->setChecked(true);
    ui->cbByte3_1->setChecked(true);
    ui->cbByte3_0->setChecked(true);
}

void BeanPacketWidget::on_btnSetBits4_clicked() {
    ui->cbByte4_7->setChecked(true);
    ui->cbByte4_6->setChecked(true);
    ui->cbByte4_5->setChecked(true);
    ui->cbByte4_4->setChecked(true);
    ui->cbByte4_3->setChecked(true);
    ui->cbByte4_2->setChecked(true);
    ui->cbByte4_1->setChecked(true);
    ui->cbByte4_0->setChecked(true);
}

void BeanPacketWidget::on_btnSetBits5_clicked() {
    ui->cbByte5_7->setChecked(true);
    ui->cbByte5_6->setChecked(true);
    ui->cbByte5_5->setChecked(true);
    ui->cbByte5_4->setChecked(true);
    ui->cbByte5_3->setChecked(true);
    ui->cbByte5_2->setChecked(true);
    ui->cbByte5_1->setChecked(true);
    ui->cbByte5_0->setChecked(true);
}

void BeanPacketWidget::on_btnSetBits6_clicked() {
    ui->cbByte6_7->setChecked(true);
    ui->cbByte6_6->setChecked(true);
    ui->cbByte6_5->setChecked(true);
    ui->cbByte6_4->setChecked(true);
    ui->cbByte6_3->setChecked(true);
    ui->cbByte6_2->setChecked(true);
    ui->cbByte6_1->setChecked(true);
    ui->cbByte6_0->setChecked(true);
}

void BeanPacketWidget::on_btnSetBits7_clicked() {
    ui->cbByte7_7->setChecked(true);
    ui->cbByte7_6->setChecked(true);
    ui->cbByte7_5->setChecked(true);
    ui->cbByte7_4->setChecked(true);
    ui->cbByte7_3->setChecked(true);
    ui->cbByte7_2->setChecked(true);
    ui->cbByte7_1->setChecked(true);
    ui->cbByte7_0->setChecked(true);
}

void BeanPacketWidget::on_btnSetBits8_clicked() {
    ui->cbByte8_7->setChecked(true);
    ui->cbByte8_6->setChecked(true);
    ui->cbByte8_5->setChecked(true);
    ui->cbByte8_4->setChecked(true);
    ui->cbByte8_3->setChecked(true);
    ui->cbByte8_2->setChecked(true);
    ui->cbByte8_1->setChecked(true);
    ui->cbByte8_0->setChecked(true);
}

void BeanPacketWidget::on_btnSetBits9_clicked() {
    ui->cbByte9_7->setChecked(true);
    ui->cbByte9_6->setChecked(true);
    ui->cbByte9_5->setChecked(true);
    ui->cbByte9_4->setChecked(true);
    ui->cbByte9_3->setChecked(true);
    ui->cbByte9_2->setChecked(true);
    ui->cbByte9_1->setChecked(true);
    ui->cbByte9_0->setChecked(true);
}

void BeanPacketWidget::on_btnSetBits10_clicked() {
    ui->cbByte10_7->setChecked(true);
    ui->cbByte10_6->setChecked(true);
    ui->cbByte10_5->setChecked(true);
    ui->cbByte10_4->setChecked(true);
    ui->cbByte10_3->setChecked(true);
    ui->cbByte10_2->setChecked(true);
    ui->cbByte10_1->setChecked(true);
    ui->cbByte10_0->setChecked(true);
}

void BeanPacketWidget::on_btnSetBits11_clicked() {
    ui->cbByte11_7->setChecked(true);
    ui->cbByte11_6->setChecked(true);
    ui->cbByte11_5->setChecked(true);
    ui->cbByte11_4->setChecked(true);
    ui->cbByte11_3->setChecked(true);
    ui->cbByte11_2->setChecked(true);
    ui->cbByte11_1->setChecked(true);
    ui->cbByte11_0->setChecked(true);
}

void BeanPacketWidget::on_btnClearBits1_clicked() {
    ui->cbByte1_7->setChecked(false);
    ui->cbByte1_6->setChecked(false);
    ui->cbByte1_5->setChecked(false);
    ui->cbByte1_4->setChecked(false);
    ui->cbByte1_3->setChecked(false);
    ui->cbByte1_2->setChecked(false);
    ui->cbByte1_1->setChecked(false);
    ui->cbByte1_0->setChecked(false);
}

void BeanPacketWidget::on_btnClearBits2_clicked() {
    ui->cbByte2_7->setChecked(false);
    ui->cbByte2_6->setChecked(false);
    ui->cbByte2_5->setChecked(false);
    ui->cbByte2_4->setChecked(false);
    ui->cbByte2_3->setChecked(false);
    ui->cbByte2_2->setChecked(false);
    ui->cbByte2_1->setChecked(false);
    ui->cbByte2_0->setChecked(false);
}

void BeanPacketWidget::on_btnClearBits3_clicked() {
    ui->cbByte3_7->setChecked(false);
    ui->cbByte3_6->setChecked(false);
    ui->cbByte3_5->setChecked(false);
    ui->cbByte3_4->setChecked(false);
    ui->cbByte3_3->setChecked(false);
    ui->cbByte3_2->setChecked(false);
    ui->cbByte3_1->setChecked(false);
    ui->cbByte3_0->setChecked(false);
}

void BeanPacketWidget::on_btnClearBits4_clicked() {
    ui->cbByte4_7->setChecked(false);
    ui->cbByte4_6->setChecked(false);
    ui->cbByte4_5->setChecked(false);
    ui->cbByte4_4->setChecked(false);
    ui->cbByte4_3->setChecked(false);
    ui->cbByte4_2->setChecked(false);
    ui->cbByte4_1->setChecked(false);
    ui->cbByte4_0->setChecked(false);
}

void BeanPacketWidget::on_btnClearBits5_clicked() {
    ui->cbByte5_7->setChecked(false);
    ui->cbByte5_6->setChecked(false);
    ui->cbByte5_5->setChecked(false);
    ui->cbByte5_4->setChecked(false);
    ui->cbByte5_3->setChecked(false);
    ui->cbByte5_2->setChecked(false);
    ui->cbByte5_1->setChecked(false);
    ui->cbByte5_0->setChecked(false);
}

void BeanPacketWidget::on_btnClearBits6_clicked() {
    ui->cbByte6_7->setChecked(false);
    ui->cbByte6_6->setChecked(false);
    ui->cbByte6_5->setChecked(false);
    ui->cbByte6_4->setChecked(false);
    ui->cbByte6_3->setChecked(false);
    ui->cbByte6_2->setChecked(false);
    ui->cbByte6_1->setChecked(false);
    ui->cbByte6_0->setChecked(false);
}
void BeanPacketWidget::on_btnClearBits7_clicked() {
    ui->cbByte7_7->setChecked(false);
    ui->cbByte7_6->setChecked(false);
    ui->cbByte7_5->setChecked(false);
    ui->cbByte7_4->setChecked(false);
    ui->cbByte7_3->setChecked(false);
    ui->cbByte7_2->setChecked(false);
    ui->cbByte7_1->setChecked(false);
    ui->cbByte7_0->setChecked(false);
}
void BeanPacketWidget::on_btnClearBits8_clicked() {
    ui->cbByte8_7->setChecked(false);
    ui->cbByte8_6->setChecked(false);
    ui->cbByte8_5->setChecked(false);
    ui->cbByte8_4->setChecked(false);
    ui->cbByte8_3->setChecked(false);
    ui->cbByte8_2->setChecked(false);
    ui->cbByte8_1->setChecked(false);
    ui->cbByte8_0->setChecked(false);
}
void BeanPacketWidget::on_btnClearBits9_clicked() {
    ui->cbByte9_7->setChecked(false);
    ui->cbByte9_6->setChecked(false);
    ui->cbByte9_5->setChecked(false);
    ui->cbByte9_4->setChecked(false);
    ui->cbByte9_3->setChecked(false);
    ui->cbByte9_2->setChecked(false);
    ui->cbByte9_1->setChecked(false);
    ui->cbByte9_0->setChecked(false);
}
void BeanPacketWidget::on_btnClearBits10_clicked() {
    ui->cbByte10_7->setChecked(false);
    ui->cbByte10_6->setChecked(false);
    ui->cbByte10_5->setChecked(false);
    ui->cbByte10_4->setChecked(false);
    ui->cbByte10_3->setChecked(false);
    ui->cbByte10_2->setChecked(false);
    ui->cbByte10_1->setChecked(false);
    ui->cbByte10_0->setChecked(false);
}
void BeanPacketWidget::on_btnClearBits11_clicked() {
    ui->cbByte11_7->setChecked(false);
    ui->cbByte11_6->setChecked(false);
    ui->cbByte11_5->setChecked(false);
    ui->cbByte11_4->setChecked(false);
    ui->cbByte11_3->setChecked(false);
    ui->cbByte11_2->setChecked(false);
    ui->cbByte11_1->setChecked(false);
    ui->cbByte11_0->setChecked(false);
}

void BeanPacketWidget::on_cbByte1_7_toggled(bool checked) {
    CB_TOGGLE(0, 7, checked);
    updateByte(0);
}

void BeanPacketWidget::on_cbByte1_6_toggled(bool checked) {
    CB_TOGGLE(0, 6, checked);
    updateByte(0);
}

void BeanPacketWidget::on_cbByte1_5_toggled(bool checked) {
    CB_TOGGLE(0, 5, checked);
    updateByte(0);
}

void BeanPacketWidget::on_cbByte1_4_toggled(bool checked) {
    CB_TOGGLE(0, 4, checked);
    updateByte(0);
}

void BeanPacketWidget::on_cbByte1_3_toggled(bool checked) {
    CB_TOGGLE(0, 3, checked);
    updateByte(0);
}

void BeanPacketWidget::on_cbByte1_2_toggled(bool checked) {
    CB_TOGGLE(0, 2, checked);
    updateByte(0);
}

void BeanPacketWidget::on_cbByte1_1_toggled(bool checked) {
    CB_TOGGLE(0, 1, checked);
    updateByte(0);
}

void BeanPacketWidget::on_cbByte1_0_toggled(bool checked) {
    CB_TOGGLE(0, 0, checked);
    updateByte(0);
}

void BeanPacketWidget::on_cbByte2_7_toggled(bool checked) {
    CB_TOGGLE(1, 7, checked);
    updateByte(1);
}

void BeanPacketWidget::on_cbByte2_6_toggled(bool checked) {
    CB_TOGGLE(1, 6, checked);
    updateByte(1);
}

void BeanPacketWidget::on_cbByte2_5_toggled(bool checked) {
    CB_TOGGLE(1, 5, checked);
    updateByte(1);
}

void BeanPacketWidget::on_cbByte2_4_toggled(bool checked) {
    CB_TOGGLE(1, 4, checked);
    updateByte(1);
}

void BeanPacketWidget::on_cbByte2_3_toggled(bool checked) {
    CB_TOGGLE(1, 3, checked);
    updateByte(1);
}

void BeanPacketWidget::on_cbByte2_2_toggled(bool checked) {
    CB_TOGGLE(1, 2, checked);
    updateByte(1);
}

void BeanPacketWidget::on_cbByte2_1_toggled(bool checked) {
    CB_TOGGLE(1, 1, checked);
    updateByte(1);
}

void BeanPacketWidget::on_cbByte2_0_toggled(bool checked) {
    CB_TOGGLE(1, 0, checked);
    updateByte(1);
}

void BeanPacketWidget::on_cbByte3_7_toggled(bool checked) {
    CB_TOGGLE(2, 7, checked);
    updateByte(2);
}

void BeanPacketWidget::on_cbByte3_6_toggled(bool checked) {
    CB_TOGGLE(2, 6, checked);
    updateByte(2);
}

void BeanPacketWidget::on_cbByte3_5_toggled(bool checked) {
    CB_TOGGLE(2, 5, checked);
    updateByte(2);
}

void BeanPacketWidget::on_cbByte3_4_toggled(bool checked) {
    CB_TOGGLE(2, 4, checked);
    updateByte(2);
}

void BeanPacketWidget::on_cbByte3_3_toggled(bool checked) {
    CB_TOGGLE(2, 3, checked);
    updateByte(2);
}

void BeanPacketWidget::on_cbByte3_2_toggled(bool checked) {
    CB_TOGGLE(2, 2, checked);
    updateByte(2);
}

void BeanPacketWidget::on_cbByte3_1_toggled(bool checked) {
    CB_TOGGLE(2, 1, checked);
    updateByte(2);
}

void BeanPacketWidget::on_cbByte3_0_toggled(bool checked) {
    CB_TOGGLE(2, 0, checked);
    updateByte(2);
}

void BeanPacketWidget::on_cbByte4_7_toggled(bool checked) {
    CB_TOGGLE(3, 7, checked);
    updateByte(3);
}

void BeanPacketWidget::on_cbByte4_6_toggled(bool checked) {
    CB_TOGGLE(3, 6, checked);
    updateByte(3);
}

void BeanPacketWidget::on_cbByte4_5_toggled(bool checked) {
    CB_TOGGLE(3, 5, checked);
    updateByte(3);
}

void BeanPacketWidget::on_cbByte4_4_toggled(bool checked) {
    CB_TOGGLE(3, 4, checked);
    updateByte(3);
}

void BeanPacketWidget::on_cbByte4_3_toggled(bool checked) {
    CB_TOGGLE(3, 3, checked);
    updateByte(3);
}

void BeanPacketWidget::on_cbByte4_2_toggled(bool checked) {
    CB_TOGGLE(3, 2, checked);
    updateByte(3);
}

void BeanPacketWidget::on_cbByte4_1_toggled(bool checked) {
    CB_TOGGLE(3, 1, checked);
    updateByte(3);
}

void BeanPacketWidget::on_cbByte4_0_toggled(bool checked) {
    CB_TOGGLE(3, 0, checked);
    updateByte(3);
}

// 5 ui
void BeanPacketWidget::on_cbByte5_7_toggled(bool checked) {
    CB_TOGGLE(4, 7, checked);
    updateByte(4);
}

void BeanPacketWidget::on_cbByte5_6_toggled(bool checked) {
    CB_TOGGLE(4, 6, checked);
    updateByte(4);
}

void BeanPacketWidget::on_cbByte5_5_toggled(bool checked) {
    CB_TOGGLE(4, 5, checked);
    updateByte(4);
}

void BeanPacketWidget::on_cbByte5_4_toggled(bool checked) {
    CB_TOGGLE(4, 4, checked);
    updateByte(4);
}

void BeanPacketWidget::on_cbByte5_3_toggled(bool checked) {
    CB_TOGGLE(4, 3, checked);
    updateByte(4);
}

void BeanPacketWidget::on_cbByte5_2_toggled(bool checked) {
    CB_TOGGLE(4, 2, checked);
    updateByte(4);
}

void BeanPacketWidget::on_cbByte5_1_toggled(bool checked) {
    CB_TOGGLE(4, 1, checked);
    updateByte(4);
}

void BeanPacketWidget::on_cbByte5_0_toggled(bool checked) {
    CB_TOGGLE(4, 0, checked);
    updateByte(4);
}

// 6 ui
void BeanPacketWidget::on_cbByte6_7_toggled(bool checked) {
    CB_TOGGLE(5, 7, checked);
    updateByte(5);
}

void BeanPacketWidget::on_cbByte6_6_toggled(bool checked) {
    CB_TOGGLE(5, 6, checked);
    updateByte(5);
}

void BeanPacketWidget::on_cbByte6_5_toggled(bool checked) {
    CB_TOGGLE(5, 5, checked);
    updateByte(5);
}

void BeanPacketWidget::on_cbByte6_4_toggled(bool checked) {
    CB_TOGGLE(5, 4, checked);
    updateByte(5);
}

void BeanPacketWidget::on_cbByte6_3_toggled(bool checked) {
    CB_TOGGLE(5, 3, checked);
    updateByte(5);
}

void BeanPacketWidget::on_cbByte6_2_toggled(bool checked) {
    CB_TOGGLE(5, 2, checked);
    updateByte(5);
}

void BeanPacketWidget::on_cbByte6_1_toggled(bool checked) {
    CB_TOGGLE(5, 1, checked);
    updateByte(5);
}

void BeanPacketWidget::on_cbByte6_0_toggled(bool checked) {
    CB_TOGGLE(5, 0, checked);
    updateByte(5);
}

// 7 ui
void BeanPacketWidget::on_cbByte7_7_toggled(bool checked) {
    CB_TOGGLE(6, 7, checked);
    updateByte(6);
}

void BeanPacketWidget::on_cbByte7_6_toggled(bool checked) {
    CB_TOGGLE(6, 6, checked);
    updateByte(6);
}

void BeanPacketWidget::on_cbByte7_5_toggled(bool checked) {
    CB_TOGGLE(6, 5, checked);
    updateByte(6);
}

void BeanPacketWidget::on_cbByte7_4_toggled(bool checked) {
    CB_TOGGLE(6, 4, checked);
    updateByte(6);
}

void BeanPacketWidget::on_cbByte7_3_toggled(bool checked) {
    CB_TOGGLE(6, 3, checked);
    updateByte(6);
}

void BeanPacketWidget::on_cbByte7_2_toggled(bool checked) {
    CB_TOGGLE(6, 2, checked);
    updateByte(6);
}

void BeanPacketWidget::on_cbByte7_1_toggled(bool checked) {
    CB_TOGGLE(6, 1, checked);
    updateByte(6);
}

void BeanPacketWidget::on_cbByte7_0_toggled(bool checked) {
    CB_TOGGLE(6, 0, checked);
    updateByte(6);
}

//8
void BeanPacketWidget::on_cbByte8_7_toggled(bool checked) {
    CB_TOGGLE(7, 7, checked);
    updateByte(7);
}

void BeanPacketWidget::on_cbByte8_6_toggled(bool checked) {
    CB_TOGGLE(7, 6, checked);
    updateByte(7);
}

void BeanPacketWidget::on_cbByte8_5_toggled(bool checked) {
    CB_TOGGLE(7, 5, checked);
    updateByte(7);
}

void BeanPacketWidget::on_cbByte8_4_toggled(bool checked) {
    CB_TOGGLE(7, 4, checked);
    updateByte(7);
}

void BeanPacketWidget::on_cbByte8_3_toggled(bool checked) {
    CB_TOGGLE(7, 3, checked);
    updateByte(7);
}

void BeanPacketWidget::on_cbByte8_2_toggled(bool checked) {
    CB_TOGGLE(7, 2, checked);
    updateByte(7);
}

void BeanPacketWidget::on_cbByte8_1_toggled(bool checked) {
    CB_TOGGLE(7, 1, checked);
    updateByte(7);
}

void BeanPacketWidget::on_cbByte8_0_toggled(bool checked) {
    CB_TOGGLE(7, 0, checked);
    updateByte(7);
}

//9
void BeanPacketWidget::on_cbByte9_7_toggled(bool checked) {
    CB_TOGGLE(8, 7, checked);
    updateByte(8);
}

void BeanPacketWidget::on_cbByte9_6_toggled(bool checked) {
    CB_TOGGLE(8, 6, checked);
    updateByte(8);
}

void BeanPacketWidget::on_cbByte9_5_toggled(bool checked) {
    CB_TOGGLE(8, 5, checked);
    updateByte(8);
}

void BeanPacketWidget::on_cbByte9_4_toggled(bool checked) {
    CB_TOGGLE(8, 4, checked);
    updateByte(8);
}

void BeanPacketWidget::on_cbByte9_3_toggled(bool checked) {
    CB_TOGGLE(8, 3, checked);
    updateByte(8);
}

void BeanPacketWidget::on_cbByte9_2_toggled(bool checked) {
    CB_TOGGLE(8, 2, checked);
    updateByte(8);
}

void BeanPacketWidget::on_cbByte9_1_toggled(bool checked) {
    CB_TOGGLE(8, 1, checked);
    updateByte(8);
}

void BeanPacketWidget::on_cbByte9_0_toggled(bool checked) {
    CB_TOGGLE(8, 0, checked);
    updateByte(8);
}

//10ui
void BeanPacketWidget::on_cbByte10_7_toggled(bool checked) {
    CB_TOGGLE(9, 7, checked);
    updateByte(9);
}

void BeanPacketWidget::on_cbByte10_6_toggled(bool checked) {
    CB_TOGGLE(9, 6, checked);
    updateByte(9);
}

void BeanPacketWidget::on_cbByte10_5_toggled(bool checked) {
    CB_TOGGLE(9, 5, checked);
    updateByte(9);
}

void BeanPacketWidget::on_cbByte10_4_toggled(bool checked) {
    CB_TOGGLE(9, 4, checked);
    updateByte(9);
}

void BeanPacketWidget::on_cbByte10_3_toggled(bool checked) {
    CB_TOGGLE(9, 3, checked);
    updateByte(9);
}

void BeanPacketWidget::on_cbByte10_2_toggled(bool checked) {
    CB_TOGGLE(9, 2, checked);
    updateByte(9);
}

void BeanPacketWidget::on_cbByte10_1_toggled(bool checked) {
    CB_TOGGLE(9, 1, checked);
    updateByte(9);
}

void BeanPacketWidget::on_cbByte10_0_toggled(bool checked) {
    CB_TOGGLE(9, 0, checked);
    updateByte(9);
}

// 11 ui
void BeanPacketWidget::on_cbByte11_7_toggled(bool checked) {
    CB_TOGGLE(10, 7, checked);
    updateByte(10);
}

void BeanPacketWidget::on_cbByte11_6_toggled(bool checked) {
    CB_TOGGLE(10, 6, checked);
    updateByte(10);
}

void BeanPacketWidget::on_cbByte11_5_toggled(bool checked) {
    CB_TOGGLE(10, 5, checked);
    updateByte(10);
}

void BeanPacketWidget::on_cbByte11_4_toggled(bool checked) {
    CB_TOGGLE(10, 4, checked);
    updateByte(10);
}

void BeanPacketWidget::on_cbByte11_3_toggled(bool checked) {
    CB_TOGGLE(10, 3, checked);
    updateByte(10);
}

void BeanPacketWidget::on_cbByte11_2_toggled(bool checked) {
    CB_TOGGLE(10, 2, checked);
    updateByte(10);
}

void BeanPacketWidget::on_cbByte11_1_toggled(bool checked) {
    CB_TOGGLE(10, 1, checked);
    updateByte(10);
}

void BeanPacketWidget::on_cbByte11_0_toggled(bool checked) {
    CB_TOGGLE(10, 0, checked);
    updateByte(10);
}

void BeanPacketWidget::on_btnClearBytes_clicked() {
    // 1 ui byte, byte[0]
    ui->cbByte1_7->setChecked(false);
    ui->cbByte1_6->setChecked(false);
    ui->cbByte1_5->setChecked(false);
    ui->cbByte1_4->setChecked(false);
    ui->cbByte1_3->setChecked(false);
    ui->cbByte1_2->setChecked(false);
    ui->cbByte1_1->setChecked(false);
    ui->cbByte1_0->setChecked(false);

    ui->cbByte2_7->setChecked(false);
    ui->cbByte2_6->setChecked(false);
    ui->cbByte2_5->setChecked(false);
    ui->cbByte2_4->setChecked(false);
    ui->cbByte2_3->setChecked(false);
    ui->cbByte2_2->setChecked(false);
    ui->cbByte2_1->setChecked(false);
    ui->cbByte2_0->setChecked(false);

    ui->cbByte3_7->setChecked(false);
    ui->cbByte3_6->setChecked(false);
    ui->cbByte3_5->setChecked(false);
    ui->cbByte3_4->setChecked(false);
    ui->cbByte3_3->setChecked(false);
    ui->cbByte3_2->setChecked(false);
    ui->cbByte3_1->setChecked(false);
    ui->cbByte3_0->setChecked(false);

    ui->cbByte4_7->setChecked(false);
    ui->cbByte4_6->setChecked(false);
    ui->cbByte4_5->setChecked(false);
    ui->cbByte4_4->setChecked(false);
    ui->cbByte4_3->setChecked(false);
    ui->cbByte4_2->setChecked(false);
    ui->cbByte4_1->setChecked(false);
    ui->cbByte4_0->setChecked(false);

    ui->cbByte5_7->setChecked(false);
    ui->cbByte5_6->setChecked(false);
    ui->cbByte5_5->setChecked(false);
    ui->cbByte5_4->setChecked(false);
    ui->cbByte5_3->setChecked(false);
    ui->cbByte5_2->setChecked(false);
    ui->cbByte5_1->setChecked(false);
    ui->cbByte5_0->setChecked(false);

    ui->cbByte6_7->setChecked(false);
    ui->cbByte6_6->setChecked(false);
    ui->cbByte6_5->setChecked(false);
    ui->cbByte6_4->setChecked(false);
    ui->cbByte6_3->setChecked(false);
    ui->cbByte6_2->setChecked(false);
    ui->cbByte6_1->setChecked(false);
    ui->cbByte6_0->setChecked(false);

    ui->cbByte7_7->setChecked(false);
    ui->cbByte7_6->setChecked(false);
    ui->cbByte7_5->setChecked(false);
    ui->cbByte7_4->setChecked(false);
    ui->cbByte7_3->setChecked(false);
    ui->cbByte7_2->setChecked(false);
    ui->cbByte7_1->setChecked(false);
    ui->cbByte7_0->setChecked(false);
    //8
    ui->cbByte8_7->setChecked(false);
    ui->cbByte8_6->setChecked(false);
    ui->cbByte8_5->setChecked(false);
    ui->cbByte8_4->setChecked(false);
    ui->cbByte8_3->setChecked(false);
    ui->cbByte8_2->setChecked(false);
    ui->cbByte8_1->setChecked(false);
    ui->cbByte8_0->setChecked(false);
    //9
    ui->cbByte9_7->setChecked(false);
    ui->cbByte9_6->setChecked(false);
    ui->cbByte9_5->setChecked(false);
    ui->cbByte9_4->setChecked(false);
    ui->cbByte9_3->setChecked(false);
    ui->cbByte9_2->setChecked(false);
    ui->cbByte9_1->setChecked(false);
    ui->cbByte9_0->setChecked(false);
    //10
    ui->cbByte10_7->setChecked(false);
    ui->cbByte10_6->setChecked(false);
    ui->cbByte10_5->setChecked(false);
    ui->cbByte10_4->setChecked(false);
    ui->cbByte10_3->setChecked(false);
    ui->cbByte10_2->setChecked(false);
    ui->cbByte10_1->setChecked(false);
    ui->cbByte10_0->setChecked(false);
    //11
    ui->cbByte11_7->setChecked(false);
    ui->cbByte11_6->setChecked(false);
    ui->cbByte11_5->setChecked(false);
    ui->cbByte11_4->setChecked(false);
    ui->cbByte11_3->setChecked(false);
    ui->cbByte11_2->setChecked(false);
    ui->cbByte11_1->setChecked(false);
    ui->cbByte11_0->setChecked(false);
}

void BeanPacketWidget::reloadPacket() {
    loadPacket();
}

void BeanPacketWidget::refreshEnabled() {
    bool enabled = false;
    if (beanPacket) {
        enabled = !beanPacket->ro;
    }

    ui->cbDestId->setEnabled(enabled);
    ui->sbBytesCount->setEnabled(enabled);
    ui->cbMsgId->setEnabled(enabled);
    ui->cbPriority->setEnabled(enabled);

    ui->btnSetBytes->setEnabled(enabled);
    ui->btnClearBytes->setEnabled(enabled);

    //1
    ui->cbFill1->setEnabled(enabled);
    ui->lbByte1->setEnabled(enabled);
    ui->label_1->setEnabled(enabled);
    ui->cbByte1_7->setEnabled(enabled);
    ui->cbByte1_6->setEnabled(enabled);
    ui->cbByte1_5->setEnabled(enabled);
    ui->cbByte1_4->setEnabled(enabled);
    ui->cbByte1_3->setEnabled(enabled);
    ui->cbByte1_2->setEnabled(enabled);
    ui->cbByte1_1->setEnabled(enabled);
    ui->cbByte1_0->setEnabled(enabled);

    //2
    ui->cbFill2->setEnabled(enabled);
    ui->lbByte2->setEnabled(enabled);
    ui->label_2->setEnabled(enabled);
    ui->cbByte2_7->setEnabled(enabled);
    ui->cbByte2_6->setEnabled(enabled);
    ui->cbByte2_5->setEnabled(enabled);
    ui->cbByte2_4->setEnabled(enabled);
    ui->cbByte2_3->setEnabled(enabled);
    ui->cbByte2_2->setEnabled(enabled);
    ui->cbByte2_1->setEnabled(enabled);
    ui->cbByte2_0->setEnabled(enabled);
    //3
    ui->cbFill3->setEnabled(enabled);
    ui->lbByte3->setEnabled(enabled);
    ui->label_3->setEnabled(enabled);
    ui->cbByte3_7->setEnabled(enabled);
    ui->cbByte3_6->setEnabled(enabled);
    ui->cbByte3_5->setEnabled(enabled);
    ui->cbByte3_4->setEnabled(enabled);
    ui->cbByte3_3->setEnabled(enabled);
    ui->cbByte3_2->setEnabled(enabled);
    ui->cbByte3_1->setEnabled(enabled);
    ui->cbByte3_0->setEnabled(enabled);
    //4
    ui->cbFill4->setEnabled(enabled);
    ui->lbByte4->setEnabled(enabled);
    ui->label_4->setEnabled(enabled);
    ui->cbByte4_7->setEnabled(enabled);
    ui->cbByte4_6->setEnabled(enabled);
    ui->cbByte4_5->setEnabled(enabled);
    ui->cbByte4_4->setEnabled(enabled);
    ui->cbByte4_3->setEnabled(enabled);
    ui->cbByte4_2->setEnabled(enabled);
    ui->cbByte4_1->setEnabled(enabled);
    ui->cbByte4_0->setEnabled(enabled);
    //5
    ui->cbFill5->setEnabled(enabled);
    ui->lbByte5->setEnabled(enabled);
    ui->label_5->setEnabled(enabled);
    ui->cbByte5_7->setEnabled(enabled);
    ui->cbByte5_6->setEnabled(enabled);
    ui->cbByte5_5->setEnabled(enabled);
    ui->cbByte5_4->setEnabled(enabled);
    ui->cbByte5_3->setEnabled(enabled);
    ui->cbByte5_2->setEnabled(enabled);
    ui->cbByte5_1->setEnabled(enabled);
    ui->cbByte5_0->setEnabled(enabled);
    //6
    ui->cbFill6->setEnabled(enabled);
    ui->lbByte6->setEnabled(enabled);
    ui->label_6->setEnabled(enabled);
    ui->cbByte6_7->setEnabled(enabled);
    ui->cbByte6_6->setEnabled(enabled);
    ui->cbByte6_5->setEnabled(enabled);
    ui->cbByte6_4->setEnabled(enabled);
    ui->cbByte6_3->setEnabled(enabled);
    ui->cbByte6_2->setEnabled(enabled);
    ui->cbByte6_1->setEnabled(enabled);
    ui->cbByte6_0->setEnabled(enabled);

    //7
    ui->cbFill7->setEnabled(enabled);
    ui->lbByte7->setEnabled(enabled);
    ui->label_7->setEnabled(enabled);
    ui->cbByte7_7->setEnabled(enabled);
    ui->cbByte7_6->setEnabled(enabled);
    ui->cbByte7_5->setEnabled(enabled);
    ui->cbByte7_4->setEnabled(enabled);
    ui->cbByte7_3->setEnabled(enabled);
    ui->cbByte7_2->setEnabled(enabled);
    ui->cbByte7_1->setEnabled(enabled);
    ui->cbByte7_0->setEnabled(enabled);

    //8
    ui->cbFill8->setEnabled(enabled);
    ui->lbByte8->setEnabled(enabled);
    ui->label_8->setEnabled(enabled);
    ui->cbByte8_7->setEnabled(enabled);
    ui->cbByte8_6->setEnabled(enabled);
    ui->cbByte8_5->setEnabled(enabled);
    ui->cbByte8_4->setEnabled(enabled);
    ui->cbByte8_3->setEnabled(enabled);
    ui->cbByte8_2->setEnabled(enabled);
    ui->cbByte8_1->setEnabled(enabled);
    ui->cbByte8_0->setEnabled(enabled);
    //9
    ui->cbFill9->setEnabled(enabled);
    ui->lbByte9->setEnabled(enabled);
    ui->label_9->setEnabled(enabled);
    ui->cbByte9_7->setEnabled(enabled);
    ui->cbByte9_6->setEnabled(enabled);
    ui->cbByte9_5->setEnabled(enabled);
    ui->cbByte9_4->setEnabled(enabled);
    ui->cbByte9_3->setEnabled(enabled);
    ui->cbByte9_2->setEnabled(enabled);
    ui->cbByte9_1->setEnabled(enabled);
    ui->cbByte9_0->setEnabled(enabled);
    //10
    ui->cbFill10->setEnabled(enabled);
    ui->lbByte10->setEnabled(enabled);
    ui->label_10->setEnabled(enabled);
    ui->cbByte10_7->setEnabled(enabled);
    ui->cbByte10_6->setEnabled(enabled);
    ui->cbByte10_5->setEnabled(enabled);
    ui->cbByte10_4->setEnabled(enabled);
    ui->cbByte10_3->setEnabled(enabled);
    ui->cbByte10_2->setEnabled(enabled);
    ui->cbByte10_1->setEnabled(enabled);
    ui->cbByte10_0->setEnabled(enabled);

    //11
    ui->cbFill11->setEnabled(enabled);
    ui->lbByte11->setEnabled(enabled);
    ui->label_11->setEnabled(enabled);
    ui->cbByte11_7->setEnabled(enabled);
    ui->cbByte11_6->setEnabled(enabled);
    ui->cbByte11_5->setEnabled(enabled);
    ui->cbByte11_4->setEnabled(enabled);
    ui->cbByte11_3->setEnabled(enabled);
    ui->cbByte11_2->setEnabled(enabled);
    ui->cbByte11_1->setEnabled(enabled);
    ui->cbByte11_0->setEnabled(enabled);
}


void BeanPacketWidget::on_cbFill1_stateChanged(int arg1) {
    switch (arg1) {
        case 0: // unhecked
            on_btnClearBits1_clicked();
            break;
        case 2:
            on_btnSetBits1_clicked();
        default:
            return;
    }
}

void BeanPacketWidget::on_cbFill2_stateChanged(int arg1) {
    switch (arg1) {
        case 0: // unhecked
            on_btnClearBits2_clicked();
            break;
        case 2:
            on_btnSetBits2_clicked();
        default:
            return;
    }
}

void BeanPacketWidget::on_cbFill3_stateChanged(int arg1) {
    switch (arg1) {
        case 0: // unhecked
            on_btnClearBits3_clicked();
            break;
        case 2:
            on_btnSetBits3_clicked();
        default:
            return;
    }
}

void BeanPacketWidget::on_cbFill4_stateChanged(int arg1) {
    switch (arg1) {
        case 0: // unhecked
            on_btnClearBits4_clicked();
            break;
        case 2:
            on_btnSetBits4_clicked();
        default:
            return;
    }
}

void BeanPacketWidget::on_cbFill5_stateChanged(int arg1) {
    switch (arg1) {
        case 0: // unhecked
            on_btnClearBits5_clicked();
            break;
        case 2:
            on_btnSetBits5_clicked();
        default:
            return;
    }
}


void BeanPacketWidget::on_cbFill6_stateChanged(int arg1) {
    switch (arg1) {
        case 0: // unhecked
            on_btnClearBits6_clicked();
            break;
        case 2:
            on_btnSetBits6_clicked();
        default:
            return;
    }
}

void BeanPacketWidget::on_cbFill7_stateChanged(int arg1) {
    switch (arg1) {
        case 0: // unhecked
            on_btnClearBits7_clicked();
            break;
        case 2:
            on_btnSetBits7_clicked();
        default:
            return;
    }
}

void BeanPacketWidget::on_cbFill8_stateChanged(int arg1) {
    switch (arg1) {
        case 0: // unhecked
            on_btnClearBits8_clicked();
            break;
        case 2:
            on_btnSetBits8_clicked();
        default:
            return;
    }
}

void BeanPacketWidget::on_cbFill9_stateChanged(int arg1) {
    switch (arg1) {
        case 0: // unhecked
            on_btnClearBits9_clicked();
            break;
        case 2:
            on_btnSetBits9_clicked();
        default:
            return;
    }
}

void BeanPacketWidget::on_cbFill10_stateChanged(int arg1) {
    switch (arg1) {
        case 0: // unhecked
            on_btnClearBits10_clicked();
            break;
        case 2:
            on_btnSetBits10_clicked();
        default:
            return;
    }
}

void BeanPacketWidget::on_cbFill11_stateChanged(int arg1) {
    switch (arg1) {
        case 0: // unhecked
            on_btnClearBits11_clicked();
            break;
        case 2:
            on_btnSetBits11_clicked();
        default:
            return;
    }
}
