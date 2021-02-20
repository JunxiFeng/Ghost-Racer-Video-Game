#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include "Actor.h"
#include <map>
#include <vector>
#include <list>
using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void setupBorderline();
    void addnewBorderline();
    void addnewOilSlicks();
    void addHolyWaterGoodies();
    void addHealingGoodies(Actor* theActor);
    void addLostSouls();
    void addZombieCab();
    void holywaterproj(int x, int y, int direction);
    bool checkOverlapofHoly(Actor* theActor);
    bool checkZombieCabFront();
    bool checkZombieCabBehind();
    void AlldoSomething();
    void RemoveDead();
    int theGRverticalSpeed();
    bool closestTOP();
    bool closestBOT();
    GhostRacer* theGRptr();
    
    void MovementPlan();
private:
    list<Actor*> actors;
    GhostRacer* theGR;
    double lastWhiteY;
};

#endif // STUDENTWORLD_H_
