#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor: public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, int direction, double size, int depth,StudentWorld* myworld,int Yspeed, int Xspeed, bool value);
  //  virtual int move();
    virtual void doSomething();
    void updateStatus(int hp=0,int water=0,int speed=0);
    void checkOffScreen();
    bool isAlive();
    
    //Getters
    int gethp();
    int getHolyWater();
    int getHorizSpeed();
    int getVertSpeed();
    
    //Setters
    void sethealth(int hp);
    void setHolyWater(int water);
    void setAlive(bool value);
    StudentWorld* getWorld();
    
private:
    int healthPoints;
    int unitsofHolyWater;
    
    int vert_speed;
    int horiz_speed;
    
    bool alive;
    
    StudentWorld* theworld;
};

class GhostRacer: public Actor
{
public:
    GhostRacer(double startX, double startY,StudentWorld* myworld);
    virtual void doSomething();
    void move();
    void swerveOff();
private:
    StudentWorld* theworld;
};


class BorderLine: public Actor
{
public:
    BorderLine(int imageID, double startX, double startY, StudentWorld* myworld, GhostRacer* gh);
    virtual void doSomething();
private:
    GhostRacer* m_gH;
};

class HolyWaterProjectiles: public Actor
{
public:
    HolyWaterProjectiles(double x, double y, int direction,StudentWorld* myworld, GhostRacer* gh);
    virtual void doSomething();
private:
    GhostRacer* m_gh;
};


class ZombieCab: public Actor
{
public:
    
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

class OilSlicks
{
    
};





class LostSouls
{
    
};

class Goodies
{
    
};

class Healing_Goodies
{
    
};

class HolyWaterBottle_Goodies
{
    
};

#endif // ACTOR_H_
