#include "beanpacketsplayermodel.h"
#include "beanpacketsplayerwindow.h"
#include "ui_beanpacketsplayerwindow.h"

#include <QCheckBox>
#include <QFileDialog>
#include <QDebug>
#include <QFileIconProvider>
#include <QStyle>
#include <QCommonStyle>
#include <QTimer>
#include <QThread>

BeanPacketsPlayerWindow::BeanPacketsPlayerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BeanPacketsPlayerWindow)
{
    ui->setupUi(this);
    ui->cbDelay->setChecked(true);
    on_cbDelay_stateChanged(0); //kick style apply

    QCommonStyle style;
    ui->actionLoadFile->setIcon(style.standardIcon(QStyle::SP_DialogOpenButton));
    ui->actionPlayPause->setIcon(style.standardIcon(QStyle::SP_MediaPlay));

    ui->toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    ui->lineEdit->setValidator(new QIntValidator(0, 10000, this));
    ui->lineEdit->setAlignment(Qt::AlignRight);

    QTableView *table = ui->tablePackets;

    playModel = new BeanPacketsPlayerModel(parent);
    table->setModel(playModel);

    int columnWidth = 30;
    table->setColumnWidth(0, 30); //checkboxes
    table->setColumnWidth(1, columnWidth * 4); // time
    table->setColumnWidth(2, columnWidth);
    table->setColumnWidth(3, columnWidth);
    table->setColumnWidth(4, columnWidth);
    table->setColumnWidth(5, columnWidth * 5);
    table->setColumnWidth(6, columnWidth);
    table->setColumnWidth(7, columnWidth);
    table->setColumnWidth(8, columnWidth * 20);
//    ui->tableLog->setColumnWidth(8, columnWidth * 20);

    table->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    table->verticalHeader()->hide();

    QHeaderView *verticalHeader = table->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(20);

//    QStringList m_TableHeader;
//    m_TableHeader << "Dst" << "Msg" ;
//    //table->setHorizontalHeaderLabels(m_TableHeader);
//    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
//    table->resizeColumnsToContents();
//    table->setColumnWidth(0, columnWidth*2);
//    table->setColumnWidth(1, columnWidth*2);
//    QCheckBox *cb = new QCheckBox;
//    cb->setText("TODO");
//    table->setCellWidget(1, 1, cb);
}

BeanPacketsPlayerWindow::~BeanPacketsPlayerWindow()
{
    delete ui;
}

bool BeanPacketsPlayerWindow::getConnected()
{
    return connected;
}

void BeanPacketsPlayerWindow::setConnected(bool value)
{
    connected = value;
    auto actions = ui->toolBar->actions();
    if (actions.size() > 1) {
        actions[1]->setEnabled(connected);
    }
}

void BeanPacketsPlayerWindow::on_actionLoadFile_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                               QDir::currentPath(),
                               tr("Dumps (*.txt *.csv)"));
    qDebug() << "File fro open: " << fileName;
    if (!fileName.isEmpty()) {
        playModel->loadFromCSV(fileName);
    }
}

void BeanPacketsPlayerWindow::on_cbDelay_stateChanged(int arg1)
{
    //ui->lineEdit->setAutoFillBackground(true);
    ui->lineEdit->setReadOnly(!ui->cbDelay->isChecked());

    QPalette palette; // fixed it (need to initialize)

//    palette.setColor(this->backgroundRole(), transparent); // for background (fixed)
//    palette.setColor(this->foregroundRole(), fgrndColor); // for foreground
    ui->lineEdit->setAutoFillBackground(true); // to allow to fill the background
//    ui->lineEdit->setPalette(palette);


   if (ui->cbDelay->isChecked()) {
       palette.setColor(QPalette::Base,Qt::white);
       ui->lineEdit->setPalette(palette);
   } else {
       palette.setColor(QPalette::Base, Qt::lightGray);
       ui->lineEdit->setPalette(palette);
   }
}

// TODO fix
void BeanPacketsPlayerWindow::on_actionPlayPause_triggered()
{
    // if (!connected) {
    //     qWarning() << "Attempt to send packets while disconnected.";
    //     ui->statusbar->showMessage("Serial port is not connected!", 10000);
    //     return;
    // }

    // QSerialPort *serial = getSerialPort();

    // QListIterator<BeanPacket*> i(playModel->getPackets());
    // bool play = true;
    // while (play) {
    //     int row = 1;
    //     while(i.hasNext()) {
    //         auto packet = i.next();

    //         packet->sendToSerialBin(serial);
    //         ui->tableView->selectRow(row);
    //         ui->tableView->scrollTo(playModel->index(row, 0));
    //         row++;
    //         //TODO fixup
    //         QThread::msleep(ui->lineEdit->text().toInt());
    // //        timer->setInterval(ui->lineEdit->text().toInt());
    // //        timer->start();
    //     }
    //     play = ui->cbRepeat->isChecked();
    // }
}


// QSerialPort *BeanPacketsPlayerWindow::getSerialPort() {
//     MainWindow window;
//     foreach(QWidget *widget, qApp->topLevelWidgets()) {
//         if (auto *mainWindow = dynamic_cast<MainWindow*>(widget)) {
//             return  mainWindow->getSerialPort();
//             break;
//         }
//     }

//     return nullptr;
// }

