#ifndef FRAME_ITEM_H
#define FRAME_ITEM_H

#include <qobject.h>
#include <qgraphicsitem.h>
#include "was_spirit.h"
#include <qqueue.h>

class Frame_item:public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    Frame_item(QSharedPointer<dream::WasSpirit>& spirit);

    virtual ~Frame_item();

    bool init(int);

    void timerEvent(QTimerEvent *event);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QRectF boundingRect() const;
private:
    QSharedPointer<dream::WasSpirit> _spirit;
    QQueue<QImage*> _queue;
};

#endif // FRAME_ITEM_H
