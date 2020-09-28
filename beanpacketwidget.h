#ifndef BEANPACKETWIDGET_H
#define BEANPACKETWIDGET_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
    class BeanPacketWidget;
}

class BeanPacketWidget : public QWidget {
Q_OBJECT


public:
    explicit BeanPacketWidget(QWidget *parent = nullptr);

    ~BeanPacketWidget() override;

    void reloadPacket();

    void setPacket(BeanPacket *packet, QAbstractTableModel *model);

    void fillPriority();

    void fillDestIds();

    void fillMessageIds();

    void updateByte(uint8_t byte_num);

    void updateBytes();

    void deviceSend();

//    bool isSerialConnected();
    static bool isLiveSend();


private:
    Ui::BeanPacketWidget *ui;
    BeanPacket *beanPacket; // points to packet that this widget is displaying/modifying
    QAbstractTableModel *model; // model that contains packet. Needs notification.
    void updateBits(uint8_t byte_num);

    static bool getBit(uint8_t *byte, int bitNum);

private slots:

    void loadPacket();

    void on_cbPriority_currentIndexChanged(int index);

    void on_cbDestId_currentIndexChanged(int index);

    void on_cbMsgId_currentIndexChanged(int index);

    void on_sbBytesCount_valueChanged(int arg1);
//    void on_cbRetransmission_toggled(bool checked);

//   void on_btnSendBytes_clicked();
//    void on_cbLiveSend_toggled(bool checked);

// 1 (human) byte on ui
    void on_cbByte1_7_toggled(bool checked);

    void on_cbByte1_6_toggled(bool checked);

    void on_cbByte1_5_toggled(bool checked);

    void on_cbByte1_4_toggled(bool checked);

    void on_cbByte1_3_toggled(bool checked);

    void on_cbByte1_2_toggled(bool checked);

    void on_cbByte1_1_toggled(bool checked);

    void on_cbByte1_0_toggled(bool checked);

//
    void on_cbByte2_7_toggled(bool checked);

    void on_cbByte2_6_toggled(bool checked);

    void on_cbByte2_5_toggled(bool checked);

    void on_cbByte2_4_toggled(bool checked);

    void on_cbByte2_3_toggled(bool checked);

    void on_cbByte2_2_toggled(bool checked);

    void on_cbByte2_1_toggled(bool checked);

    void on_cbByte2_0_toggled(bool checked);

//
    void on_cbByte3_7_toggled(bool checked);

    void on_cbByte3_6_toggled(bool checked);

    void on_cbByte3_5_toggled(bool checked);

    void on_cbByte3_4_toggled(bool checked);

    void on_cbByte3_3_toggled(bool checked);

    void on_cbByte3_2_toggled(bool checked);

    void on_cbByte3_1_toggled(bool checked);

//
    void on_cbByte3_0_toggled(bool checked);

    void on_cbByte4_7_toggled(bool checked);

    void on_cbByte4_6_toggled(bool checked);

    void on_cbByte4_5_toggled(bool checked);

    void on_cbByte4_4_toggled(bool checked);

    void on_cbByte4_3_toggled(bool checked);

    void on_cbByte4_2_toggled(bool checked);

    void on_cbByte4_1_toggled(bool checked);

//
    void on_cbByte4_0_toggled(bool checked);

    void on_cbByte5_7_toggled(bool checked);

    void on_cbByte5_6_toggled(bool checked);

    void on_cbByte5_5_toggled(bool checked);

    void on_cbByte5_4_toggled(bool checked);

    void on_cbByte5_3_toggled(bool checked);

    void on_cbByte5_2_toggled(bool checked);

    void on_cbByte5_1_toggled(bool checked);

    void on_cbByte5_0_toggled(bool checked);

//
    void on_cbByte6_7_toggled(bool checked);

    void on_cbByte6_6_toggled(bool checked);

    void on_cbByte6_5_toggled(bool checked);

    void on_cbByte6_4_toggled(bool checked);

    void on_cbByte6_3_toggled(bool checked);

    void on_cbByte6_2_toggled(bool checked);

    void on_cbByte6_1_toggled(bool checked);

