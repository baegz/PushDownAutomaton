#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include <QInputDialog>

#define MAXSTATE 25

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    this->pda = new PushDownAutomaton(MAXSTATE);
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_initialState_clicked()
{
    if(!pda->existInicialState()){
        retryName:
        const QString name = QInputDialog::getText(this,"Estado inicial","Nombre del estado");
        if(name.size() == 0){
            QMessageBox::warning(this,"Error de creacion","El nombre no puede estar vacio");
            goto retryName;
        }
        pda->newState(name,1);
        Dialog::drawState(pda->getNumStates(),Qt::red,name);
    }else{
        QString name = pda->getState(0).getName();
        QMessageBox::critical(this,"Error de creacion","El estado inicial ya existe: "+name);
    }
}

void Dialog::on_normalState_clicked()
{
    retryName:
    const QString name = QInputDialog::getText(this,"Estado de transicion","Nombre del estado");
    if(name.size() == 0){
        QMessageBox::warning(this,"Error de creacion","El nombre no puede estar vacio");
        goto retryName;
    }
    if(pda->getNumOfState(name) == -1){
        pda->newState(name,2);
        Dialog::drawState(pda->getNumStates(),Qt::blue,name);
    }else QMessageBox::warning(this,"Error de creacion","El estado ya existe!");
}

void Dialog::on_acceptanceState_clicked()
{
    QString name;
    do{
        name = QInputDialog::getText(this,"Estado de aceptacion","Nombre del estado");
    }while(name.size() == EMPTY);

    if(pda->getNumOfState(name) == -1){
        pda->newState(name,3);
        Dialog::drawState(pda->getNumStates(),Qt::green,name);
    }else QMessageBox::warning(this,"Error de creacion","El estado ya existe!");
}

void Dialog::on_newCondition_clicked()
{
    pda->setOrigin(ui->origin->text());
    pda->setDestination(ui->destination->text());
    pda->setCondition(ui->tape->text(),ui->pop->text(),ui->push->text());
    int result = pda->newCondition();
    switch(result){
        case CREATEDCONDITION:
            QMessageBox::information(this,"Nueva condicion","Condicion creada exitosamente!");
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

void Dialog::on_validateExpression_clicked()
{
    QString path="";
    QString expression;
    expression = ui->expression->text();
    if(expression.size() != 0){
        QVector<State> s;
        pda->getAcceptanceStates(s);
        if(pda->existInicialState()){
            if(s.size() != 0){
                if(pda->validateExpression(expression)){
                    for(int i=0;i<this->pda->auxPath.size();i++){
                        path=path+pda->auxPath[i]+"->";
                    }
                    QMessageBox::information(this,"Resultado","Expresion aceptada\n"+path);
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
