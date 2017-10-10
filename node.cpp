#include "edge.h"
#include "node.h"
#include "graphwidget.h"
#include <state.h>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

Node::Node(GraphWidget *graphWidget, QString name, int typeNode)
    : graph(graphWidget)
{
    setName(name);
    setType(typeNode);
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> Node::edges() const
{
    return edgeList;
}

bool Node::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}

QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QFont font;
    QPolygonF arrow;
    font.setFamily("Segoe UI");
    font.setPointSize(6);
    painter->setFont(font);

    switch(this->getType()){
        case INITIAL:
            painter->setPen(QColor(255,0,0,255));
            painter->setBrush(QColor(255,0,0,150));
            painter->drawEllipse(-10,-10,20,20);
            arrow << QPoint(-13,7);
            arrow << QPoint(-13,-7);
            arrow << QPoint(-9,0);
            painter->setBrush(QColor(255,0,0,255));
            painter->drawPolygon(arrow);
            break;
        case TRANSITION:
            painter->setPen(QColor(20,20,255,255));
            painter->setBrush(QColor(20,20,255,150));
            painter->drawEllipse(-10,-10,20,20);
            break;
        case ACCEPTANCE:
            painter->setPen(QColor(0,255,0,255));
            painter->setBrush(QColor(0,255,0,150));
            painter->drawEllipse(-10,-10,20,20);
            painter->setBrush(QColor(0,255,0,25));
            painter->drawEllipse(-8,-8,16,16);
            break;
    }
    painter->drawText(-6,2,this->getName());
    Q_UNUSED(option);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        graph->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
