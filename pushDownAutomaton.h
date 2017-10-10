#ifndef PUSHDOWNAUTOMATON_H
#define PUSHDOWNAUTOMATON_H

#include <QDebug>
#include <QCharRef>
#include <QVector>
#include <QStack>
#include <QString>
#include <state.h>

#define EMPTY 0
#define NOEXIST -1
#define CREATEDCONDITION 1
#define NOSTATE 2
#define NOORIGIN 3
#define NODESTINATION 4
#define EXISTCONDITION 5
#define INVALIDCONDITION 6
#define LAMBDA "~"
#define CLAMBDA '~'


using namespace std;

class PushDownAutomaton
{
public:
    //Constructores
    PushDownAutomaton();
    PushDownAutomaton(int max);

    bool adjacent(QString a, QString b);
    bool adjacent(int a, int b);
    bool existCondition(QString a, QString b, condition c);
    bool existInicialState();
    bool existAcceptanceState();
    bool newState(QString name, int type);
    bool seekAcceptance(QString origin, QString destination, int level, QStack<char> characters);
    bool validateExpression(QString expression);

    int getNumOfState(QString name);
    int getNumStates();
    int getMaxStates();
    int newCondition();

    QString getConditions();
    QString getAcceptanceRoute();

    State getInitialState();
    State getState(int x);

    void getAcceptanceStates(QVector<State> &s);
    void setCondition(QString tape, QString pop, QString push);
    void setDestination(QString destination);
    void setOrigin(QString origin);
    void setNumStates(int n);
    void setState(State v, int va);

private:
    int numStates;
    int maxStates;
    State *states;
    QVector<condition> **matAdj;
    QStack<char> characters;
    QString expression;
    QString origin, destination, tape;
    condition myCondition;
    QVector<QString> path;
    QList<QString> route, acceptanceRoute;
    bool isAccepted;
};

#endif // PUSHDOWNAUTOMATON_H
