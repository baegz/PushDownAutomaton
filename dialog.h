#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QMessageBox>
#include <pushDownAutomaton.h>
#include <graphicstate.h>

const QString STARTTITLE = "Nuevo automata de pila";
const QString GETLIMIT ="Bienvenido a PushDownAutomaton Simulator BETA\n\nNumero maximo de estado: ";
const QString ERRORNEWSTATE = "Error de creacion";
const QString LIMITSTATE = "Has alcanzado el limite maximo para poder crear estados!";
const QString RESULT = "Resultado";

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);

    static int randomBetween(int low, int high){
        return (qrand() % ((high + 1) - low) + low);
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
    PushDownAutomaton *pda;
    GraphicState *graphicState;
    bool movingState;
};

#endif // DIALOG_H
