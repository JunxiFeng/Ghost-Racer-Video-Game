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

void Actor::doneDamaged(bool value)
{
    damage=value;
}

bool Actor::hasDamaged()
{
    return damage;
}

void Actor::damageToGH(int damageDone)
{
    updateStatus(-damageDone);
   
}

void Actor::move(Actor *A)
{
        int vert_speed=A->getVertSpeed()-theworld->theGRverticalSpeed();
        int horiz_speed=A->getHorizSpeed();
        int new_y=A->getY()+vert_speed;
        int new_x=A->getX()+horiz_speed;
        moveTo(new_x, new_y);
    checkOffScreen(A);
}

void Actor::checkOffScreen(Actor *theActor)
{
    if(theActor->getX()<0 ||theActor->getY()<0 ||theActor->getX()>VIEW_WIDTH ||theActor->getY()>VIEW_HEIGHT)
    {
       setAlive(false);
        return;
    }
}

void Actor::updateStatus(int hp, int water, int Yspeed, int Xspeed)
{
    healthPoints=healthPoints+hp;
    unitsofHolyWater=unitsofHolyWater+water;
    vert_speed=vert_speed+Yspeed;
    horiz_speed=horiz_speed+Xspeed;
}

bool Actor::checkOverlap(Actor* A, Actor* B)
{
   // auto ptr=theworld->theGRptr();
    int delta_x=abs(A->getX()-B->getX());
    int delta_y=abs(A->getY()-B->getY());
    int radius_sum=A->getRadius()+B->getRadius();
    if(delta_x < radius_sum*0.25 && delta_y < radius_sum*0.6)
        return true;
    else
        return false;
}

void Actor::checkOverlapofHoly()
{
    if(getWorld()->checkOverlapofHoly(this))
    {
        setAlive(false);
    }
}

bool Actor::isAlive()
{
    return alive;
}

