#include <pushDownAutomaton.h>

PushDownAutomaton::PushDownAutomaton()
{
    characters.push('#');
    maxStates = 1;
    PushDownAutomaton(maxStates);
    expression="";
}

PushDownAutomaton::PushDownAutomaton(int max)
{
    this->isAccepted = false;
    characters.push('#');
    expression="";
    maxStates = max;
    numStates = 0;
    states = new State[max];
    matAdj = new QVector<condition>*[max];
    for(int i = 0;i < max;i++) matAdj[i] = new QVector<condition>[max];
}

QString PushDownAutomaton::getConditions()
{
    QString conditions="";
    for(int i=0;i<numStates;i++){
        for(int j=0;j<numStates;j++){
            if(matAdj[i][j].size() > 0){
                for(int k=0;k<matAdj[i][j].size();k++){
                    conditions = conditions+"Δ ("+states[i].getName()+",  ";
                    conditions[conditions.size()-1]=matAdj[i][j][k].tape;
                    conditions+=", "+matAdj[i][j][k].pop+") = ("+states[j].getName()+", ";
                    conditions+=matAdj[i][j][k].push+")\n";
                }
            }
        }
    }
    conditions[conditions.size()-1]='\0';
    return conditions;
}

QString PushDownAutomaton::getAcceptanceRoute()
{
    QString route="";
    for(int i=0;i<acceptanceRoute.size();i++){
        route=route+acceptanceRoute[i];
        if(i != acceptanceRoute.size()-1){
            route+=" -> ";
        }
    }
    return route;
}

void PushDownAutomaton::setOrigin(QString origin)
{
    this->origin = origin;
}

void PushDownAutomaton::setDestination(QString destination)
{
    this->destination = destination;
}

void PushDownAutomaton::setCondition(QString tape, QString pop, QString push)
{
    string auxTape; //Convertir de QString a std string
    this->tape = tape;
    auxTape = tape.toStdString();
    myCondition.tape = auxTape[0];
    myCondition.pop = pop;
    myCondition.push = push;
}

int PushDownAutomaton::getMaxStates()
{
    return this->maxStates;
}

int PushDownAutomaton::getNumStates()
{
    return this->numStates;
}

bool PushDownAutomaton::existInicialState()
{
    for(int i = 0;i < numStates;i++)
        if(states[i].getType() == INITIAL)
            return true;
    return false;
}

bool PushDownAutomaton::existAcceptanceState()
{
    for(int i = 0;i < numStates;i++)
        if(states[i].getType() == ACCEPTANCE)
            return true;
    return false;
}

State PushDownAutomaton::getInitialState()
{
    for(int i = 0;i < numStates;i++)
        if(states[i].getType() == INITIAL)
            return states[i];
}

void PushDownAutomaton::getAcceptanceStates(QVector<State> &s)
{
    for(int i = 0;i < numStates;i++)
        if(states[i].getType() == ACCEPTANCE)
            s.push_back(states[i]);
}

void PushDownAutomaton::setNumStates(int n)
{
    numStates = n;
}

int PushDownAutomaton::getNumOfState(QString name)
{
    for(int i = 0;i < numStates;i++){
        if(states[i].similar(name))
            return i;
    }
    return NOEXIST;
}

