#include <edge.h>
#include <node.h>

#include <math.h>

#include <QPainter>

static const double PI = 3.14159265358979323846264338327950288419717;
static double TwoPI = 2.0 * PI;

Edge::Edge(Node *sourceNode, Node *destNode)
    : arrowSize(10)
{
    setAcceptedMouseButtons(0);
    source = sourceNode;
    dest = destNode;
    source->addEdge(this);
    dest->addEdge(this);
    adjust();
}

Node *Edge::sourceNode() const
{
    return source;
}

Node *Edge::destNode() const
{
    return dest;
}

void Edge::adjust()
{
    if (!source || !dest) return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    } else {
        sourcePoint = destPoint = line.p1();
    }
}

int Edge::type() const
{
    return Type;
}

QRectF Edge::boundingRect() const
{
    if (!source || !dest) return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized().adjusted(-extra, -extra, extra, extra);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    painter->setPen(QPen(QColor(200,200,200).lighter(), 0.33, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    // Dibujando los condiciones
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPI - angle;
    QPointF destArrowP1 = destPoint + QPointF(sin(angle - PI / 3) * arrowSize/1.6,
                                              cos(angle - PI / 3) * arrowSize/1.6);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - PI + PI / 3) * arrowSize/1.6,
                                              cos(angle - PI + PI / 3) * arrowSize/1.6);

    painter->setBrush(QColor(200,200,200).lighter());
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}
