#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"



class Studio{		
public:
	Studio();
    Studio(const std::string &configFilePath);
    // copy constructor
    Studio(const Studio& other);
    // copy assignment
    Studio& operator=(const Studio& other);
    // destructor
    virtual ~Studio();
    // move constructor
    Studio(Studio&& other)noexcept;
    //move assignment
    Studio& operator=(Studio&& other) noexcept;


    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    static WorkoutType FindType (const char* _type);
    void set_open(bool status);
    bool isOpen() const;

private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    //copy
    void copy(const Studio& other);
    //clear
    void clear();
};

#endif