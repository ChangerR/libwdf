#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qstring.h>
#include <qfiledialog.h>
#include <qstandarditemmodel.h>
#include <qgraphicsscene.h>
#include "frame_item.h"
#include <qfile.h>
#include <qmessagebox.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _item(NULL)
{
    ui->setupUi(this);
    QStandardItemModel* mode = new QStandardItemModel(ui->treeView);
    mode->setHorizontalHeaderLabels(QStringList()<<"文件列表");
    ui->treeView->setModel(mode);

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->treeView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(onItemClick(QPoint)));

    QGraphicsScene* scene = new QGraphicsScene(ui->graphicsView);
    scene->setSceneRect(-ui->graphicsView->rect().width() / 2,-ui->graphicsView->rect().height() / 2,
                        ui->graphicsView->rect().width(),ui->graphicsView->height());

    QPixmap pixmap(80,80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);

    painter.fillRect(QRectF(40,0,40,40),QBrush(QColor(222,222,222)));
    painter.fillRect(QRectF(0,40,40,40),QBrush(QColor(222,222,222)));

    scene->setBackgroundBrush(pixmap);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->centerOn(0,0);
    connect(ui->actionOpen,SIGNAL(triggered(bool)),this,SLOT(onFileOpen()));
    connect(ui->actionAbout,SIGNAL(triggered(bool)),this,SLOT(onAbout()));
}

MainWindow::~MainWindow()
{
    delete ui;

    for(auto iter = _was.begin();iter != _was.end();++iter) {
        delete iter.value();
    }

    for(auto iter = _packs.begin() ;iter != _packs.end();++iter) {
        delete *iter;
    }
}

void MainWindow::onFileOpen()
{
    QString filename;
    QFileDialog* p_filedialog = new QFileDialog(this);

    p_filedialog->setFilter(QDir::Files);
    p_filedialog->setNameFilter("was(*.was);wdf(*.wd*);Allfile(*.*)");
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
                QByteArray* data = new QByteArray(file.readAll());
                _was.insert(fileinfo.fileName(),data);
                /*
                QSharedPointer<dream::WasSpirit> was(dream::WasSpirit::createSpirit((unsigned char*)data.data(),data.size()));
                Frame_item* item = new Frame_item(was);
                if(item->init(0) != false) {
                    ui->graphicsView->scene()->addItem(item);
                    item->setPos(0,0);
                }else {
                    delete item;
                }
                */
            }
        }else if(fileinfo.isFile()) {
            QStandardItem* item = new QStandardItem(fileinfo.fileName());
            ((QStandardItemModel*)(ui->treeView->model()))->appendRow(item);
            QByteArray ar = fileinfo.absoluteFilePath().toLatin1();
            WdfPack* pack = WdfPack::createWdfPack(ar.data());
            item->setEditable(false);
            if(pack != NULL) {
                unsigned int hash;
                int size;
                unsigned char* buffer;
                pack->beginEnumFile();
                while((buffer = pack->enumFile(hash,size)) != NULL) {
                    if(*(unsigned short*)buffer == WAS_FILE_TAG) {
                        QStandardItem* item2 = new QStandardItem(QString("%1").arg(hash));
                        item->appendRow(item2);
                        item2->setEditable(false);
                        QByteArray* array = new QByteArray((char*)buffer,size);
                        _was.insert(QString("%1").arg(hash),array);
                    }
                }
                _packs.push_back(pack);
            }
        }
    }
    delete p_filedialog;
}

void MainWindow::onItemClick(const QPoint& point)
{
    QModelIndex index = ui->treeView->indexAt(point);
    if(index.isValid()) {
        QStandardItem* item = ((QStandardItemModel*)(ui->treeView->model()))->itemFromIndex(index);
        if(item) {
            QPoint globalPos = ui->treeView->mapToGlobal(point);

            QMenu menu;

            menu.addAction("Open");
            menu.addAction("Save");

            QAction* action = menu.exec(globalPos);
            if(action && action->text() == "Open") {
                auto iter = _was.find(item->text());
                if(iter != _was.end()) {
                    QByteArray* byte = iter.value();
                    if(_item) {
                        ui->graphicsView->scene()->removeItem(_item);
                        delete _item;
                        _item = NULL;
                    }
                    QSharedPointer<dream::WasSpirit> was(dream::WasSpirit::createSpirit((unsigned char*)byte->data(),byte->size()));
                    _item = new Frame_item(was);
                    if(_item->init() != false) {
                        ui->graphicsView->scene()->addItem(_item);
                        _item->setPos(0,0);
                    }else {
                        delete _item;
                        _item = NULL;
                    }
                } else {
                    QMessageBox::about(this,"Warning","cannot show this resource");
                }
            }else if(action && action->text() == "Save") {
                QString s_name = QFileDialog::getSaveFileName(this,tr("save file"),
                                                              ".",tr("Images(*.was"));
                auto iter = _was.find(item->text());
                if(iter != _was.end()) {
                    QByteArray* byte = iter.value();
                    QFile file(s_name);

                    if (!file.open(QIODevice::WriteOnly)) {
                        QMessageBox::critical(this, "ERROR", "cannot create file");
                        return;
                    }
                    file.write(byte->data(),byte->size());
                    file.close();
                }

            }
        }
    }
}

void MainWindow::onAbout()
{
    QMessageBox::about(this,"About","<p>this is a tool for read spirit from wdf file</p>"
                       "<p>author:<span style=\"color:red\">changer</span></p>"
                       "<p>email:<span style=\"color:blue\">dingjinchengyx@163.com</span></p>");
}
