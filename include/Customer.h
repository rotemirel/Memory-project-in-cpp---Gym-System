#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Workout.h"


class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Workout> &workout_options)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;

    virtual Customer* clone()=0;
    virtual const std::string& getType()=0; // our addition
    // copy constructor
    Customer(const Customer& other);// our addition
    virtual ~Customer()=default;
private:
    const std::string name;
    const int id;

};


class SweatyCustomer : public Customer {
public:
    SweatyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    const std::string&  getType();// our addition

    Customer* clone();
private:
    std::string type; // our addition
};


class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    const std::string&  getType();// our addition

    Customer* clone();
private:
    std::string type; // our addition
};


class HeavyMuscleCustomer : public Customer {
public:
	HeavyMuscleCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    const std::string&  getType();
    Customer* clone();
private:
    std::string type; // our addition
};


class FullBodyCustomer : public Customer {
public:
	FullBodyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    const std::string&  getType();
    Customer* clone();
private:
    std::string type; // our addition
};


#endif