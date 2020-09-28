#include "beanpacketssender.h"
#include "ui_beanpacketssender.h"

#include <beanpacketssendermodel.h>
#include <QDebug>
#include <QFontDatabase>

BeanPacketsSender::BeanPacketsSender(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::BeanPacketsSender) {
    ui->setupUi(this);
    connected = false;
    int baseWidth = 32;

    QTableView *table = ui->tablePacketsSend;
    senderModel = new BeanPacketsSenderModel(table);
    table->setModel(senderModel);

    auto firstPacket = new BeanPacket();
    firstPacket->ro = false;
    senderModel->appendPacket(firstPacket);
    ui->packetWidget->reloadPacket();

    auto *selectionModel = table->selectionModel();
    if (selectionModel) {
        qDebug() << "Connecting row changed slot";
        QObject::connect(
                selectionModel,
                SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
                this,
                SLOT(rowChanged(const QModelIndex &, const QModelIndex &))
        );
    }

//    auto font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
//    font.setKerning(true);
//    font.setBold(false);
//    font.setPointSize(13);
//    table->setFont(font);

    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setDefaultSectionSize(20);
    table->verticalHeader()->hide();
    table->setGridStyle(Qt::SolidLine);
    table->setSortingEnabled(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setShowGrid(true);
    table->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    table->setAlternatingRowColors(true);
    //table->setCornerButtonEnabled(true);

    QStringList m_TableHeader;
    m_TableHeader << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "";
//    table->setHorizontalHeaderLabels(m_TableHeader);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    //table->horizontalHeader()->setStretchLastSection(true);

    table->setColumnWidth(0, baseWidth);
    table->setColumnWidth(1, baseWidth);
    table->setColumnWidth(2, baseWidth);
    table->setColumnWidth(3, baseWidth * 7); // data
    table->setColumnWidth(4, baseWidth);
    table->setColumnWidth(5, baseWidth);
    table->setColumnWidth(6, baseWidth);
    table->setColumnWidth(7, baseWidth * 2);
    table->setColumnWidth(8, baseWidth * 2);
}

BeanPacketsSender::~BeanPacketsSender() {
    delete ui;
}

bool BeanPacketsSender::isLiveSend() {
    return true; // TODO
}

void BeanPacketsSender::deviceSend() {
    // TODO
}

bool BeanPacketsSender::getConnected() const {
    return connected;
}

void BeanPacketsSender::setConnected(bool value) {
    connected = value;
}

void BeanPacketsSender::on_btnAdd_clicked() {
    auto *newPacket = new BeanPacket();
    senderModel->appendPacket(newPacket);
}

void BeanPacketsSender::on_tablePacketsSend_clicked(const QModelIndex &index) {
    qDebug() << "Clicked row" << index.row();
    rowChanged(index.row());
}
void BeanPacketsSender::dataChanged() {
    senderModel->layoutChanged();
}

void BeanPacketsSender::rowChanged(const int row) const {
    ui->packetWidget->setPacket(senderModel->packetAt(row), senderModel);
}

void BeanPacketsSender::on_btnRun_clicked() {
    // get reference to selected packet from send packets table

}

void BeanPacketsSender::on_btnDel_clicked() {
    int row = ui->tablePacketsSend->currentIndex().row();
    // TODO widget should have weak ref to not crash when table is empty
    if (row > 0) {
        qDebug() << "Removing row: " << row;
        senderModel->removePacketAt(row);
        qDebug() << "Selecting after rm row" << row;
        ui->tablePacketsSend->selectRow(row - 1);
        rowChanged(row - 1);
    }
}

// notify UI that packet received to redraw something
void BeanPacketsSender::packetReceived() {
    ui->packetWidget->reloadPacket();
}



void BeanPacketsSender::appendPacket(BeanPacket *pPacket) {
    pPacket->ro = false;
    senderModel->appendPacket(pPacket);
    ui->packetWidget->reloadPacket();
}

void BeanPacketsSender::rowChanged(const QModelIndex &current, const QModelIndex &previous) {
    rowChanged(current.row());
}

void BeanPacketsSender::on_btnShot_clicked()
{
    int row = ui->tablePacketsSend->currentIndex().row();
    auto packet = senderModel->packetAt(row);
    if (packet) {
        qDebug() << "Asking packet to send once...";
        packet->sendOnce();
    }
}
