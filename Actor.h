#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor: public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, int direction, double size, int depth,StudentWorld* myworld,int Yspeed, int Xspeed, bool value);
  
    //Anything
    virtual void doSomething();
    void updateStatus(int hp=0,int water=0,int Yspeed=0, int Xspeed=0);
    void checkOffScreen(Actor* theActor);
    bool checkOverlap(Actor* A, Actor* B);
    void checkOverlapofHoly();
    void move(Actor* A);
    void doneDamaged(bool value);
    bool hasDamaged();
    void damageToGH(int damageDone);
    bool isAlive();
    bool isProj();
    //Getters
    int gethp();
    int getHolyWater();
    int getHorizSpeed();
    int getVertSpeed();
    
    //Setters
    void sethealth(int hp);
    void setHolyWater(int water);
    void setAlive(bool value);
    void setPorj(bool value);
    StudentWorld* getWorld();
    
private:
    int healthPoints;
    int unitsofHolyWater;
    
    int vert_speed;
    int horiz_speed;
    
    bool alive;
    bool damage;
    bool proj;
    StudentWorld* theworld;
};

class GhostRacer: public Actor
{
public:
    GhostRacer(double startX, double startY,StudentWorld* myworld);
    virtual void doSomething();
    void move_algorithm();
    void swerveOff();
private:
};


class BorderLine: public Actor
{
public:
    BorderLine(int imageID, double startX, double startY, StudentWorld* myworld);
    virtual void doSomething();
private:
};

class HolyWaterProjectiles: public Actor
{
public:
    HolyWaterProjectiles(double x, double y, int direction,StudentWorld* myworld);
    virtual void doSomething();
private:
};


class ZombieCab: public Actor
{
public:
    ZombieCab(double x, double y, StudentWorld* myworld, GhostRacer* gh);
    virtual void doSomething();
private:
    GhostRacer* m_gh;
};

class Pedestrian
{
    
};

class Human_Pedestrians
{
    
};

class Zombie_Pedestrians
{
    
};

class OilSlicks: public Actor
{
public:
    OilSlicks(double x, double y, StudentWorld* myworld);
    virtual void doSomething();
};





class LostSouls
{
    
};

class Goodies: public Actor
{
public:
    Goodies(int imageID, double x, double y,int direction, double size, int depth,int Yspeed, int Xspeed, bool value,StudentWorld* myworld);
    virtual void doSomething();
private:
  //  HolyWaterProjectiles* theProj;
};

class Healing_Goodies: public Goodies
{
public:
    Healing_Goodies(double x, double y, StudentWorld* myworld);
    virtual void doSomething();
};

class HolyWaterBottle_Goodies: public Goodies
{
public:
    HolyWaterBottle_Goodies(double x, double y, StudentWorld* myworld);
    virtual void doSomething();
private:
};

#endif // ACTOR_H_
