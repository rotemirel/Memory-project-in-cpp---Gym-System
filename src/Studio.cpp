//
// Created by spl211 on 07/11/2021.
//
#include "Studio.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include "Workout.h"
#include "Action.h"
#include <vector>
#include <string>


using std::ifstream;

Studio::Studio():open(true){}
Studio::Studio(const std::string &configFilePath):open(true){ // help us to run this method
    int  counter=0;
    std::ifstream file(configFilePath);
    int workout_id =0;
    char line[256];
    char* temp_cap;
    char* temp_info;


    while(file){

        file.getline(line,256);
        if(line[0]=='#'||line[0]=='\0'||line[0]=='\n')
            continue;

        counter++;

        if(counter==1){
            std::stoi(line);
        }

        else if(counter==2){

            int cap;
            temp_cap = strtok(line," ,");
            while(temp_cap != nullptr) {
                cap = std::atoi(temp_cap);


                temp_cap = strtok(nullptr, " ,");
                Trainer *trainer = new Trainer(cap);
                trainers.push_back(trainer);

            }
        }

        else{
            std::string name;
            int price;
            WorkoutType type;
            temp_info= strtok(line,",");

            for(int i=0 ; i<3 && temp_info != nullptr ; i++) {
                if (i == 0) {
                        name = temp_info;
                }
                else if(i==1) {
                    type = FindType(temp_info);
                }
                else {
                    price = std::atoi(temp_info);
                }


                temp_info = strtok(nullptr, " ,");

            }
            Workout workout(workout_id , name, price, type);
            workout_options.push_back(workout);
            workout_id++;
            }

        }
        (file).close();


    }


// copy constructor
Studio::Studio(const Studio& other):open(other.isOpen()){
    copy(other);
}
// copy assignment
 Studio& Studio::operator=(const Studio& other){
    if(this!= &other){
        clear();
        trainers.clear();
        actionsLog.clear();
        workout_options.clear();
        copy(other);
    }
    return *this;
}
// destructor
Studio :: ~Studio(){
    clear();
}
// move constructor
Studio::Studio(Studio&& other) noexcept :open(other.open){
    for(Trainer* t:other.trainers){
        trainers.push_back(t);
        t= nullptr;
    }
    for(BaseAction* b:other.getActionsLog()){
        actionsLog.push_back(b);
        b= nullptr;
    }
    for(const Workout& w:other.workout_options){
        workout_options.push_back(w);
    }

}

//move assignment
Studio& Studio::operator=(Studio&& other) noexcept {

    if(this!=&other) {
        clear();
        for (Trainer *t: other.trainers) {
            trainers.push_back(t);
            t = nullptr;
        }
        for (BaseAction *b: other.getActionsLog()) {
            actionsLog.push_back(b);
            b = nullptr;
        }
        for (const Workout& w: other.workout_options) {
            workout_options.push_back(w);
        }
    }
    return *this;

}
//copy
void Studio::copy(const Studio& other){

    for(Trainer* t:other.trainers ){
        auto* curr_t = new Trainer(*t);
        trainers.push_back(curr_t);
    }
    for(const Workout& w:other.workout_options)
        workout_options.push_back(w);
    for(BaseAction* i : other.getActionsLog()){
         actionsLog.push_back(i->clone());
    }

}
//clear
void Studio::clear(){
    for(Trainer* t: trainers) {
        delete t;
    }
    for(BaseAction* b : actionsLog){
        delete b;
    }
}


WorkoutType Studio::FindType (const char* _type) {
    if (_type[0] =='a' ||_type[0] =='A')
        return ANAEROBIC;
    else if (_type[0] =='c' ||_type[0] =='C')
        return CARDIO;
    else
        return MIXED;
}

