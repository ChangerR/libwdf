#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "WdfPack.h"
#include <QVector>
#include <QMap>
#include "qbytearray.h"
#include "frame_item.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QVector<WdfPack*> _packs;
    QMap<QString,QByteArray*> _was;
    Frame_item* _item;
private slots:
    void onFileOpen();

    void onItemClick(const QPoint& point);

    void onAbout();
};


#endif // MAINWINDOW_H
