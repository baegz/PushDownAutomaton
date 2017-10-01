#include <dialog.h>
#include <ui_dialog.h>
#include <QMessageBox>
#include <QInputDialog>

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
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    movingState = true;
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_initialState_clicked(){
    if(!pda->existInicialState()){
        QString name = "q"+QString::number(pda->getNumStates());
        if(pda->newState(name,INITIAL)){
            GraphicState *state = new GraphicState(name,INITIAL);
            state->setPos(Dialog::randomBetween(-350,350),Dialog::randomBetween(-170,170));
            scene->addItem(state);
            ui->initialState->setEnabled(false);
        }else
            QMessageBox::critical(this,ERRORNEWSTATE,LIMITSTATE);
    }
}

void Dialog::on_normalState_clicked(){
    QString name = "q"+QString::number(pda->getNumStates());

    if(pda->newState(name,TRANSITION)){
        GraphicState *state = new GraphicState(name,TRANSITION);
        state->setPos(Dialog::randomBetween(-350,350),Dialog::randomBetween(-170,170));
        scene->addItem(state);
    }else
        QMessageBox::critical(this,ERRORNEWSTATE,LIMITSTATE);
}

void Dialog::on_acceptanceState_clicked(){
    QString name = "q"+QString::number(pda->getNumStates());

    if(pda->newState(name,ACCEPTANCE)){
        GraphicState *state = new GraphicState(name,ACCEPTANCE);
        state->setPos(Dialog::randomBetween(-350,350),Dialog::randomBetween(-170,170));
        scene->addItem(state);
    }else
        QMessageBox::critical(this,ERRORNEWSTATE,LIMITSTATE);
}

void Dialog::on_newCondition_clicked(){
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

void Dialog::on_validateExpression_clicked(){
    QString expression;
    expression = ui->expression->text();

    if(expression.size() != EMPTY){
        QVector<State> s;
        pda->getAcceptanceStates(s);

        if(pda->existInicialState()){
            if(s.size() != EMPTY){
                if(pda->validateExpression(expression)){
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
