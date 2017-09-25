#ifndef STACKAUTOMATON_H
#define STACKAUTOMATON_H

#include "state.h"

int password=0xFFFF;
bool isAccepted;

class StackAutomaton{
    protected:
        int numStates;
        int maxStates;
        State *states;
        vector<condition> **matAdj;
        stack<char> characters;
        string expression;
    public:
        StackAutomaton(){
            characters.push('#');
            maxStates = 1;
            StackAutomaton(maxVertex);
            expression="";
        }

        StackAutomaton(int max){
            characters.push('#');
            expression="";
            maxStates = max;
            numStates = 0;
            states = new State[max];
            matAdj = new vector<condition>*[max];
            for(int i = 0;i < max;i++){
                matAdj[i] = new vector<condition>[max];
            }
        }

        int getMaxStates(){
            return this->maxStates;
        }

        int getNumStates(){
            return this->numStates;
        }

        State getInitialState(){
            for(int i=0;i<numStates;i++)
                if(states[i].getType() == "Inicial")
                    return states[i];
        }

        void getAcceptanceStates(vector<State> &s){
            vector<State> v;
            for(int i=0;i<numStates;i++)
                if(states[i].getType() == "Aceptacion")
                    s.push_back(states[i]);
        }

        void setNumStates(int n){
            numStates = n;
        }

        int getNumOfState(string name){
            bool founed=false;
            for(int i = 0;i < numStates && !founed;i++){
                if(states[i].similar(name))
                    return i;
            }
            return -1;
        }

        void newState(string name, int type){
            bool exist = getNumOfState(name)>-1;
            if(!exist){
                State s = State(name, numStates, type);
                states[numStates++] = s;
                cout<<"\n\n\t\tEstado creado exitosamente!";
            }else{
                cout<<"\n\n\t\tEl estado ya existe!";
            }
        }

        void newCondition(string a, string b, condition c){
            int va = getNumOfState(a);
            int vb = getNumOfState(b);
            if(va<0 && vb<0){
                cout<<"\n\n\t\tNo existen los estados";
            }else if(va<0){
                cout<<"\n\n\t\tEl estado origen no existe!";
            }else if(vb<0){
                cout<<"\n\n\t\tEl estado destino no existe!";
            }else{
                matAdj[va][vb].push_back(c);
                cout<<"\n\n\t\tCondicion creada correctamente!";
            }
        }

        bool adjacent(string a, string b){
            int va =  getNumOfState(a);
            int vb =  getNumOfState(b);
            if(va<0 || vb<0){
                return false;
            }else{
                return !matAdj[va][vb].empty();
            }
        }

        bool adjacent(int a, int b){
            if(a< 0 || b<0){
                return false;
            }else{
                return !matAdj[a][b].empty();
            }
        }

        State getState(int x){
            if(x<0 || x> numStates){
                cout<<"This vertex don't exist!\n";
            }else{
                return states[x];
            }
        }

        void setState(State v, int va){
            if(va<0 || va>numStates){
                cout<<"This vertex don't exist!\n";
            }else{
                states[va]=v;
            }
        }

        void printStates(){
            if(numStates < 1){
                cout<<"No hay estados!";
            }else{
                cout<<"Estados: {";
                for(int i=0; i<numStates; i++){
                    cout<<"("<<states[i].getName()<<","<<states[i].getType()<<")";
                    if(i != numStates-1)
                        cout<<" - ";
                }
                cout<<"}";
            }
        }

        void printConditions(){
            for(int i=0;i<numStates;i++){
                for(int j=0;j<numStates;j++){
                    if(matAdj[i][j].size() > 0){
                        cout<<"\nCondiciones de \""<<states[i].getName()<<"\" hacia \""<<states[j].getName()<<"\"\n";
                        for(int k=0;k<matAdj[i][j].size();k++){
                            cout<<matAdj[i][j][k].tape<<", "<<matAdj[i][j][k].x<<"/ "<<matAdj[i][j][k].y<<endl;
                        }
                    }
                }
            }
        }

        bool seekAcceptance(string origin, string destination, int level){
            cout<<"Estado origen es: "<<origin<<endl;
            if(!isAccepted){
                int v = getNumOfState(origin);
                for(int i=0;i<=numStates;i++){
                    if(adjacent(v,i)){ //Verifica si existen condiciones de un estado a otro
                        bool possible = false;
                        if(level < expression.size()){
                            for(int j=0;j<matAdj[v][i].size();j++){ //Vector de condiciones en esa posicion de la matriz
                                if(matAdj[v][i][j].tape == expression[level]){ //
                                    possible = true;
                                    if(characters.top() == matAdj[v][i][j].x[0]){
                                        cout<<"\nLa cinta y la pila coinciden!\n";
                                        if(states[i].getName() ==  destination){
                                            isAccepted=true;
                                            cout<<"Aceptado en: "<<destination;
                                            cout<<"\nEstado final de la pila:\n";
                                            while(!this->characters.empty()){
                                                cout<<this->characters.top()<<endl;
                                                characters.pop();
                                            }
                                            return isAccepted;
                                        }else{
                                            int k;
                                            bool put=false;
                                            vector<char> toApile;
                                            toApile.push_back(this->characters.top());
                                            this->characters.pop(); //Quitando de la pila

                                            if(matAdj[v][i][j].y != "~"){ //Si es diferende lamda puedo apilar
                                                put=true;
                                                for(k=0;k<matAdj[v][i][j].y.size();k++){
                                                    cout<<"apilando: "<<matAdj[v][i][j].y[k]<<endl;
                                                    this->characters.push(matAdj[v][i][j].y[k]);
                                                }
                                            }
                                            return seekAcceptance(states[i].getName(),destination,level+1);
                                            if(put){
                                                for(k=0;k<matAdj[v][i][j].y.size();k++)
                                                    this->characters.pop();
                                            }

                                            for(k=0;k<toApile.size();k++){
                                                this->characters.push(toApile[i]);
                                            }
                                        }
                                    }
                                }
                            }
                        }else{
                            //cout<<"\nSe han habilitado las oscilaciones epsilon!";
                            for(int m=0;m<matAdj[v][i].size();m++){
                                if(matAdj[v][i][m].tape == '~'){
                                    if(characters.top() == matAdj[v][i][m].x[0]){
                                        //cout<<"\nEstoy en el estado: "<<origin;
                                        //cout<<"\nTengo una osilacion epsilon que hacer!\nFUE ACEPTADA!";
                                        if(states[i].getName() ==  destination){
                                            isAccepted=true;
                                            //cout<<"\nEstado final de la pila:\n";
                                            cout<<"Aceptada en: "<<destination<<" por Epsilon!!!!\n";
                                            while(!this->characters.empty()){
                                                cout<<this->characters.top()<<endl;
                                                characters.pop();
                                            }
                                            return isAccepted;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        bool validateExpression(string expression){
            while(!this->characters.empty()){
                this->characters.pop();
            }
            this->characters.push('#');
            vector<State> s;
            getAcceptanceStates(s);
            this->expression = expression;
            State initial = this->getInitialState();
            isAccepted=false;
            for(int i=0;i<s.size();i++){
                seekAcceptance(initial.getName(),s[i].getName(),0);
            }
            return isAccepted;
        }
};

#endif // STACKAUTOMATON_H
