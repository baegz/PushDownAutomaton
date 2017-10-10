#ifndef STATE_H
#define STATE_H

#include <condition.h>
#include <QString>

#define INITIAL 1
#define TRANSITION 2
#define ACCEPTANCE 3

class State
{
public:
    State(){}
    State(QString name);
    State(QString name, int num, int type);

    bool similar(State v);
    bool similar(QString name);

    int getNumOfState();
    int getType();

    QString getName();

    void setName(const QString name);
    void setNumOfState(int num);

private:
    QString name;
    int numOfState;
    int type;
};

#endif // STATE_H
