#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include <QInputDialog>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    this->stackAutomaton = new StackAutomaton(25);
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
    if(!stackAutomaton->existInicialState()){
        retryName:
        const QString name = QInputDialog::getText(this,"Estado inicial","Nombre del estado");
        if(name.size() == 0){
            QMessageBox::warning(this,"Error de creacion","El nombre no puede estar vacio");
            goto retryName;
        }
        stackAutomaton->newState(name,1);
        Dialog::drawState(stackAutomaton->getNumStates(),Qt::red,name);
    }else{
        QString name = stackAutomaton->getState(0).getName();
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
    if(stackAutomaton->getNumOfState(name) == -1){
        stackAutomaton->newState(name,2);
        Dialog::drawState(stackAutomaton->getNumStates(),Qt::blue,name);
    }else QMessageBox::warning(this,"Error de creacion","El estado ya existe!");
}

void Dialog::on_acceptanceState_clicked()
{
    retryName:
    const QString name = QInputDialog::getText(this,"Estado de aceptacion","Nombre del estado");
    if(name.size() == 0){
        QMessageBox::warning(this,"Error de creacion","El nombre no puede estar vacio");
        goto retryName;
    }
    if(stackAutomaton->getNumOfState(name) == -1){
        stackAutomaton->newState(name,3);
        Dialog::drawState(stackAutomaton->getNumStates(),Qt::green,name);
    }else QMessageBox::warning(this,"Error de creacion","El estado ya existe!");
}

void Dialog::on_newCondition_clicked()
{
    QString origin = ui->origin->text();
    QString destination = ui->destination->text();
    condition c;
    string temp;
    temp = ui->tape->text().toStdString();
    c.tape = temp[0];
    c.x = ui->pop->text();
    c.y = ui->push->text();
    if(origin.size() == 0 || destination.size() == 0 || temp.size() == 0 || c.x.size() == 0 || c.y.size() == 0){
        QMessageBox::warning(this,"Error de creacion","Todos los campos son obligatorios!");
    }else{
        int a = stackAutomaton->getNumOfState(origin), b = stackAutomaton->getNumOfState(destination);
        if(a != -1 &&  b != -1){
            if(!stackAutomaton->existCondition(origin,destination,c)){
                stackAutomaton->newCondition(origin,destination,c);
                QMessageBox::information(this,"Nueva codicion","La condicion ha sido creada exitosamente!");
            }else{
                stackAutomaton->newCondition(origin,destination,c);
                QMessageBox::information(this,"Nueva codicion","La condicion ya existe!");
            }
        }else{
            if(a == -1){
                QMessageBox::warning(this,"Error de creacion","El estado de origen no existe!");
            }else{
                QMessageBox::warning(this,"Error de creacion","El estado destino no existe!");
            }

        }

    }
}

void Dialog::on_validateExpression_clicked()
{
    QString expression;
    expression = ui->expression->text();
    if(expression.size() != 0){
        QVector<State> s;
        stackAutomaton->getAcceptanceStates(s);
        if(stackAutomaton->existInicialState()){
            if(s.size() != 0){
                if(stackAutomaton->validateExpression(expression)){
                    QMessageBox::information(this,"Resultado","Expresion aceptada!");
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
