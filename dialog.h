#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QMessageBox>
#include "stackautomaton.h"

struct point{
    QString name;
    int x;
    int y;
};

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);

    void drawState(int x, QColor c, const QString txt){
        //Look x,y for each state
        x--;
        int y=(x/5)*150;
        x = (x%5)*150;

        if(c == Qt::green){ //Drawing acceptances states
            QBrush brush(QColor(0,255,0,150));
            QPen pen(Qt::green);
            pen.setWidth(2);
            currentState = scene->addEllipse(x,y,65,65,pen,brush);
            scene->addEllipse(x+8,y+7,50,50,pen,brush);
        }else if(c == Qt::red){ //Drawing initial state
            QBrush brush(QColor(255,0,0,150));
            QPen pen(Qt::red);
            pen.setWidth(2);
            currentState = scene->addEllipse(x,y,65,65,pen,brush);
        }else{ //Drawing Tara
            QBrush brush(QColor(0,0,255,150));
            QPen pen(Qt::blue);
            pen.setWidth(2);
            currentState = scene->addEllipse(x,y,65,65,pen,brush);
        }

        QGraphicsTextItem *graphicText = new QGraphicsTextItem;
        QFont font;
        font.setPointSize(18);
        font.setFamily("Consolas");
        graphicText->setFont(font);
        graphicText->setPos(x+19,y+15);
        graphicText->setPlainText(txt);
        scene->addItem(graphicText);
        states.push_back(currentState);
        point p;
        p.x=x+18;
        p.y=y+15;
        p.name = txt;
        pointStates.push_back(p);
    }

    void drawCondition(QString origin, QString destination, condition cd){
        QBrush brush(Qt::NoBrush);
        QPen pen(Qt::white);
        pen.setWidth(1);
        point p,q;
        for(int i=0;i<pointStates.size();i++){
            if(pointStates[i].name == origin){
                p.x = pointStates[i].x;
                p.y = pointStates[i].y;
            }
            if(pointStates[i].name == destination){
                q.x = pointStates[i].x;
                q.y = pointStates[i].y;
            }
        }

        scene->addRect(p.x,p.y,q.x,q.y,pen,brush);
        /*QGraphicsTextItem *graphicText = new QGraphicsTextItem;
        QFont font;
        font.setPointSize(18);
        font.setFamily("Consolas");
        graphicText->setFont(font);
        graphicText->setPos((x*150)+18,(y*150)+15);
        graphicText->setPlainText(txt);
        scene->addItem(graphicText);
        states.push_back(currentState);*/
    }
    ~Dialog();

private slots:
    void on_initialState_clicked();

    void on_normalState_clicked();

    void on_acceptanceState_clicked();

    void on_newCondition_clicked();

    void on_validateExpression_clicked();

private:
    Ui::Dialog *ui;
    QGraphicsScene *scene;
    QGraphicsEllipseItem *currentState;
    StackAutomaton *stackAutomaton;
    QVector<QGraphicsEllipseItem *> states;
    QVector<point> pointStates;
    QRect *arrow;
};

#endif // DIALOG_H
