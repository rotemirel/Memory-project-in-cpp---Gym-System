//
//#ifndef WORKOUT_CPP_
//#define WORKOUT_CPP_

#include <string>
#include "Workout.h"


// Created by spl211 on 07/11/2021.
//

    Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type):id(w_id),name(w_name),price(w_price),type(w_type){}

    int Workout::getId() const {return id ;}

    std::string Workout::getName() const {return name;}

    int Workout:: getPrice() const {return price;}

    WorkoutType Workout:: getType() const {return type;}

    std::string Workout::tostring_type() const{
        if(type==CARDIO)
            return "Cardio";
        else if(type==ANAEROBIC)
            return "Anaerobic";
        else
            return "Mixed";
    }


