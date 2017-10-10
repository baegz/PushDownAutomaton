#include <state.h>


State::State(QString name)
{
    this->name = name;
    this->type = 2;
    numOfState = -1;
}

int State::getType()
{
    return type;return type;
}

State::State(QString name, int num, int type)
{
    this->name = name;
    this->numOfState = num;
    this->type = type;
}

void State::setName(const QString name)
{
    this->name = name;
}

void State::setNumOfState(int num)
{
    this->numOfState = num;
}

QString State::getName()
{
    return this->name;
}

int State::getNumOfState()
{
    return this->numOfState;
}

bool State::similar(State v)
{
    return v.name == this->name;
}

bool State::similar(QString name)
{
    return name == this->name;
}
