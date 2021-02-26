#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <cmath>
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// ///////Base Class
Actor::Actor(int imageID, double startX, double startY, int direction, double size, int depth,StudentWorld* myworld,int Yspeed, int Xspeed, bool value,bool collisionAvoidance,bool isAffected, int health): GraphObject(imageID, startX, startY, direction, size, depth)
{
    theworld=myworld;
    vert_speed=Yspeed;
    horiz_speed=Xspeed;
    alive=value;
    collision=collisionAvoidance;
    water=isAffected;
    healthPoints=health;
}

StudentWorld* Actor::getWorld()
{
    return theworld;
}


void Actor::damageToGH(int damageDone)
{
    updateStatus(-damageDone);
   
}

void Actor::decreasePlanLength()
{
    planLength=planLength-1;
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

void Actor::reverSpeed()
{
    horiz_speed=horiz_speed*(-1); //Reverse the horizontal speed
}

void Actor::checkOffScreen(Actor *theActor)
{
    //If the actor's x position or the y position is less than 0 or larger than the width and height of the screen, they are out
    if(theActor->getX()<0 ||theActor->getY()<0 ||theActor->getX()>VIEW_WIDTH ||theActor->getY()>VIEW_HEIGHT)
    {
       theActor->setAlive(false);
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
    //Follow the algorithm from the spec
    int delta_x=abs(A->getX()-B->getX());
    int delta_y=abs(A->getY()-B->getY());
    int radius_sum=A->getRadius()+B->getRadius();
    if(delta_x < radius_sum*0.25 && delta_y < radius_sum*0.6)
        return true;
    else
        return false;
}



bool Actor::isAlive()
{
    return alive;
}


bool Actor::isAffectedByWater()
{
    return water;
}

bool Actor::isZombieCab()
{
    return zombieCab;
}

bool Actor::isCollisionAvoidance()
{
    return collision;
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


void Actor::setZombieCab(bool value)
{
    zombieCab=value;
}

void Actor::setPlanDistance(int value)
{
    planLength=value;
}

void Actor::setHorizSpeed(int Xspeed)
{
    horiz_speed=Xspeed;
}

void Actor::setVertiSpeed(int Yspeed)
{
    vert_speed=Yspeed;
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

int Actor::getPlanDistance()
{
    return planLength;
}


// ///////////Ghost Racer
GhostRacer::GhostRacer(double startX, double startY, StudentWorld* myworld): Actor(IID_GHOST_RACER, startX, startY, 90, 4.0, 0, myworld,0,0,true,true,false,100)
{
    setHolyWater(10); //It starts with 10 holy water
}


void GhostRacer::move_algorithm()
{
    //Follow the spec
    int max_shift_per_tick = 4.0;
    int direction=getDirection();
    //Convert the degree to radian by multiplying pi and divide by 180
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
                }
                break;
            case KEY_PRESS_RIGHT:
                if(getDirection()>66)
                {
                    setDirection(getDirection()-8);
                }
                break;
            case KEY_PRESS_UP:
                if(getVertSpeed()<5)
                {
                    int y=getVertSpeed();
                    y=y+1;
                    setVertiSpeed(y);
                }
                break;
            case KEY_PRESS_DOWN:
                if(getVertSpeed()>-1)
                {
                    int y=getVertSpeed();
                    y=y-1;
                    setVertiSpeed(y);
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
        }
    }
    move_algorithm();
}

// /////////// Border Line
BorderLine::BorderLine(int imageID, double startX, double startY, StudentWorld* myworld):  Actor(imageID, startX, startY, 0, 2.0, 1, myworld,-4,0, true,false,false,0)
{
}

void BorderLine::doSomething()
{
    if(!isAlive())
        return;
    move(this);
}


// //////////// Holy Water Projectile
HolyWaterProjectiles::HolyWaterProjectiles(double x, double y, int direction,StudentWorld* myworld): Actor(IID_HOLY_WATER_PROJECTILE, x, y, direction,1.0, 1,myworld,0,0,true,false,false,0)
{
}

void HolyWaterProjectiles::doSomething()
{
    GhostRacer* ptr=getWorld()->theGRptr();
    if(!isAlive())
        return;
    getWorld()->checkOverlapofHoly(this);
    moveForward(SPRITE_HEIGHT);
    checkOffScreen(this);
    int distance=sqrt(pow(getX()-ptr->getX(), 2)+pow(getY()-ptr->getY(), 2));//This method is not given in the spec, basically use the math equation for the line between two points
    if(distance>160)
        setAlive(false);
}


// ////////////// Zombie Cab
ZombieCab::ZombieCab(double x, double y, StudentWorld* myworld, int Yspeed): Actor(IID_ZOMBIE_CAB, x, y, 90, 4, 0, myworld, Yspeed, 0, true,true,true,3)
{
    setZombieCab(true);
    setPlanDistance(0);
    doneDamaged(false);
}

void ZombieCab::doneDamaged(bool value)
{
    damage=value;
}

bool ZombieCab::hasDamaged()
{
    return damage;
}

void ZombieCab::doSomething()
{
    GhostRacer* ptr=getWorld()->theGRptr();
    if(!isAlive())
        return;
   if(checkOverlap(this,ptr) && !hasDamaged())//If has done damaged, the following cannot happen again
   {
           getWorld()->playSound(SOUND_VEHICLE_CRASH);
           ptr->updateStatus(-20);
           doneDamaged(true);
           //To the left
           if(getX()<ptr->getX() || getX()==ptr->getX())
           {
               setHorizSpeed(-5);
               setDirection(60-randInt(0, 19));
           }
           //To the right
           else if(getX()>ptr->getX())
           {
               setHorizSpeed(5);
               setDirection(120+randInt(0, 19));
           }
   }
    move(this);
    //Front
    if(getVertSpeed()>ptr->getVertSpeed() && getWorld()->closestFront(this))
    {
        int y=this->getVertSpeed();
        this->setVertiSpeed(y-0.5);
        return;
    }
    //Back
    else if(getVertSpeed()<=ptr->getVertSpeed() && getWorld()->closestBack(this))
    {
        int y=this->getVertSpeed();
        this->setVertiSpeed(y+0.5);
        return;
    }
    
    decreasePlanLength();
    if(getPlanDistance()>0)
        return;
    else
    {
        int y=getVertSpeed();
        y=y+randInt(-2, 2);
        setPlanDistance(randInt(4, 32));
        setVertiSpeed(y);
    }
}


// //////////// Oil Slicks
OilSlicks::OilSlicks(double x, double y, StudentWorld* myworld): Actor(IID_OIL_SLICK, x, y, 0, randInt(2, 5), 1, myworld, -4, 0, true,false,false,0)
{
}

void OilSlicks::doSomething()
{
    auto ptr=getWorld()->theGRptr();
    int RacerDirection=ptr->getDirection();
    move(this);
    if(checkOverlap(this,ptr))
    {
        int chance=randInt(0, 1);//Clockwise or counter by chance
        int clockwise=randInt(5, 20);
        int counterclockwise=randInt(-5, -20);
        getWorld()->playSound(SOUND_OIL_SLICK);
        if(RacerDirection>=60 && RacerDirection<=120)
        {
            if(chance==0)
                ptr->setDirection(RacerDirection+clockwise);
            if(chance==1)
                ptr->setDirection(RacerDirection+counterclockwise);
        }
    }
}

// ////////////// Goodies (Healing and Holywater)
Goodies::Goodies(int imageID, double x, double y,int direction, double size, int depth,int Yspeed, int Xspeed, bool value,StudentWorld* myworld, bool collision): Actor(imageID, x, y, direction, size, depth, myworld, Yspeed, Xspeed, value,collision,true,1)
{
    
}

void Goodies::doSomething()
{
    move(this);
}

void Goodies::choice(int value)
{
    GhostRacer* ptr=getWorld()->theGRptr();
    if(checkOverlap(this,ptr))
    {
        if(value==250) //If increase score by 250, then it's the healing goodie
        {
            if(gethp()>90) //When health is greater than 90, the health cannot exceed 100
                ptr->updateStatus(100-gethp());
            else if(gethp()<90)
                ptr->updateStatus(10);
        }
        if(value==50) //If increase score by 50, it's holy water goodie
            ptr->updateStatus(0,10);
        setAlive(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(value);
    }
    if(gethp()<=0)
        setAlive(false);
}

Healing_Goodies::Healing_Goodies(double x, double y, StudentWorld* myworld): Goodies(IID_HEAL_GOODIE, x, y, 0, 1, 2, -4, 0, true, myworld,false)
{
}

void Healing_Goodies::doSomething()
{
    Goodies::doSomething();
    choice(250);
}

HolyWaterBottle_Goodies::HolyWaterBottle_Goodies(double x, double y, StudentWorld* myworld): Goodies(IID_HOLY_WATER_GOODIE, x, y, 0, 2, 2, -4, 0, true, myworld,false)
{
}

void HolyWaterBottle_Goodies::doSomething()
{
    Goodies::doSomething();
    choice(50);
}

// ////////////// Soul
LostSouls::LostSouls(double x, double y, StudentWorld* myworld): Actor(IID_SOUL_GOODIE, x, y, 0, 4, 2, myworld, -4, 0, true,false,false,0)
{
}

void LostSouls::doSomething()
{
    GhostRacer* ptr=getWorld()->theGRptr();
    move(this);
    if(checkOverlap(this, ptr))
    {
        setAlive(false);
        getWorld()->playSound(SOUND_GOT_SOUL);
        getWorld()->increaseScore(100);
        getWorld()->setSouls(); //Decrease the souls to save by 1
    }
    setDirection(getDirection()-10);
}

// ////////////// Pedestrian
Pedestrian::Pedestrian(int imageID, double startX, double startY, double size,StudentWorld* myworld): Actor(imageID, startX, startY, 0, size, 0, myworld, -4, 0, true, true,true,2)
{
    setPlanDistance(0);
}


int Pedestrian::RandomInt() //Exclude 0
{
    int random=randInt(-3, 3);
    while(random==0)
    {
        random=randInt(-3, 3);
        if(random!=0)
            break;
    }
    return random;
}

Human_Pedestrians::Human_Pedestrians(double x, double y, StudentWorld* myworld): Pedestrian(IID_HUMAN_PED, x, y, 2, myworld)
{
    setHumanPed(true);
}

void Human_Pedestrians::doSomething()
{
    GhostRacer* ptr=getWorld()->theGRptr();
    if(checkOverlap(this, ptr))
    {
        getWorld()->levelEnd(); //When human is hit, the level end and player is considered dead
        return;
    }
    move(this);
    decreasePlanLength();
    if(getPlanDistance()>0)
        return;
    else
    {
        setHorizSpeed(RandomInt());
        setPlanDistance(randInt(4, 32));
        if(getHorizSpeed()<0)
            setDirection(180);
        else if(getHorizSpeed()>0)
            setDirection(0);
    }
}

void Actor::setHumanPed(bool value)
{
    humanped=value;
}

bool Actor::isHumanPed()
{
    return humanped;
}

Zombie_Pedestrians::Zombie_Pedestrians(double x, double y, StudentWorld* myworld): Pedestrian(IID_ZOMBIE_PED, x, y, 3.0, myworld)
{
    sethealth(2);
    setTicks(0);
    setZombiePed(true);
}

int Zombie_Pedestrians::getTicks()
{
    return grunt;
}

void Zombie_Pedestrians::setTicks(int value)
{
    grunt=value;
}

void Actor::setZombiePed(bool value)
{
    zombieped=value;
}

bool Actor::isZombiePed()
{
    return zombieped; 
}

void Zombie_Pedestrians::doSomething()
{
    //Follow the spec
    GhostRacer* ptr=getWorld()->theGRptr();
    if(!this->isAlive())
        return;
    if(checkOverlap(this, ptr))
    {
        ptr->updateStatus(-5);
        getWorld()->playSound(SOUND_PED_DIE);
        setAlive(false);
        return;
    }
    if(abs(this->getX()-ptr->getX())<30 && this->getY()>ptr->getY())
    {
        setDirection(270);
        if(this->getX()<ptr->getX())
            setHorizSpeed(1);
        else if(this->getX()>ptr->getX())
            setHorizSpeed(-1);
        grunt=grunt-1;
        if(getTicks()<=0)
        {
            getWorld()->playSound(SOUND_ZOMBIE_ATTACK);
            setTicks(20);
        }
    }
    move(this);
    if(getPlanDistance()>0)
    {
        decreasePlanLength();
        return;
    }
    else
    {
        setHorizSpeed(RandomInt());
        setPlanDistance(randInt(4, 32));
        if(getHorizSpeed()<0)
            setDirection(180);
        else if(getHorizSpeed()>0)
            setDirection(0);
    }
}
