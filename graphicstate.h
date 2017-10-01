#ifndef GRAPHICSTATE_H
#define GRAPHICSTATE_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>
#include <state.h>

class GraphicState : public QObject, public QGraphicsItem {
    Q_OBJECT
public:
    void setType(int type){
        this->type = type;
    }

    int getType(){
        return this->type;
    }

    void setName(QString name){
        this->name = name;
    }

    QString getName() const {
        return this->name;
    }

    explicit GraphicState(QString name, int type, QObject *parent = 0);

    ~GraphicState();

private:
    int type;
    QString name;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // GRAPHICSTATE_H
