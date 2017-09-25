#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    stackAutomaton = new StackAutomaton(25);
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}