void Studio::start(){
    int customer_id=0;//counter
    open= true;
    std::cout<<"Studio is now open!\n";

    std::string input;
    std::getline(std::cin,input);
    char line[256];
    std::strcpy(line,input.c_str());
    char* action;
    action=std::strtok(line," ,");
    bool flag= true;
    while(isOpen()){

        if(strcmp((char*)"open",action)==0){
            std::string customer_name;
            std::string customer_strategy;
            std::vector<Customer*> _customers;
            action = strtok(nullptr, " ,");
            int trainer_id= std::atoi(action);
            int capacity = getTrainer(trainer_id)->getCapacity();
            int i=0;
            while(action != nullptr&& i<capacity) {

                action = strtok(nullptr, " ,");
                if (action != nullptr) {
                    customer_name = action;
                    action = strtok(nullptr, " ,");
                    customer_strategy = action;
                    Customer *c;

                    if (customer_strategy == "swt") {
                        c = new SweatyCustomer(customer_name, customer_id++);
                    } else if (customer_strategy == "chp") {
                        c = new CheapCustomer(customer_name, customer_id++);
                    } else if (customer_strategy == "mcl") {
                        c = new HeavyMuscleCustomer(customer_name, customer_id++);
                    } else {
                        c = new FullBodyCustomer(customer_name, customer_id++);
                    }
                    _customers.push_back(c);
                    i++;
                }
            }
                OpenTrainer *op = new OpenTrainer(trainer_id, _customers);
                for(Customer* c:_customers)
                    delete c;
                op->act(*this);

                actionsLog.push_back(op);

        }
        else if(strcmp((char*)"order",action)==0){
            int trainer_id;
            action = strtok(nullptr, " ,");
            trainer_id=std::atoi(action);
            Order* o=new Order(trainer_id);
            o->act(*this);
            actionsLog.push_back(o);

        }
        else if(strcmp((char*)"move",action)==0){
            int src_id;
            int dst_id;
            int _id;

            action = strtok(nullptr, " ,");
            src_id=std::atoi(action);
            action = strtok(nullptr, " ,");
            dst_id=std::atoi(action);
            action = strtok(nullptr, " ,");
            _id=std::atoi(action);

            MoveCustomer* m_c =new MoveCustomer(src_id,dst_id,_id);
            m_c->act(*this);
            actionsLog.push_back(m_c);

        }
        else if(strcmp((char*)"close",action)==0){
            int trainer_id;
            action = strtok(nullptr, " ,");
            trainer_id=std::atoi(action);

            Close* close=new Close(trainer_id);
            close->act(*this);
            actionsLog.push_back(close);
        }
        else if(strcmp((char*)"closeall",action)==0){
            CloseAll* closeAll =new CloseAll();
            closeAll->act(*this);
            flag= false;
            delete closeAll;
        }
        else if(strcmp((char*)"workout_options",action)==0){
            PrintWorkoutOptions* pwo=new PrintWorkoutOptions();
            pwo->act(*this);
            actionsLog.push_back(pwo);
        }
        else if(strcmp((char*)"status",action)==0){
            int trainer_id;
            action = strtok(nullptr, " ,");
            trainer_id=std::atoi(action);

            PrintTrainerStatus* pts=new PrintTrainerStatus(trainer_id);
            pts->act(*this);
            actionsLog.push_back(pts);
        }
        else if(strcmp((char*)"log",action)==0){
            PrintActionsLog* pal= new PrintActionsLog();
            pal->act(*this);
            actionsLog.push_back(pal);
        }
        else if(strcmp((char*)"backup",action)==0){
            BackupStudio* bus= new BackupStudio();
            bus->act(*this);
            actionsLog.push_back(bus);
        }
        else if(strcmp((char*)"restore",action)==0){
            RestoreStudio* res= new RestoreStudio();
            res->act(*this);
            actionsLog.push_back(res);
        }
        if(flag){
            std::getline(std::cin, input);
            std::strcpy(line, input.c_str()); //check deletion
            action = std::strtok(line, " ,");
        }
    }

}

int Studio::getNumOfTrainers() const{return  trainers.size();}

Trainer* Studio::getTrainer(int tid) {
    if (tid >= getNumOfTrainers())
        return nullptr;
    else
        return trainers[tid];
}

const std::vector<BaseAction*>& Studio::getActionsLog() const{return actionsLog;}

std::vector<Workout>& Studio:: getWorkoutOptions(){return workout_options;}

void Studio::set_open(bool status){ open=status;}

bool Studio::isOpen () const{
    return open;
}



