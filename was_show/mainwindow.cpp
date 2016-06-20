#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qstring.h>
#include <qfiledialog.h>
#include <qstandarditemmodel.h>
#include <qgraphicsscene.h>
#include "frame_item.h"
#include <qfile.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStandardItemModel* mode = new QStandardItemModel(ui->treeView);
    mode->setHorizontalHeaderLabels(QStringList()<<"文件列表");
    ui->treeView->setModel(mode);
    QGraphicsScene* scene = new QGraphicsScene(ui->graphicsView);
    scene->setSceneRect(-ui->graphicsView->rect().width() / 2,-ui->graphicsView->rect().height() / 2,
                        ui->graphicsView->rect().width(),ui->graphicsView->height());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->centerOn(0,0);
    connect(ui->actionOpen,SIGNAL(triggered(bool)),this,SLOT(onFileOpen()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onFileOpen()
{
    QString filename;
    QFileDialog* p_filedialog = new QFileDialog(this);

    p_filedialog->setFilter(QDir::Files);
    p_filedialog->setNameFilter("was(*.was);;Allfile(*.*)");
    p_filedialog->setViewMode(QFileDialog::List);
    p_filedialog->setDirectory(QDir::current());
    if(p_filedialog->exec() == QFileDialog::Accepted) {
        filename = p_filedialog->selectedFiles()[0];
        QFileInfo fileinfo(filename);
        if(fileinfo.isFile() && fileinfo.suffix() == "was") {
            QStandardItem* item = new QStandardItem(fileinfo.fileName());
            ((QStandardItemModel*)(ui->treeView->model()))->appendRow(item);

            QFile file(filename);
            if(file.open(QIODevice::ReadOnly) != 0) {
                QByteArray data =  file.readAll();
                QSharedPointer<WasSpirit> was(WasSpirit::createSpirit((unsigned char*)data.data(),data.size()));
                Frame_item* item = new Frame_item(was);
                if(item->init(0) != false) {
                    ui->graphicsView->scene()->addItem(item);
                    item->setPos(0,0);
                }else {
                    delete item;
                }
            }
        }
    }
}
