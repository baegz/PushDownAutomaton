#ifndef STATE_H
#define STATE_H

#include <condition.h>

#define INITIAL 1
#define TRANSITION 2
#define ACCEPTANCE 3

class State{
    protected:
        QString name;
        int numOfState;
        int type;
    public:
        State(){}

        State(QString name){
            this->name = name;
            this->type = 2;
            numOfState = -1;
        }

        int getType(){
            return type;
        }

        State(QString name, int num, int type){
            this->name = name;
            this->numOfState = num;
            this->type = type;
        }

        void setName(const QString name){
            this->name = name;
        }

        void setNumOfState(int num){
            this->numOfState = num;
        }

        QString getName(){
            return this->name;
        }

        int getNumOfState(){
            return this->numOfState;
        }

        bool similar(State v){
            return v.name == this->name;
        }

        bool similar(QString name){
            return name == this->name;
        }
};

#endif // STATE_H
