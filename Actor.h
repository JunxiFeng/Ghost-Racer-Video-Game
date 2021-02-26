#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor: public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, int direction, double size, int depth,StudentWorld* myworld,int Yspeed, int Xspeed, bool value, bool collisionAvoidance,bool isAffected, int health);
  
    //Anything
    virtual void doSomething()=0;
    void updateStatus(int hp=0,int water=0,int Yspeed=0, int Xspeed=0);
    void checkOffScreen(Actor* theActor);
    bool checkOverlap(Actor* A, Actor* B);
    void move(Actor* A);
    
    void damageToGH(int damageDone);
    void decreasePlanLength();
    void reverSpeed();
    bool isAlive();
    bool isAffectedByWater();
    bool isZombieCab();
    bool isZombiePed();
    bool isHumanPed();
    bool isCollisionAvoidance();
    //Getters
    int gethp();
    int getHolyWater();
    int getHorizSpeed();
    int getVertSpeed();
    int getPlanDistance();
    
    //Setters
    void sethealth(int hp);
    void setHolyWater(int water);
    void setAlive(bool value);
    void setZombieCab(bool value);
    void setZombiePed(bool value);
    void setHumanPed(bool value);
    void setPlanDistance(int value);
    void setHorizSpeed(int Xspeed);
    void setVertiSpeed(int Yspeed);
    StudentWorld* getWorld();
    
    virtual ~Actor(){};
private:
    int healthPoints;
    int unitsofHolyWater;
    
    int vert_speed;
    int horiz_speed;
    
    int planLength;
    
    bool alive;
    bool water;
    bool zombieCab;
    bool zombieped;
    bool humanped;
    bool collision;
    StudentWorld* theworld;
};

class GhostRacer: public Actor
{
public:
    GhostRacer(double startX, double startY,StudentWorld* myworld);
    virtual void doSomething();
    void move_algorithm();
    void swerveOff();
    virtual ~GhostRacer(){};
private:
};


class BorderLine: public Actor
{
public:
    BorderLine(int imageID, double startX, double startY, StudentWorld* myworld);
    virtual void doSomething();
    virtual ~BorderLine(){};
private:
};

class HolyWaterProjectiles: public Actor
{
public:
    HolyWaterProjectiles(double x, double y, int direction,StudentWorld* myworld);
    virtual void doSomething();
    virtual ~HolyWaterProjectiles(){};
private:
};


class ZombieCab: public Actor
{
public:
    ZombieCab(double x, double y, StudentWorld* myworld, int Yspeed);
    virtual void doSomething();
    void doneDamaged(bool value);
    bool hasDamaged();
    virtual ~ZombieCab(){};
private:
    bool damage;
};

class Pedestrian: public Actor
{
public:
    Pedestrian(int imageID, double startX, double startY, double size,StudentWorld* myworld);
    virtual void doSomething()=0;
    int RandomInt();
    virtual ~Pedestrian(){};
};

class Human_Pedestrians: public Pedestrian
{
public:
    Human_Pedestrians(double x, double y, StudentWorld* myworld);
    virtual void doSomething();
    virtual ~Human_Pedestrians(){};
};

class Zombie_Pedestrians: public Pedestrian
{
public:
    Zombie_Pedestrians(double x, double y, StudentWorld* myworld);
    virtual void doSomething();
    int getTicks();
    void setTicks(int value);
    virtual ~Zombie_Pedestrians(){};
private:
    int grunt;
    
};

class OilSlicks: public Actor
{
public:
    OilSlicks(double x, double y, StudentWorld* myworld);
    virtual void doSomething();
    virtual ~OilSlicks(){};
};

class LostSouls: public Actor
{
public:
    LostSouls(double x, double y, StudentWorld* myworld);
    virtual void doSomething();
    virtual ~LostSouls(){};
};

class Goodies: public Actor
{
public:
    Goodies(int imageID, double x, double y,int direction, double size, int depth,int Yspeed, int Xspeed, bool value,StudentWorld* myworld, bool collision);
    virtual void doSomething();
    void choice(int value);
    virtual ~Goodies(){};
private:
};

class Healing_Goodies: public Goodies
{
public:
    Healing_Goodies(double x, double y, StudentWorld* myworld);
    virtual void doSomething();
    virtual ~Healing_Goodies(){};
};

class HolyWaterBottle_Goodies: public Goodies
{
public:
    HolyWaterBottle_Goodies(double x, double y, StudentWorld* myworld);
    virtual void doSomething();
    virtual ~HolyWaterBottle_Goodies(){};
private:
};

#endif // ACTOR_H_