    void on_cbByte6_0_toggled(bool checked);

//
    void on_cbByte7_0_toggled(bool checked);

    void on_cbByte7_1_toggled(bool checked);

    void on_cbByte7_2_toggled(bool checked);

    void on_cbByte7_3_toggled(bool checked);

    void on_cbByte7_4_toggled(bool checked);

    void on_cbByte7_5_toggled(bool checked);

    void on_cbByte7_6_toggled(bool checked);

    void on_cbByte7_7_toggled(bool checked);

//
    void on_cbByte8_0_toggled(bool checked);

    void on_cbByte8_1_toggled(bool checked);

    void on_cbByte8_2_toggled(bool checked);

    void on_cbByte8_3_toggled(bool checked);

    void on_cbByte8_4_toggled(bool checked);

    void on_cbByte8_5_toggled(bool checked);

    void on_cbByte8_6_toggled(bool checked);

    void on_cbByte8_7_toggled(bool checked);

    //
    void on_cbByte9_0_toggled(bool checked);

    void on_cbByte9_1_toggled(bool checked);

    void on_cbByte9_2_toggled(bool checked);

    void on_cbByte9_3_toggled(bool checked);

    void on_cbByte9_4_toggled(bool checked);

    void on_cbByte9_5_toggled(bool checked);

    void on_cbByte9_6_toggled(bool checked);

    void on_cbByte9_7_toggled(bool checked);

//10 ui
    void on_cbByte10_0_toggled(bool checked);

    void on_cbByte10_1_toggled(bool checked);

    void on_cbByte10_2_toggled(bool checked);

    void on_cbByte10_3_toggled(bool checked);

    void on_cbByte10_4_toggled(bool checked);

    void on_cbByte10_5_toggled(bool checked);

    void on_cbByte10_6_toggled(bool checked);

    void on_cbByte10_7_toggled(bool checked);
// 11 ui
    void on_cbByte11_0_toggled(bool checked);

    void on_cbByte11_1_toggled(bool checked);

    void on_cbByte11_2_toggled(bool checked);

    void on_cbByte11_3_toggled(bool checked);

    void on_cbByte11_4_toggled(bool checked);

    void on_cbByte11_5_toggled(bool checked);

    void on_cbByte11_6_toggled(bool checked);

    void on_cbByte11_7_toggled(bool checked);

    void on_btnClearBytes_clicked();

    void on_btnSetBytes_clicked();

    void on_btnSetBits1_clicked();

    void on_btnSetBits2_clicked();

    void on_btnSetBits3_clicked();

    void on_btnSetBits4_clicked();

    void on_btnSetBits5_clicked();

    void on_btnSetBits6_clicked();

    void on_btnSetBits7_clicked();

    void on_btnSetBits8_clicked();

    void on_btnSetBits9_clicked();

    void on_btnSetBits10_clicked();

    void on_btnSetBits11_clicked();
//
    void on_btnClearBits1_clicked();

    void on_btnClearBits2_clicked();

    void on_btnClearBits3_clicked();

    void on_btnClearBits4_clicked();

    void on_btnClearBits5_clicked();

    void on_btnClearBits6_clicked();
    void on_btnClearBits7_clicked();
    void on_btnClearBits8_clicked();
    void on_btnClearBits9_clicked();
    void on_btnClearBits10_clicked();
    void on_btnClearBits11_clicked();


    void updatePacketModel() const;

    void refreshEnabled();
    void refreshBytesVisible(int dlc);

    void on_cbFill1_stateChanged(int arg1);
    void on_cbFill2_stateChanged(int arg1);
    void on_cbFill3_stateChanged(int arg1);
    void on_cbFill4_stateChanged(int arg1);
    void on_cbFill5_stateChanged(int arg1);
    void on_cbFill6_stateChanged(int arg1);
    void on_cbFill7_stateChanged(int arg1);
    void on_cbFill8_stateChanged(int arg1);
    void on_cbFill9_stateChanged(int arg1);
    void on_cbFill10_stateChanged(int arg1);
    void on_cbFill11_stateChanged(int arg1);
};

#endif // BEANPACKETWIDGET_H
