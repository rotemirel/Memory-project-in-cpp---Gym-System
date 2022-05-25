//
// Created by spl211 on 07/11/2021.
//
#include "Customer.h"
#include "Workout.h"
#include <vector>
#include <algorithm>


Customer::Customer(std::string c_name, int c_id):name(c_name),id(c_id){}
// copy constructor
Customer::Customer(const Customer& other):name(other.getName()),id(other.getId()){}

std::string Customer:: getName() const{return name;}

int Customer::getId() const{return id;}






SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name, id) ,type("swt"){}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> myOrders;
    for(const auto & workout_option : workout_options){
        if(workout_option.getType()==CARDIO){
            myOrders.push_back(workout_option.getId());
        }
    }
    return myOrders;
}

std::string SweatyCustomer:: toString() const{
    return std::to_string(getId())+" "+getName();
}

const std::string& SweatyCustomer:: getType(){return type;}

Customer* SweatyCustomer::clone(){
    return new SweatyCustomer(*this);
}


CheapCustomer::CheapCustomer(std::string name, int id):Customer(name, id) , type("chp"){}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> myOrders;
    int currMinPrice=workout_options[0].getPrice();
    int currMinId=0;

    for(size_t i=1;i<workout_options.size();i++){
        if(workout_options[i].getPrice()<currMinPrice){
            currMinPrice=workout_options[i].getPrice();
            currMinId=workout_options[i].getId();
        }
    }
    myOrders.push_back(currMinId);
    return myOrders;
}
std::string CheapCustomer:: toString() const{return std::to_string(getId())+" "+getName();}

const std::string& CheapCustomer:: getType(){return type;}

Customer* CheapCustomer::clone(){
    return new CheapCustomer(*this);
}




HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id):Customer(name, id),type("mcl"){}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> myOrders;
    for(const auto & workout_option : workout_options){
        if(workout_option.getType()==ANAEROBIC){
            myOrders.push_back(workout_option.getId());
        }
    }

    sort(myOrders.begin(),myOrders.end(),[&workout_options](int id1,int id2){
        return workout_options[id1].getPrice() > workout_options[id2].getPrice();
    });

    return myOrders;
}
std::string HeavyMuscleCustomer::toString() const{ return std::to_string(getId())+" "+getName();}

const std::string& HeavyMuscleCustomer:: getType(){return type;}

Customer* HeavyMuscleCustomer::clone(){
    return new HeavyMuscleCustomer(*this);
}




FullBodyCustomer::FullBodyCustomer(std::string name, int id):Customer(name, id),type("fbd"){}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> myOrders;
    int currMinPriceCardio=INT16_MAX;
    int currMinIdCardio=0;
    int currMinPriceAnaerobic=INT16_MAX;
    int currMinIdAnaerobic=0;
    int currMaxPriceMixed=-1;
    int currMaxIdMixed=0;

    for(const auto & workout_option : workout_options){

        if(workout_option.getType()==ANAEROBIC && workout_option.getPrice()<currMinPriceAnaerobic){
            currMinPriceAnaerobic=workout_option.getPrice();
            currMinIdAnaerobic=workout_option.getId();
        }
        else if(workout_option.getType()==CARDIO && workout_option.getPrice()<currMinPriceCardio){
            currMinPriceCardio=workout_option.getPrice();
            currMinIdCardio=workout_option.getId();
        }
        else if(workout_option.getType()==MIXED && workout_option.getPrice()>currMaxPriceMixed){
            currMaxPriceMixed=workout_option.getPrice();
            currMaxIdMixed=workout_option.getId();
        }
    }
    myOrders.push_back(currMinIdCardio);
    myOrders.push_back(currMaxIdMixed);
    myOrders.push_back(currMinIdAnaerobic);

    return myOrders;
}
std::string FullBodyCustomer::toString() const{ return std::to_string(getId())+" "+getName();}

const std::string& FullBodyCustomer:: getType(){return type;}

Customer* FullBodyCustomer::clone(){
    return new FullBodyCustomer(*this);
}