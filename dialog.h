#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QMessageBox>
#include "stackautomaton.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);

    void drawState(int x, QColor c){
        QBrush brush(c);
        QPen blackpen(Qt::black);
        blackpen.setWidth(3);
        //ellipse = scene->addEllipse(10,10,90,90,blackpen,redBrush);
        x--;
        int y=(x/5);
        x = (x%5);
        QMessageBox::information(this,"pos","y: "+QString::number(y)+" x:"+QString::number(x));
        currentState = scene->addEllipse(x*150,y*150,65,65,blackpen,brush);
        states.push_back(currentState);
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
};

#endif // DIALOG_H
