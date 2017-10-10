#include <dialog.h>
#include <ui_dialog.h>
#include <QMessageBox>
#include <QInputDialog>
#include <node.h>
#include <graphwidget.h>
#include <QGraphicsScene>
#include <edge.h>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    int MAXSTATES=0;
    do{
        MAXSTATES = QInputDialog::getText(this,STARTTITLE,GETLIMIT).toInt();
    }while(MAXSTATES < 1);
    this->pda = new PushDownAutomaton(MAXSTATES);

    ui->setupUi(this);
    this->graphWidget = new GraphWidget(this);
}

Dialog::~Dialog()
{
    delete ui;
}

Node *Dialog::getNode(QString name)
{
    for(int i=0;i<myNodes.size();i++)
        if(myNodes[i]->getName() == name)
            return myNodes[i];
    return nullptr;
}

void Dialog::on_initialState_clicked()
{
    if(!pda->existInicialState()){
        QString name = "q"+QString::number(pda->getNumStates());

        if(pda->newState(name,INITIAL)) {
            Node *state = new Node(graphWidget,name,INITIAL);
            state->setPos(Dialog::randomBetween(-125,125),Dialog::randomBetween(-80,80));
            graphWidget->scene()->addItem(state);
            ui->initialState->setEnabled(false);
            myNodes.append(state);
        }else
            QMessageBox::critical(this,ERRORNEWSTATE,LIMITSTATE);
    }
}

void Dialog::on_normalState_clicked()
{
    QString name = "q"+QString::number(pda->getNumStates());

    if(pda->newState(name,TRANSITION)) {
        Node *state = new Node(graphWidget, name, TRANSITION);
        state->setPos(Dialog::randomBetween(-125,125),Dialog::randomBetween(-80,80));
        graphWidget->scene()->addItem(state);
        myNodes.append(state);
    }else
        QMessageBox::critical(this,ERRORNEWSTATE,LIMITSTATE);
}

void Dialog::on_acceptanceState_clicked()
{
    QString name = "q"+QString::number(pda->getNumStates());

    if(pda->newState(name,ACCEPTANCE)) {
        Node *state = new Node(graphWidget,name,ACCEPTANCE);
        state->setPos(Dialog::randomBetween(-125,125),Dialog::randomBetween(-80,80));
        graphWidget->scene()->addItem(state);
        myNodes.append(state);
    }else
        QMessageBox::critical(this,ERRORNEWSTATE,LIMITSTATE);
}

void Dialog::on_newCondition_clicked()
{
    pda->setOrigin(ui->origin->text());
    pda->setDestination(ui->destination->text());
    pda->setCondition(ui->tape->text(),ui->pop->text(),ui->push->text());

    int result = pda->newCondition();
    Node *origin = NULL, *destination = NULL;
    switch(result) {
        case CREATEDCONDITION:
            origin = Dialog::getNode(ui->origin->text());
            destination = Dialog::getNode(ui->destination->text());
            graphWidget->scene()->addItem(new Edge(origin, destination));
            break;
        case NOSTATE:
            QMessageBox::critical(this,"Nueva condicion","No existen estados");
            break;
        case NOORIGIN:
            QMessageBox::critical(this,"Nueva condicion","No existe el estado origen!");
            break;
        case NODESTINATION:
            QMessageBox::critical(this,"Nueva condicion","No existe el estado de destino!");
            break;
        case EXISTCONDITION:
            QMessageBox::information(this,"Nueva condicion","La condicion ya existe!");
            break;
        case INVALIDCONDITION:
            QMessageBox::warning(this,"Nueva condicion","Condicion invalida, todos los campos son obligatorios");
            break;
    }
}

void Dialog::on_validateExpression_clicked() {
    QString expression;
    expression = ui->expression->text();

    if(expression.size() != EMPTY) {
        if(pda->existInicialState()) {
            if(Dialog::pda->existAcceptanceState()){
                if(pda->validateExpression(expression)) {
                    QMessageBox::information(this,"Resultado","Expresion aceptada!\n\nRuta:\n"+pda->getAcceptanceRoute());
                }else{
                    QMessageBox::information(this,"Resultado","Expresion no aceptada!");
                }
            }else{
                QMessageBox::warning(this,"Resultado","Se requiere un estado de aceptacion!");
            }
        }else{
            QMessageBox::warning(this,"Resultado","Se requiere un estado inicial!");
        }
    }else{
        QMessageBox::warning(this,"Resultado","Sin expression!");
    }
}

void Dialog::on_showConditions_clicked()
{
    QString conditions = pda->getConditions();
    if(conditions.size() == EMPTY) QMessageBox::information(this,"PushDownAutomaton Simulator: Condiciones","No existen condiciones");
    else QMessageBox::information(this,"PushDownAutomaton Simulator: Condiciones","Condiciones:\n"+conditions);
}
