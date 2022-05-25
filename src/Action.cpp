//
// Created by spl211 on 07/11/2021.
//
#include "Action.h"
#include "Customer.h"
#include "Trainer.h"
#include "Studio.h"
#include <iostream>


BaseAction::BaseAction():status(COMPLETED){}
ActionStatus BaseAction:: getStatus() const{return status;}

void BaseAction::complete(){
    status=COMPLETED;
}
void BaseAction:: error(std::string errorMsg){
    status=ERROR;
    std::cout<< "Error: "<<errorMsg;
    this->errorMsg=errorMsg;
}
std::string BaseAction:: getErrorMsg() const{return errorMsg;}




//OpenTrainer
OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList):trainerId(id){
    copy_customers_list(customersList);
}

void OpenTrainer:: act(Studio &studio){
    if (trainerId >= studio.getNumOfTrainers() || (studio.getTrainer(trainerId))->isOpen()){
        error("Workout session does not exist or is already open");
        std::cout<<'\n';

        for(size_t i=0;i< customers.size() ;i++) {
            delete customers[i];
        }
        customers.clear();
    }
    else {
        Trainer* t=(studio.getTrainer(trainerId));

        // deep copy customer list
        for(size_t i=0;i< customers.size() ;i++){
            int capacity=t->getCapacity();
            if( i<(size_t)capacity)
                t->getCustomers().push_back(customers[i]->clone());
            delete customers[i];

        }
        customers.clear();
        complete();
        t->openTrainer();
         complete();
    }

}

std::string OpenTrainer::toString() const{

    std::string stat = "open " + std::to_string(trainerId);
    for (Customer *c: customers)
        stat += " " + c->getName() + "," + c->getType();

    if (getStatus() == COMPLETED)
        stat += " Completed";
    else
        stat += " Error: "+getErrorMsg();

    return stat;
}
OpenTrainer:: ~OpenTrainer (){
    for(Customer* c: customers)
        delete c;
}
BaseAction* OpenTrainer:: clone(){
    return new OpenTrainer(*this);
}

//Order
Order :: Order(int id):trainerId(id){}

void Order::act(Studio &studio){

    if (trainerId >= studio.getNumOfTrainers() || !((studio.getTrainer(trainerId))->isOpen())){
        error("Trainer does not exist or is not open");
        std::cout<<'\n';
    }

    else {

        Trainer* t=(studio.getTrainer(trainerId));

        for (Customer *c: t->getCustomers()) {

            std::vector<int> workout_id = c->order(studio.getWorkoutOptions());
            if(t->get_ordered())
                t->order(c->getId(), workout_id, studio.getWorkoutOptions());
            for (int i: workout_id) {
                std::cout << c->getName() << " Is Doing " << (studio.getWorkoutOptions()[i]).getName() << "\n";
            }
        }
        t->set_before_order(false);


        complete();
    }

}

std::string Order::toString() const{
    std::string stat="order "+std::to_string(trainerId)+" ";
    if (getStatus() == COMPLETED)
        stat += "Completed";

    else
        stat += "Error";

    return stat;
}

BaseAction* Order:: clone(){
    return new Order(*this);
}

//moveCustomer
MoveCustomer::MoveCustomer(int src, int dst, int customerId):srcTrainer(src), dstTrainer(dst), id(customerId){}

void MoveCustomer::act(Studio &studio){
    if(dstTrainer>=studio.getNumOfTrainers()||srcTrainer>=studio.getNumOfTrainers()){
        error("cannot move customer");
        std::cout<<'\n';
    }
    else{
        Trainer* dst_trainer=studio.getTrainer(dstTrainer);
        Trainer* src_trainer=studio.getTrainer(srcTrainer);
        Customer* c=src_trainer->getCustomer(id);

        if(!dst_trainer->isOpen()||!src_trainer->isOpen()||
                (size_t)(dst_trainer->getCapacity())==dst_trainer->getCustomers().size()|| c==nullptr){
            error("cannot move customer");
            std::cout<<'\n';
        }
        else{
            dst_trainer->addCustomer(c);
            for (const OrderPair& o: src_trainer->getOrders()){
                if (o.first == id ) {
                    dst_trainer->getOrders().push_back(o);
                    dst_trainer->add_salary(o.second.getPrice());
                    src_trainer->add_salary(o.second.getPrice()*(-1));
                }
            }
            src_trainer->removeCustomer(id);
            if(src_trainer->getCustomers().empty()){
                Close close(srcTrainer);
                close.act(studio);
            }

            complete();
        }
        
    }
}
std::string MoveCustomer::toString() const{
    std::string stat="move "+std::to_string(srcTrainer)+" "+std::to_string(dstTrainer)+" "+std::to_string(id);
    if (getStatus() == COMPLETED)
        stat += " Completed";
    else
        stat += " Error "+getErrorMsg();

    return stat;
}

BaseAction* MoveCustomer:: clone(){
    return new MoveCustomer(*this);
}

