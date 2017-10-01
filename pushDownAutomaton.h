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


using namespace std;

class PushDownAutomaton {
    private:
        int numStates;
        int maxStates;
        State *states;
        QVector<condition> **matAdj;
        QStack<char> characters;
        QString expression;
        bool isAccepted;
        QString origin, destination, tape;
        condition myCondition;
        QVector<QString> path;

    public:
        PushDownAutomaton(){
            characters.push('#');
            maxStates = 1;
            PushDownAutomaton(maxStates);
            expression="";
        }

        PushDownAutomaton(int max){
            characters.push('#');
            expression="";
            maxStates = max;
            numStates = 0;
            states = new State[max];
            matAdj = new QVector<condition>*[max];

            for(int i = 0;i < max;i++) matAdj[i] = new QVector<condition>[max];
        }

        void setOrigin(QString origin){
            this->origin = origin;
        }

        void setDestination(QString destination){
            this->destination = destination;
        }

        void setCondition(QString tape, QString pop, QString push){
            string auxTape; //Convertir de QString a std string
            this->tape = tape;
            auxTape = tape.toStdString();
            myCondition.tape = auxTape[0];
            myCondition.pop = pop;
            myCondition.push = push;
        }

        int getMaxStates(){
            return this->maxStates;
        }

        int getNumStates(){
            return this->numStates;
        }

        bool existInicialState(){
            for(int i = 0;i < numStates;i++)
                if(states[i].getType() == INITIAL)
                    return true;
            return false;
        }

        State getInitialState(){
            for(int i = 0;i < numStates;i++)
                if(states[i].getType() == INITIAL)
                    return states[i];
        }

        void getAcceptanceStates(QVector<State> &s){
            for(int i = 0;i < numStates;i++)
                if(states[i].getType() == ACCEPTANCE)
                    s.push_back(states[i]);
        }

        void setNumStates(int n){
            numStates = n;
        }

        int getNumOfState(QString name){
            for(int i = 0;i < numStates;i++){
                if(states[i].similar(name))
                    return i;
            }
            return NOEXIST;
        }

        bool newState(QString name, int type){
            if(getNumOfState(name) == NOEXIST){
                if(numStates < maxStates){
                    State s = State(name, numStates, type);
                    states[numStates++] = s;
                    return true;
                }
                return false;
            }
            return false;
        }

        bool existCondition(QString a, QString b, condition c){
            int va = getNumOfState(a);
            int vb = getNumOfState(b);
            if(va == NOEXIST || vb == NOEXIST){
                return false;
            }else{
                for(int i = 0;i < matAdj[va][vb].size();i++)
                    if(matAdj[va][vb][i].tape == c.tape && matAdj[va][vb][i].pop == c.pop && matAdj[va][vb][i].push == c.push)
                        return true;
                return false;
            }
        }

        int newCondition(){
            int origin = getNumOfState(this->origin), destination = getNumOfState(this->destination);
            if(origin == NOEXIST && destination == NOEXIST){
                return NOSTATE;
            }else if(origin == NOEXIST){
                return NOORIGIN;
            }else if(destination == NOEXIST){
                return NODESTINATION;
            }else{
                if(tape == EMPTY || myCondition.pop == EMPTY || myCondition.push == EMPTY){
                    return INVALIDCONDITION;
                }else{
                    if(existCondition(this->origin,this->destination,myCondition)){
                        return EXISTCONDITION;
                    }else{
                        matAdj[origin][destination].push_back(this->myCondition);
                        return CREATEDCONDITION;
                    }

                }
            }
        }

        bool adjacent(QString a, QString b){
            int va =  getNumOfState(a);
            int vb =  getNumOfState(b);
            if(va == NOEXIST || vb == NOEXIST){
                return false;
            }else{
                return !matAdj[va][vb].empty();
            }
        }

        bool adjacent(int a, int b){
            if(a == NOEXIST || b == NOEXIST){
                return false;
            }else{
                return !matAdj[a][b].empty();
            }
        }

        State getState(int x){
            if(x<=NOEXIST || x> numStates){
                qDebug() << "El estado no existe";
            }else{
                return states[x];
            }
        }

        void setState(State v, int va){
            if(va<=NOEXIST || va>numStates){
                qDebug() << "El estado no existe\n";
            }else{
                states[va]=v;
            }
        }

        void seekAcceptance(QString origin, QString destination, int level){
            if(!isAccepted){
                int v = getNumOfState(origin),k,m;
                for(int i = 0;i <= numStates;i++){
                    if(adjacent(v,i)){
                        if(level < expression.size()){
                            for(int j = 0;j < matAdj[v][i].size();j++){
                                if(matAdj[v][i][j].tape == expression[level]){
                                    if(characters.top() == matAdj[v][i][j].pop[0]){
                                        if(states[i].getName() ==  destination && expression.size()-1 == level){
                                            isAccepted=true;
                                            while(!this->characters.empty()){
                                                characters.pop();
                                            }
                                        }else{
                                            string stackData;
                                            bool put=false;
                                            QVector<char> toApile;
                                            toApile.push_back(this->characters.top());
                                            this->characters.pop();
                                            if(matAdj[v][i][j].push != LAMBDA){
                                                put=true;
                                                for(k = 0;k < matAdj[v][i][j].push.size();k++){
                                                    stackData = matAdj[v][i][j].push.toStdString();
                                                    this->characters.push(stackData[k]);
                                                }
                                            }
                                            seekAcceptance(states[i].getName(),destination,level+1);

                                            if(put)
                                                for(k=0;k<matAdj[v][i][j].push.size();k++)
                                                    this->characters.pop();

                                            for(k = 0;k < toApile.size();k++)
                                                this->characters.push(toApile[i]);
                                        }
                                    }/*else{
                                        if(matAdj[v][i][j].pop == LAMBDA){
                                            if(states[i].getName() ==  destination && expression.size()-1 == level){
                                                isAccepted=true;
                                                while(!this->characters.empty()){
                                                    characters.pop();
                                                }
                                            }else{
                                                string stackData;
                                                bool put=false;
                                                if(matAdj[v][i][j].push != LAMBDA){
                                                    put=true;
                                                    for(k = 0;k < matAdj[v][i][j].push.size();k++){
                                                        stackData = matAdj[v][i][j].push.toStdString();
                                                        this->characters.push(stackData[k]);
                                                    }
                                                }
                                                return seekAcceptance(states[i].getName(),destination,level+1);

                                                if(put)
                                                    for(k=0;k<matAdj[v][i][j].push.size();k++)
                                                        this->characters.pop();
                                            }
                                        }
                                    }*/
                                }
                            }
                        }else{
                            for(m = 0;m < matAdj[v][i].size();m++){
                                if(matAdj[v][i][m].tape == '~'){
                                    if(characters.top() == matAdj[v][i][m].pop[0]){
                                        if(states[i].getName() ==  destination && expression.size()-1 == level){
                                            isAccepted=true;
                                            while(!this->characters.empty()){
                                                characters.pop();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        bool validateExpression(QString expression){
            while(!this->characters.empty()){
                this->characters.pop();
            }
            this->characters.push('#');
            QVector<State> s;
            getAcceptanceStates(s);
            this->expression = expression;
            State initial = this->getInitialState();
            isAccepted=false;
            for(int i=0;i<s.size();i++){
                if(!isAccepted) seekAcceptance(initial.getName(),s[i].getName(),0);
            }
            return isAccepted;
        }
};

#endif // PUSHDOWNAUTOMATON_H
