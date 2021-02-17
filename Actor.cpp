#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <cmath>
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// ///////Base Class
Actor::Actor(int imageID, double startX, double startY, int direction, double size, int depth,StudentWorld* myworld,int Yspeed, int Xspeed, bool value): GraphObject(imageID, startX, startY, direction, size, depth)
{
    theworld=myworld;
    vert_speed=Yspeed;
    horiz_speed=Xspeed;
    alive=value;
}

StudentWorld* Actor::getWorld()
{
    return theworld;
}

void Actor::damaged(int damage)
{
    healthPoints=healthPoints-damage;
}

bool Actor::isAlive()
{
    return alive;
}

void Actor::doSomething()
{
    if(!isAlive())
        return;
}

void Actor::setAlive(bool value)
{
    alive=value;
}

void Actor::sethealth(int hp)
{
    healthPoints=hp;
}

void Actor::setHolyWater(int water)
{
    unitsofHolyWater=water;
}

int Actor::gethp()
{
    return healthPoints;
}

int Actor::getHolyWater()
{
    return unitsofHolyWater;
}

int Actor::getVertSpeed()
{
    return vert_speed;
}

int Actor::getHorizSpeed()
{
    return horiz_speed;
}

// ///////////Ghost Racer
GhostRacer::GhostRacer(double startX, double startY, StudentWorld* myworld): Actor(IID_GHOST_RACER, startX, startY, 90, 4.0, 0, myworld,0,0,true)
{
    sethealth(100);
    setHolyWater(10);
}

void GhostRacer::move()
{
    int max_shift_per_tick = 4.0;
    int direction=getDirection();
    int radian=direction*M_PI/180;
    int delta_x = cos(radian) * max_shift_per_tick;
    int cur_x=getX();
    int cur_y=getY();
    if(getDirection()<90)
        moveTo(cur_x+delta_x, cur_y);
    else if(getDirection()>90)
        moveTo(cur_x-delta_x, cur_y);
}

void GhostRacer::doSomething()
{
  //  Actor::doSomething();
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    if(gethp()<=0)
    {
        setAlive(false);
        return;
    }
    if(getX()<=LEFT_EDGE || getX()>=RIGHT_EDGE)
    {
        if(getDirection()>90)
            setDirection(82);
        else if(getDirection()<90)
        {
            setDirection(98);
            move();
        }
        
        damaged(10);
        getWorld()->playSound(SOUND_VEHICLE_CRASH);
    }
    int ch;
    if(getWorld()->getKey(ch))
    {
        switch(ch)
        {
            case KEY_PRESS_LEFT:
                if(getDirection()<114)
                {
                    setDirection(getDirection()+8);
                    move();
                }
                break;
            case KEY_PRESS_RIGHT:
                if(getDirection()>66)
                {
                    setDirection(getDirection()-8);
                    move();
                }
                break;
            case KEY_PRESS_UP:
                break;
            case KEY_PRESS_DOWN:
                break;
            case KEY_PRESS_SPACE:
                break;
            case KEY_PRESS_TAB:
                break;
            case KEY_PRESS_ENTER:
                break;
            case KEY_PRESS_ESCAPE:
                break;
        }
    }
    move();
}

// /////////// Border Line
BorderLine::BorderLine(int imageID, double startX, double startY, StudentWorld* myworld, GhostRacer* gh):  Actor(imageID, startX, startY, 0, 2.0, 1, myworld,-4,0, true)
{
    m_gH=gh;
}



void BorderLine::doSomething()
{
    Actor::doSomething();
    int vert_speed=getVertSpeed()-m_gH->getVertSpeed();
    int horiz_speed=getHorizSpeed();
    int new_y=getY()+vert_speed;
    int new_x=getX()+horiz_speed;
    moveTo(new_x, new_y);
    if(getX()<0 || getY()<0 || getX()>VIEW_WIDTH || getY()>VIEW_HEIGHT)
    {
       setAlive(false);
        return;
    }
}
