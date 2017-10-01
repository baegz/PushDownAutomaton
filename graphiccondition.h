#ifndef GRAPHICCONDITION_H
#define GRAPHICCONDITION_H

#include <QGraphicsObject>

class graphicCondition : public QGraphicsObject {

public:
    graphicCondition();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
};

#endif // GRAPHICCONDITION_H