bool PushDownAutomaton::newState(QString name, int type)
{
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

bool PushDownAutomaton::existCondition(QString a, QString b, condition c)
{
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

int PushDownAutomaton::newCondition()
{
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

bool PushDownAutomaton::adjacent(QString a, QString b)
{
    int va =  getNumOfState(a);
    int vb =  getNumOfState(b);
    if(va == NOEXIST || vb == NOEXIST){
        return false;
    }else{
        return !matAdj[va][vb].empty();
    }
}

bool PushDownAutomaton::adjacent(int a, int b)
{
    if(a == NOEXIST || b == NOEXIST){
        return false;
    }else{
        return !matAdj[a][b].empty();
    }
}

State PushDownAutomaton::getState(int x)
{
    if(x<=NOEXIST || x> numStates){
        qDebug() << "El estado no existe";
    }else{
        return states[x];
    }
}

void PushDownAutomaton::setState(State v, int va)
{
    if(va<=NOEXIST || va>numStates){
        qDebug() << "El estado no existe\n";
    }else{
        states[va]=v;
    }
}

bool PushDownAutomaton::seekAcceptance(QString origin, QString destination, int level, QStack<char> characters)
{
    if(!isAccepted){
        int v = getNumOfState(origin);
        for(int i=0;i<=numStates;i++){
            if(adjacent(v,i)){ //Verifica si existen condiciones de un estado a otro
                if(level < expression.size()){
                    for(int j=0;j<matAdj[v][i].size();j++){ //Vector de condiciones en esa posicion de la matriz
                        if(matAdj[v][i][j].tape == expression[level]){ //
                            if(characters.top() == matAdj[v][i][j].pop[0]){
                                if(states[i].getName() ==  destination && level == expression.size()-1){
                                    isAccepted=true;
                                    while(!characters.empty()){
                                        characters.pop();
                                    }
                                    route.append(states[i].getName());
                                    for(int z=0;z<route.size();z++){
                                        acceptanceRoute.push_back(route[z]);
                                    }
                                    return isAccepted;
                                }else{
                                    string stackData;
                                    int k;
                                    bool put=false;
                                    vector<char> toApile;
                                    toApile.push_back(characters.top());
                                    characters.pop(); //Quitando de la pila

                                    if(matAdj[v][i][j].push != LAMBDA){ //Si es diferende lamda puedo apilar
                                        put=true;
                                        for(k=0;k<matAdj[v][i][j].push.size();k++){
                                            stackData = matAdj[v][i][j].push.toStdString();
                                            characters.push(stackData[k]);
                                        }
                                    }
                                    route.push_back(states[i].getName());
                                    seekAcceptance(states[i].getName(),destination,level+1, characters);
                                    route.pop_back();
                                    if(put){
                                        for(k=0;k<matAdj[v][i][j].push.size();k++)
                                            characters.pop();
                                    }

                                    for(k=0;k<toApile.size();k++){
                                        characters.push(toApile[k]);
                                    }
                                }
                            }else{
                                if(matAdj[v][i][j].tape == CLAMBDA){
                                    string stackData;
                                    int k;
                                    bool put=false;
                                    vector<char> toApile;
                                    toApile.push_back(characters.top());
                                    characters.pop(); //Quitando de la pila

                                    if(matAdj[v][i][j].push != LAMBDA){ //Si es diferende lamda puedo apilar
                                        put=true;
                                        for(k=0;k<matAdj[v][i][j].push.size();k++){
                                            stackData = matAdj[v][i][j].push.toStdString();
                                            characters.push(stackData[k]);
                                        }
                                    }
                                    route.push_back(states[i].getName());
                                    seekAcceptance(states[i].getName(),destination,level, characters);
                                    route.pop_back();
                                    if(put){
                                        for(k=0;k<matAdj[v][i][j].push.size();k++)
                                            characters.pop();
                                    }

                                    for(k=0;k<toApile.size();k++){
                                        characters.push(toApile[k]);
                                    }
                                }
                            }
                        }
                    }
                }else{
                    for(int m=0;m<matAdj[v][i].size();m++){
                        if(matAdj[v][i][m].tape == '~'){
                            if(characters.top() == matAdj[v][i][m].pop[0]){
                                if(states[i].getName() ==  destination){
                                    isAccepted=true;
                                    while(!characters.empty()){
                                        characters.pop();
                                    }
                                    route.push_back(states[i].getName());
                                    for(int z=0;z<route.size();z++){
                                        acceptanceRoute.push_back(route[z]);
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

bool PushDownAutomaton::validateExpression(QString expression)
{
    isAccepted=false;
    QStack<char> characters;
    while(!acceptanceRoute.empty()) acceptanceRoute.pop_front();
    while(!route.empty()) route.pop_front();
    characters.push('#');

    QVector<State> s;
    getAcceptanceStates(s);

    this->expression = expression;

    State initial = this->getInitialState();
    route.append(initial.getName());

    for(int i=0;i<s.size() && !isAccepted;i++){
        seekAcceptance(initial.getName(),s[i].getName(),0,characters);
    }
    return isAccepted;
}
