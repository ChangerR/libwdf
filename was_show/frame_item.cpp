#include "frame_item.h"
#include <qpainter.h>

Frame_item::Frame_item(QSharedPointer<WasSpirit> &spirit)
{
    _spirit = spirit;
    setFlag(QGraphicsItem::ItemIsMovable);

    startTimer(500);
}

Frame_item::~Frame_item()
{
    while(_queue.empty() == false) {
        QImage* img = _queue.dequeue();
        delete img;
    }
}

bool Frame_item::init(int spirit_index)
{
    bool ret = false;
    do {
        if(_spirit == NULL && spirit_index < _spirit->_wasHead.spritecount)
            break;

        for(int i = 0;i < _spirit->_wasHead.framecount;i++) {
            QImage* image = new QImage((unsigned char*)_spirit->_frames[spirit_index][i].data,
                                       _spirit->_frames[spirit_index][i].width,
                                       _spirit->_frames[spirit_index][i].height,
                                       QImage::Format_ARGB32);
           _queue.enqueue(image);
        }
        ret = true;
    }while(0);
    return ret;
}

void Frame_item::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    update();
}

void Frame_item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->fillRect(this->boundingRect(),Qt::blue);
    if(!_queue.empty()) {
        QImage* image = _queue.dequeue();
        _queue.enqueue(image);

        painter->drawImage(boundingRect().topLeft(),*image);
    }
}

QRectF Frame_item::boundingRect() const
{
    return QRectF(-_spirit->_wasHead.width / 2,-_spirit->_wasHead.height / 2,
                  _spirit->_wasHead.width / 2 ,_spirit->_wasHead.height / 2);
}
