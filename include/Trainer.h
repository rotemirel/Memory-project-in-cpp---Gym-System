#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    Trainer(int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int>& workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary() const;
    bool isOpen() const;
    void add_salary(int addition_salary);

    // copy constructor
    Trainer(const Trainer& other);
    // copy assignment
    Trainer& operator=(const Trainer& other);
    // destructor
    virtual ~Trainer();
    // move constructor
    Trainer(Trainer&& other) noexcept;
    //move assignment
    Trainer& operator=(Trainer&& other) noexcept;

    bool get_ordered() const;
    void delete_orders(int id);
    void set_before_order(bool set);


    //void copy_customers_list(const std::vector<Customer*>& _customersList,int capacity); delete after checking clone

private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
    int salary;

    void copy(const Trainer& other  ); // reference on vector of pointers?
    void clear();
    bool before_order;
};


#endif