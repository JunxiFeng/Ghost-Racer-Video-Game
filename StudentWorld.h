#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include "Actor.h"
#include <list>
using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void setupBorderline();
    
    //Adding new actors
    void addnewBorderline();
    void addnewOilSlicks();
    void addHolyWaterGoodies();
    void addHealingGoodies(Actor* theActor);
    void addLostSouls();
    void addZombieCab();
    void addHumanPed();
    void addZombiePed();
    
    void holywaterproj(int x, int y, int direction);
    void checkOverlapofHoly(Actor* theActor);
    void AlldoSomething();
    void RemoveDead();
    int theGRverticalSpeed();
    void levelEnd();
    GhostRacer* theGRptr() ;
    void setStats();
    int getSouls();
    void setSouls();
    
    //Movement plan for the zombie cab & related helper functions
    void MovementPlan();
    bool closestFront(Actor* cab);
    bool closestBack(Actor* cab);
    bool closestTOP(int value);
    bool closestBOT(int value);
    bool isLeftLane(Actor* actor);
    bool isMiddleLane(Actor* actor);
    bool isRightLane(Actor* actor);
    bool choice(int value, Actor* actor);
    bool checkSameLane(Actor* A, Actor* B);
private:
    list<Actor*> actors;
    GhostRacer* theGR;
    double lastWhiteY;
    int souls;
    int bonus;
};

#endif // STUDENTWORLD_H_

