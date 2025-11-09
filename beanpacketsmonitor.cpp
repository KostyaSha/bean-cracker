#include "beanpacket.h"
#include "beanpacketsmonitor.h"
#include "ui_beanpacketsmonitor.h"
#include "beanpacketwidget.h"
#include <QDebug>
#include <QFontDatabase>

BeanPacketsMonitor::BeanPacketsMonitor(QWidget *parent) :
        QFrame(parent),
        ui(new Ui::BeanPacketsMonitor) {
    ui->setupUi(this);
    monitorModel = new BeanPacketsMonitorModel(parent);
    QTableView *table = ui->tableMonitor;
    table->setModel(monitorModel);

    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    table->setFont(font);

    auto *selectionModel = table->selectionModel();
    if (selectionModel) {
        qDebug() << "Connecting row changed slot";
        QObject::connect(selectionModel, SIGNAL(currentRowChanged()), this, SLOT(rowChanged()));
    }
    initializeBeanTable();

    auto* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &BeanPacketsMonitor::onUpdateTimer);
    timer->start(100);
}

BeanPacketsMonitor::~BeanPacketsMonitor() {
    delete ui;
}

void BeanPacketsMonitor::onUpdateTimer()
{
    // monitorModel.da
    // update();
}


void BeanPacketsMonitor::initializeBeanTable() {
    QTableView *table = ui->tableMonitor;
    int columnWidth = 30;

    QHeaderView *verticalHeader = table->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(20);
    verticalHeader->hide();

    //table->horizontalHeader()->setDefaultSectionSize(24);
//    table->verticalHeader()->hide();
    table->setSortingEnabled(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setShowGrid(true);
    table->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    table->setAlternatingRowColors(true);

    QStringList m_TableHeader;
    m_TableHeader << "" << "" << "" << "" << "" << "" << "" << "" << "";
//    table->setHorizontalHeaderLabels(m_TableHeader);
//    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    //table->horizontalHeader()->setStretchLastSection(true);

    table->setColumnWidth(0, columnWidth); // mark
    table->setColumnWidth(1, columnWidth); // dst
    table->setColumnWidth(2, columnWidth); // msg
    table->setColumnWidth(3, columnWidth); // dlc
    table->setColumnWidth(4, 200); // data
    table->setColumnWidth(5, columnWidth); // crc
    table->setColumnWidth(6, columnWidth); // prio
    table->setColumnWidth(7, columnWidth * 4); // repeat
    table->setColumnWidth(8, columnWidth * 4); // counter
    table->setColumnWidth(9, columnWidth * 5); // comment
}

void BeanPacketsMonitor::on_btnClearTable_clicked() {
    monitorModel->clearPackets();
}

void BeanPacketsMonitor::receiveSerialLine(QSharedPointer<BeanPacket> packet) {
    monitorModel->appendPacket(packet, ui->cbGroupMsgs->isChecked());
    if (ui->cbAutoScroll->isChecked()) {
        ui->tableMonitor->scrollToBottom();
    }
}

void BeanPacketsMonitor::on_tableMonitor_clicked(const QModelIndex &index) {
    rowChanged(index.row());
}


void BeanPacketsMonitor::rowChanged(int row) {
    auto packetWidget = parent()->findChild<BeanPacketWidget *>("packetWidget");
    if (packetWidget) {
        packetWidget->setPacket(monitorModel->getPacketAt(row), monitorModel);
    }
}

void BeanPacketsMonitor::rowChanged(const QModelIndex &current, const QModelIndex &previous) {
    Q_UNUSED(previous);
    rowChanged(current.row());
}

QSharedPointer<BeanPacket> BeanPacketsMonitor::getSelectedPacket() {
    auto selectedRows = ui->tableMonitor->selectionModel()->selectedRows();
    if (!selectedRows.isEmpty()) {
        int row = selectedRows.first().row();
        return monitorModel->getPacketAt(row);
    }
    return nullptr;
}