//Close
Close::Close(int id):trainerId(id){}

void Close::act(Studio &studio){
    Trainer* t = studio.getTrainer(trainerId);
    if (!t || !(t->isOpen())){
        error("Trainer does not exist or is not open");
        std::cout<<'\n';
    }
    else{
        for(Customer* c:t->getCustomers())
            delete c;
        t->getCustomers().clear();
        t->getOrders().clear();
        t->closeTrainer();
        t->set_before_order(true);
        std::cout << "Trainer " << std::to_string(trainerId) << " closed. Salary " << std::to_string(t->getSalary())<<"NIS\n";
        complete();
    }
}

std::string Close::toString() const{
    std::string stat="close "+std::to_string(trainerId);
    if (getStatus() == COMPLETED)
        stat += " Completed";

    else
        stat += " Error "+getErrorMsg();

    return stat;
}

BaseAction* Close:: clone(){
    return new Close(*this);
}

//CloseAll
CloseAll::CloseAll(){}

void CloseAll::act(Studio &studio){
    Trainer* t;

    for(int i = 0 ; i < studio.getNumOfTrainers() ; i++){
        t = studio.getTrainer(i);
        if (t->isOpen()) {

            t->closeTrainer();
            std::cout << "Trainer " << std::to_string(i) << " closed. Salary " << std::to_string(t->getSalary())<<"NIS\n";

        }
    }

    studio.set_open(false);
    complete();

}

std::string CloseAll::toString() const{
    return "closeall Completed";
}

BaseAction* CloseAll:: clone(){
    return new CloseAll(*this);
}


//PrintWorkoutOptions
PrintWorkoutOptions::PrintWorkoutOptions(){}

void PrintWorkoutOptions::act(Studio &studio){

    for(const Workout& w:studio.getWorkoutOptions()){
        std::cout<<w.getName()<<", "<<w.tostring_type()<<", "<<std::to_string(w.getPrice())+"\n";
    }
    complete();
}

std::string PrintWorkoutOptions:: toString() const{
    return  "workout_options Completed";
}


BaseAction* PrintWorkoutOptions:: clone(){
    return new PrintWorkoutOptions(*this);
}

//PrintTrainerStatus
PrintTrainerStatus::PrintTrainerStatus(int id):trainerId(id){}

void PrintTrainerStatus:: act(Studio &studio){
    Trainer* t=studio.getTrainer(trainerId);
    if(!t->isOpen())
        std::cout<<"Trainer "<<std::to_string(trainerId)<<" status: closed\n";
    else{
        std::cout<<"Trainer "<<std::to_string(trainerId)<<" status: open\n";
        std::cout<<"Customers:\n";
        for(Customer* c:t->getCustomers()){
            std::cout<<c->toString()<<'\n';
        }
        std::cout<<"Orders:\n";
        for(const OrderPair& o:t->getOrders()){
            Workout w=o.second;
            std::cout<<w.getName()<<" "<<w.getPrice()<<"NIS "<<std::to_string(o.first)<<'\n';
        }
        std::cout<<"Current Trainer's Salary: "<<std::to_string(t->getSalary())<<"NIS\n";
    }
    complete();
}

std::string PrintTrainerStatus:: toString() const{
    return "status "+std::to_string(trainerId)+" Completed";
}

BaseAction* PrintTrainerStatus:: clone(){
    return new PrintTrainerStatus(*this);
}

//PrintActionsLog
PrintActionsLog::PrintActionsLog(){}

void PrintActionsLog::act(Studio &studio){

    for(BaseAction* b:studio.getActionsLog()){
        std::cout<<b->toString()<<'\n';
    }
    complete();
}

std::string PrintActionsLog::toString() const{
    return "log Completed";
}

BaseAction* PrintActionsLog:: clone(){
    return new PrintActionsLog(*this);
}

//BackupStudio
BackupStudio::BackupStudio(){}

void BackupStudio::act(Studio &studio){// backup!= null

    if(backup!= nullptr){
       // delete backup;
       // backup= nullptr;
        *backup=(studio);
    }
    else
        backup=new Studio(studio);
    // opera
    complete();
}

std::string BackupStudio:: toString() const{
    return "backup Completed";
}

BaseAction* BackupStudio::clone(){
    return new BackupStudio(*this);
}


RestoreStudio::RestoreStudio(){}

void RestoreStudio:: act(Studio &studio) {
    if(backup== nullptr) {
        error("No backup available");
        std::cout<<'\n';
    }
    else{
        studio=*backup;
    }
}

std::string RestoreStudio::toString() const{
    std::string str= "restore ";
    if(getStatus()==COMPLETED)
        str+="Completed";
    else
        str+="Error: "+getErrorMsg();

    return str;
}

BaseAction* RestoreStudio::clone(){
    return new RestoreStudio(*this);
}




void OpenTrainer:: copy_customers_list(const std::vector<Customer*>& _customersList){

    for(Customer* c: _customersList){
        customers.push_back(c->clone());

    }

}