bool Actor::isProj()
{
    return proj;
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

void Actor::setPorj(bool value)
{
    proj=value;
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


void GhostRacer::move_algorithm()
{
    int max_shift_per_tick = 4.0;
    int direction=getDirection();
    double delta_x = cos(direction*M_PI/180) * max_shift_per_tick;
    double cur_x=getX();
    double cur_y=getY();
    moveTo(cur_x+delta_x, cur_y);
}

void GhostRacer::swerveOff()
{
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    if(getX()<=LEFT_EDGE || getX()>=RIGHT_EDGE)
    {
        if(getDirection()>90)
        {
            setDirection(82);
            move_algorithm();
        }
        else if(getDirection()<90)
        {
            setDirection(98);
            move_algorithm();
        }
        updateStatus(-10);
        getWorld()->playSound(SOUND_VEHICLE_CRASH);
    }
}

void GhostRacer::doSomething()
{
  //  Actor::doSomething();
    if(gethp()<=0)
    {
        setAlive(false);
        return;
    }
    swerveOff();
    int ch;
    if(getWorld()->getKey(ch))
    {
        switch(ch)
        {
            case KEY_PRESS_LEFT:
                if(getDirection()<114)
                {
                    setDirection(getDirection()+8);
                  //  move_algorithm();
                }
                break;
            case KEY_PRESS_RIGHT:
                if(getDirection()>66)
                {
                    setDirection(getDirection()-8);
                   // move_algorithm();
                }
                break;
            case KEY_PRESS_UP:
                if(getVertSpeed()<5)
                {
                    updateStatus(0,0,1);
                }
                break;
            case KEY_PRESS_DOWN:
                if(getVertSpeed()>-1)
                {
                    updateStatus(0,0,-1);
                }
                break;
            case KEY_PRESS_SPACE:
                if(getHolyWater()>=1)
                {
                getWorld()->holywaterproj(getX()+cos(getDirection()*M_PI/180),getY()+sin(getDirection()*M_PI/180), getDirection());
                    
                    getWorld()->playSound(SOUND_PLAYER_SPRAY);
                    updateStatus(0,-1);
                }
                break;
            case KEY_PRESS_TAB:
                break;
            case KEY_PRESS_ENTER:
                break;
            case KEY_PRESS_ESCAPE:
                break;
        }
    }
    move_algorithm();
}

// /////////// Border Line
BorderLine::BorderLine(int imageID, double startX, double startY, StudentWorld* myworld):  Actor(imageID, startX, startY, 0, 2.0, 1, myworld,-4,0, true)
{
    
}

void BorderLine::doSomething()
{
    Actor::doSomething();
    move(this);
}


// //////////// Holy Water Projectile
HolyWaterProjectiles::HolyWaterProjectiles(double x, double y, int direction,StudentWorld* myworld): Actor(IID_HOLY_WATER_PROJECTILE, x, y, direction,1.0, 1,myworld,0,0,true)
{
    setPorj(true);
}

void HolyWaterProjectiles::doSomething()
{
    auto ptr=getWorld()->theGRptr();
    Actor::doSomething();
    moveForward(SPRITE_HEIGHT);
    checkOffScreen(this);
    int distance=sqrt(pow(getX()-ptr->getX(), 2)+pow(getY()-ptr->getY(), 2));
    if(distance>160)
        setAlive(false);
}


// ////////////// Zombie Cab
ZombieCab::ZombieCab(double x, double y, StudentWorld* myworld, GhostRacer* gh): Actor(IID_ZOMBIE_CAB, x, y, 90, 4, 0, myworld, 0, 0, true)
{
    sethealth(3);
    m_gh=gh;
}

void ZombieCab::doSomething()
{
    auto ptr=getWorld()->theGRptr();
    if(!isAlive())
        return;
   if(checkOverlap(this,ptr))
   {
       if(hasDamaged())
           move(this);
       else
       {
           getWorld()->playSound(SOUND_VEHICLE_CRASH);
           updateStatus(-20,0,0);
           doneDamaged(true);
           //To the left
           if((getX()+getRadius())<m_gh->getX() || getX()==m_gh->getX())
           {
               updateStatus(0,0,0,-5);//Horiz speed to -5
               setDirection(60-randInt(0, 19));
           }
           //To the right
           else if(getX()+getRadius()>m_gh->getX())
           {
               updateStatus(0,0,0,5);//Horiz speed to 5
               setDirection(120+randInt(0, 19));
           }
       }
   }
    move(this);
    if(this->getVertSpeed()>m_gh->getVertSpeed())
    {
        //Not Done
    }
}


// //////////// Oil Slicks
OilSlicks::OilSlicks(double x, double y, StudentWorld* myworld): Actor(IID_OIL_SLICK, x, y, 0, randInt(2, 5), 1, myworld, -4, 0, true)
{
}

void OilSlicks::doSomething()
{
    auto ptr=getWorld()->theGRptr();
    int RacerDirection=ptr->getDirection();
    move(this);
    if(checkOverlap(this,ptr))
    {
        int clockwise=randInt(5, 20);
        int counterclockwise=randInt(-20, -5);
        int delta_degree=randInt(counterclockwise, clockwise);
        getWorld()->playSound(SOUND_OIL_SLICK);
        if(RacerDirection>60 && RacerDirection<120)
        {
            ptr->setDirection(RacerDirection+delta_degree);
        }
    }
}

// ////////////// Goodies (Healing and Holywater)
Goodies::Goodies(int imageID, double x, double y,int direction, double size, int depth,int Yspeed, int Xspeed, bool value,StudentWorld* myworld): Actor(imageID, x, y, direction, size, depth, myworld, Yspeed, Xspeed, value)
{
    
}

void Goodies::doSomething()
{
    move(this);
    
}

Healing_Goodies::Healing_Goodies(double x, double y, StudentWorld* myworld): Goodies(IID_HEAL_GOODIE, x, y, 0, 1, 2, -4, 0, true, myworld)
{
    
}

void Healing_Goodies::doSomething()
{
    auto ptr=getWorld()->theGRptr();
    Goodies::doSomething();
    if(checkOverlap(this,ptr))
    {
        updateStatus(10);
        setAlive(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(250);
    }
    checkOverlapofHoly();
}

HolyWaterBottle_Goodies::HolyWaterBottle_Goodies(double x, double y, StudentWorld* myworld): Goodies(IID_HOLY_WATER_GOODIE, x, y, 0, 2, 2, -4, 0, true, myworld)
{
}

void HolyWaterBottle_Goodies::doSomething()
{
    auto ptr=getWorld()->theGRptr();
    Goodies::doSomething();
    if(checkOverlap(this, ptr))
    {
        updateStatus(0,10);
        setAlive(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(50);
    }
    checkOverlapofHoly();
}
