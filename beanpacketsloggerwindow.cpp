#include "beanpacketsloggerwindow.h"
#include "ui_beanpacketsloggerwindow.h"
#include "beanpacketsloggermodel.h"

#include <QFileDialog>
#include <QDebug>
#include <QCommonStyle>

BeanPacketsLoggerWindow::BeanPacketsLoggerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BeanPacketsLoggerWindow)
{
    ui->setupUi(this);

    QCommonStyle style;
    ui->actionSaveAll->setIcon(style.standardIcon(QStyle::SP_DriveFDIcon));

    ui->toolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    auto logTable = ui->tableLog;
    logModel = new BeanPacketsLoggerModel();

    logTable->setModel(logModel);

    //BeanPacketsLoggerHeaderView *myHeader = new BeanPacketsLoggerHeaderView(Qt::Horizontal, ui->tableLog);
    //ui->tableLog->setHorizontalHeader(myHeader);

    QHeaderView *verticalHeader = logTable->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(24);

    int columnWidth = 30;
    logTable->setColumnWidth(0, 30); //checkboxes
    logTable->setColumnWidth(1, columnWidth * 4); // time
    logTable->setColumnWidth(2, columnWidth);
    logTable->setColumnWidth(3, columnWidth);
    logTable->setColumnWidth(4, columnWidth);
    logTable->setColumnWidth(5, columnWidth * 5);
    logTable->setColumnWidth(6, columnWidth);
    logTable->setColumnWidth(7, columnWidth);
    logTable->setColumnWidth(8, columnWidth * 20);
//    ui->tableLog->setColumnWidth(8, columnWidth * 20)
    logTable->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);

    // //////////
    ui->tableIds->setColumnCount(2);
    ui->tableIds->setRowCount(2);
    ui->tableIds->verticalHeader()->hide();
    QStringList m_TableHeader;
    m_TableHeader << "Dst" << "Msg" ;
    ui->tableIds->setHorizontalHeaderLabels(m_TableHeader);
    ui->tableIds->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableIds->resizeColumnsToContents();
    ui->tableIds->setColumnWidth(0, columnWidth*2);
    ui->tableIds->setColumnWidth(1, columnWidth*2);
    auto *cb = new QCheckBox;
    cb->setText("TODO");
    ui->tableIds->setCellWidget(1, 1, cb);
}

BeanPacketsLoggerWindow::~BeanPacketsLoggerWindow()
{
    delete ui;
    delete logModel;
}

void BeanPacketsLoggerWindow::appendPacket(QSharedPointer<BeanPacket> packet)
{
    logModel->appendPacket(packet);
    if (ui->cbAutoScroll->isChecked()) {
        ui->tableLog->scrollToBottom();
    }
}

void BeanPacketsLoggerWindow::on_pushButton_clicked()
{
    logModel->clearPackets();
}

void BeanPacketsLoggerWindow::on_actionSaveAll_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               QDir::homePath() + "/untitled.csv",
                               tr("Dumps (*.txt *.csv)"));
    qDebug() << fileName;
    logModel->saveAsCSV(fileName);
}
