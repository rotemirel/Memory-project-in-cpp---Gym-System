//
// Created by spl211 on 07/11/2021.
//
#include <iostream>
#include "Trainer.h"
#include <vector>
#include "Customer.h"
#include "Workout.h"
using namespace std;

//constructor
Trainer::Trainer(int t_capacity):capacity(t_capacity),open(false) ,salary(0),before_order(true){
}


// copy constructor
Trainer::Trainer(const Trainer& other):capacity(other.capacity),open(other.open),salary(other.salary),before_order(other.get_ordered()){
}

// copy assignment
Trainer& Trainer:: operator=(const Trainer& other){
    if(this!= &other)
    {
        clear();
        before_order=other.before_order;
        capacity=other.capacity;
        open=other.open;
        salary=other.salary;
        copy(other);

    }
    return *this;
}

// destructor
Trainer::~Trainer(){
    clear();
}

// move constructor
Trainer::Trainer(Trainer&& other) noexcept :capacity(other.capacity),open(other.open),customersList(other.getCustomers()),orderList(other.getOrders()),salary(other.getSalary()),before_order(other.get_ordered()) {


    other.capacity= '\0' ;
    other.open='\0';


}

//move assignment
Trainer& Trainer:: operator=(Trainer&& other) noexcept {
    if(this!= &other){
        clear();
        capacity=other.getCapacity();
        open=other.isOpen();
        salary=other.getSalary();
        customersList=other.getCustomers();
        before_order=(other.before_order);

        for(const OrderPair& o:other.getOrders()){
            orderList.push_back(o);
        }

        other.capacity= '\0' ;
        other.open='\0';
        other.salary='\0';
    }
    return *this;
}

void Trainer::copy(const Trainer& other  ){

    for(const OrderPair& o:other.orderList){
        orderList.push_back(o);
    }
    for(Customer* c: other.customersList){
        customersList.push_back(c->clone());
    }

}

void Trainer:: clear(){
    for(Customer* c:customersList){
        delete c;
    }

}

int Trainer::getCapacity() const{return capacity;}

void Trainer::addCustomer(Customer* customer){
    if(customersList.size()<(size_t)capacity)
        customersList.push_back(customer);
}


void Trainer:: removeCustomer(int id){
    bool found= false;
    for (size_t i=0;i<customersList.size()&&!found;i++){
        if((*customersList[i]).getId()==id){
            customersList.erase(customersList.begin()+i);
            found= true;
        }
    }
    delete_orders(id);

}
Customer* Trainer::getCustomer(int id){

    for(Customer* c: customersList){
        if(c->getId()==id)
            return c;
    }

    return nullptr;

}

std::vector<Customer*>& Trainer::getCustomers(){
    return customersList;
}
std::vector<OrderPair>& Trainer::getOrders(){
    return orderList;
}
void Trainer:: order(const int customer_id, const std::vector<int>& workout_ids, const std::vector<Workout>& workout_options){
    for(int id : workout_ids){
        OrderPair p(customer_id,workout_options[id]);
        orderList.push_back(p);
        add_salary(p.second.getPrice());

    }
}
void Trainer::openTrainer(){open= true;}

void Trainer::closeTrainer(){
    open=false;
}

int Trainer::getSalary() const{
    return salary;
}

bool Trainer::isOpen() const{return open;}

void Trainer::add_salary(int addition_salary){salary= salary +addition_salary;}


void Trainer:: delete_orders(int id){
    std::vector<OrderPair > temp ;
    for(const OrderPair& o: orderList){
        if(o.first!= id)
            temp.push_back(o);
    }
    orderList.swap(temp);
}

bool Trainer::get_ordered() const{
    return before_order;
}

void Trainer::set_before_order(bool set){
    before_order=set;
}

