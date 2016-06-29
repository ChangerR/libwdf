#include "frame_item.h"
#include <qpainter.h>

Frame_item::Frame_item(QSharedPointer<dream::WasSpirit> &spirit)
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
        if(_spirit == NULL && spirit_index < _spirit->get_spirit_count())
            break;

        for(int i = 0;i < _spirit->get_frame_count();i++) {
            QImage* image = new QImage((unsigned char*)(_spirit->get_frame(spirit_index,i)->data),
                                       _spirit->get_frame(spirit_index,i)->width,
                                       _spirit->get_frame(spirit_index,i)->height,
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
    painter->fillRect(this->boundingRect(),Qt::transparent);

    if(!_queue.empty()) {
        QImage* image = _queue.dequeue();
        _queue.enqueue(image);

        painter->drawImage(boundingRect().topLeft(),*image);
    }

}

QRectF Frame_item::boundingRect() const
{
    qreal adjust=0.5;

    return QRectF(-_spirit->get_width() / 2 - adjust,-_spirit->get_height() / 2 - adjust,
                  _spirit->get_width() + adjust,_spirit->get_height() + adjust);
}
