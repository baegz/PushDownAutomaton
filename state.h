#ifndef STATE_H
#define STATE_H

#include "condition.h"

class State{
    protected:
        string name;
        int numOfState;
        int type;
    public:
        State(){}

        State(string name){
            this->name = name;
            this->type = 2;
            numOfState = -1;
        }

        string getType(){
            switch(type){
                case 1:
                    return "Inicial";
                case 2:
                    return "Transicion";
                case 3:
                    return "Aceptacion";
                default:
                    return "Sin tipo";
            }
        }

        State(string name, int num, int type){
            this->name = name;
            this->numOfState = num;
            this->type = type;
        }

        void setName(string name){
            this->name = name;
        }

        void setNumOfState(int num){
            this->numOfState = num;
        }

        string getName(){
            return this->name;
        }

        int getNumOfState(){
            return this->numOfState;
        }

        bool similar(State v){
            return v.name == this->name;
        }

        bool similar(string name){
            return name == this->name;
        }
};

#endif // STATE_H
