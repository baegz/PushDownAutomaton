#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <pushDownAutomaton.h>
#include <graphwidget.h>

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
    explicit Dialog(QWidget *parent = nullptr);

    static int randomBetween(int low, int high){
        return (qrand() % ((high + 1) - low) + low);
    }

    ~Dialog();
    Node *getNode(QString name);
private slots:
    void on_initialState_clicked();
    void on_normalState_clicked();
    void on_acceptanceState_clicked();
    void on_newCondition_clicked();
    void on_validateExpression_clicked();

    void on_showConditions_clicked();

private:
    Ui::Dialog *ui;
    GraphWidget *graphWidget;
    PushDownAutomaton *pda;
    QList<Node *> myNodes;
};

#endif // DIALOG_H
