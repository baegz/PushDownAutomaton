#include <graphicstate.h>

GraphicState::GraphicState(QString name, int type, QObject *parent) :
    QObject(parent), QGraphicsItem(){
    this->setType(type);
    this->setName(name);
}

GraphicState::~GraphicState(){}

QRectF GraphicState::boundingRect() const {
    return QRectF (0,0,50,50);
}

void GraphicState::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QFont font;
    font.setFamily("Arial");
    font.setPointSize(15);
    painter->setFont(font);

    switch(this->getType()){
        case INITIAL:
            painter->setPen(Qt::red);
            painter->setBrush(QColor(255,0,0,150));
            painter->drawText(15,30,this->getName());
            painter->drawEllipse(0,0,50,50);
            break;
        case ACCEPTANCE:
            painter->setPen(Qt::green);
            painter->setBrush(QColor(0,255,0,150));
            painter->drawText(15,30,this->getName());
            painter->drawEllipse(0,0,50,50);
            painter->drawEllipse(5,5,40,40);
            break;
        default:
            painter->setPen(Qt::blue);
            painter->setBrush(QColor(0,0,255,150));
            painter->drawText(15,30,this->getName());
            painter->drawEllipse(0,0,50,50);
    }

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void GraphicState::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    /* Set the position of the graphical element in the graphic scene,
     * translate coordinates of the cursor within the graphic element
     * in the coordinate system of the graphic scenes
     * */
    this->setPos(mapToScene(event->pos()));
}

void GraphicState::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    this->setCursor(QCursor(Qt::ClosedHandCursor));
    Q_UNUSED(event);
}

void GraphicState::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    this->setCursor(QCursor(Qt::ArrowCursor));
    Q_UNUSED(event);
}
