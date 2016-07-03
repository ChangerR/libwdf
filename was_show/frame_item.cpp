#include "frame_item.h"
#include <qpainter.h>

Frame_item::Frame_item(QSharedPointer<dream::WasSpirit> &spirit)
{
    _spirit = spirit;
    setFlag(QGraphicsItem::ItemIsMovable);

    startTimer(200);
}

Frame_item::~Frame_item()
{
    for(auto iter = _queues.begin();iter != _queues.end();++iter) {
        while(iter->empty() == false) {
            QImage* img = iter->dequeue();
            if(img)
                delete img;
        }
    }
}

bool Frame_item::init()
{
    bool ret = false;
    do {
        if(_spirit == NULL)
            break;

        for(int spirit_index = 0;spirit_index < _spirit->get_spirit_count();spirit_index++) {
            QQueue<QImage*> queue;

            for(int i = 0;i < _spirit->get_frame_count();i++) {
                QImage* image = NULL;
                if(_spirit->get_frame(spirit_index,i)->data)
                    image = new QImage((unsigned char*)(_spirit->get_frame(spirit_index,i)->data),
                                               _spirit->get_frame(spirit_index,i)->width,
                                               _spirit->get_frame(spirit_index,i)->height,
                                               QImage::Format_ARGB32);
               queue.enqueue(image);
            }

            _queues.push_back(queue);
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

    if(_queues.size() == 1) {
        QImage* img = _queues[0].dequeue();
        _queues[0].enqueue(img);

        if(img)
            painter->drawImage(boundingRect().topLeft(),*img);
    } else {
        for(int i = 0;i < _queues.size();i++) {
            QImage* img = _queues[i].dequeue();
            _queues[i].enqueue(img);
            if(img == NULL)
                continue;

            switch (i) {
            case 0:
                painter->drawImage(QPointF(-(boundingRect().width() / 2),-(boundingRect().height() / 2)),*img);
                break;
            case 1:
                painter->drawImage(QPointF(0,-(boundingRect().height() / 2)),*img);
                break;
            case 2:
                painter->drawImage(QPointF(-(boundingRect().width() / 2),0),*img);
                break;
            case 3:
                painter->drawImage(QPointF(0,0),*img);
                break;
            default:
                break;
            }
        }
    }
}

QRectF Frame_item::boundingRect() const
{
    qreal adjust=0.5;

    if(_queues.size() <= 1) {
        return QRectF(-_spirit->get_width() / 2 - adjust,-_spirit->get_height() / 2 - adjust,
                      _spirit->get_width() + adjust,_spirit->get_height() + adjust);
    }else {
        return QRectF(-_spirit->get_width() - adjust,-_spirit->get_height() - adjust,
                      _spirit->get_width() * 2 + adjust,_spirit->get_height()*2 + adjust);
    }

}
