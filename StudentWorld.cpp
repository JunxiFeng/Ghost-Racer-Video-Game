#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
using namespace std;


GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::theGRverticalSpeed()
{
    return theGR->getVertSpeed();
}

GhostRacer* StudentWorld::theGRptr()
{
    return theGR;
}

void StudentWorld::holywaterproj(int x, int y, int direction)
{
    actors.push_back(new HolyWaterProjectiles(x, y, direction, this));
}

void StudentWorld::setupBorderline()
{
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    //White BorderLine
    for(int M=0;M<=VIEW_HEIGHT/(4*SPRITE_HEIGHT);M++)
    {
        for(int j=0;j<=M-1;j++)
        {
            actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE,RIGHT_EDGE - ROAD_WIDTH/3, j *(4*SPRITE_HEIGHT), this));
            actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE,LEFT_EDGE + ROAD_WIDTH/3, j *(4*SPRITE_HEIGHT), this));
        }
    }
    //Yellow BorderLine
    for(int N=0;N<=VIEW_HEIGHT / SPRITE_HEIGHT;N++)
    {
        for(int j=0;j<=N-1;j++)
        {
            actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, LEFT_EDGE,j * SPRITE_HEIGHT, this));
            actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE,RIGHT_EDGE,j * SPRITE_HEIGHT, this));
        }
    }
}

void StudentWorld::addnewBorderline()
{
    lastWhiteY = lastWhiteY -4- theGR->getVertSpeed();
    
    int new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    int delta_y=new_border_y-lastWhiteY;
    if(delta_y>=SPRITE_HEIGHT)
    {
        actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE,ROAD_CENTER - ROAD_WIDTH/2,new_border_y, this));
        actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH/2, new_border_y, this));
    }
    if(delta_y >= 4*SPRITE_HEIGHT)
    {
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH/2+ROAD_WIDTH/3, new_border_y, this));
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH/2-ROAD_WIDTH/3, new_border_y, this));
        lastWhiteY=248;
    }
}

void StudentWorld::addnewOilSlicks()
{
    int ChanceOilSlick = max(150-getLevel() * 10, 40);
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    if(randInt(0, ChanceOilSlick-1)==0)
    {
        actors.push_back(new OilSlicks(randInt(LEFT_EDGE, RIGHT_EDGE),VIEW_HEIGHT,this));
    }
}

void StudentWorld::addHolyWaterGoodies()
{
    int ChanceOfHolyWater = 100+10*getLevel();
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    if(randInt(0, ChanceOfHolyWater-1)==0)
    {
        actors.push_back(new HolyWaterBottle_Goodies(randInt(LEFT_EDGE, RIGHT_EDGE),VIEW_HEIGHT,this));
    }
}

void StudentWorld::addHealingGoodies(Actor* theActor)
{
    int ChanceOfHealingGoodies = randInt(1, 5);
    if(ChanceOfHealingGoodies==1)
    {
        actors.push_back(new Healing_Goodies(theActor->getX(),theActor->getY(),this));
    }
}

void StudentWorld::addLostSouls()
{
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    int ChanceOfLostSoul = 100;
    if(randInt(0, ChanceOfLostSoul-1)==0)
    {
        actors.push_back(new LostSouls(randInt(LEFT_EDGE, RIGHT_EDGE),VIEW_HEIGHT,this));
    }
}

void StudentWorld::addZombieCab()
{
    int ChanceVehicle=max(100-getLevel()*10,20);
    if(randInt(0, ChanceVehicle-1)==0)
        MovementPlan();
}

void StudentWorld::addHumanPed()
{
    int ChanceHumanPed=max(200-getLevel()*10,30);
    if(randInt(0, ChanceHumanPed-1)==0)
    {
        actors.push_back(new Human_Pedestrians(randInt(0, VIEW_WIDTH),VIEW_HEIGHT,this));
    }
}

void StudentWorld::addZombiePed()
{
    int ChanceZombiePed = max(100-getLevel() * 10, 20);
    if(randInt(0,ChanceZombiePed-1)==0)
    {
        actors.push_back(new Zombie_Pedestrians(randInt(0, VIEW_WIDTH),VIEW_HEIGHT,this));
    }
}

void StudentWorld::AlldoSomething()
{
    list<Actor*>::iterator it=actors.begin();
    //Loop through and have all actors that are alive do something
    while(it!=actors.end())
    {
        if((*it)->isAlive())
        {
            (*it)->doSomething();
            it++;
        }
        else
            it++;
    }
}

void StudentWorld::RemoveDead()
{
    list<Actor*>::iterator it2 = actors.begin();
    //Loop through and remove the dead
      while(it2!=actors.end())
      {
          if (!(*it2)->isAlive())
          {
              delete *it2;
              auto toErase = it2;
              it2++;
              actors.erase(toErase);
          }
          else
              it2++;
      }
}

void StudentWorld::checkOverlapofHoly(Actor* Proj)
{
    list<Actor*>:: iterator it=actors.begin();
    while(it!=actors.end())
    {
        if((*it)->isAffectedByWater() && Proj->checkOverlap(Proj, (*it)))
        {
            Proj->setAlive(false);
            (*it)->updateStatus(-1); //Minus 1 health
            //Hit Zombie Pedestrian
            if((*it)->isZombiePed())
            {
                if((*it)->gethp()<=0)//If the zombie dies
                {
                    (*it)->setAlive(false);
                    playSound(SOUND_PED_DIE);
                    if(!Proj->checkOverlap((*it), theGR))//Possibility to add healing goodies
                        addHealingGoodies((*it));
                    increaseScore(150);
                }
                else
                    playSound(SOUND_PED_HURT);
            }
            //Hit Human Pedestrian
            if((*it)->isHumanPed())
            {
                (*it)->reverSpeed();//Multiply by -1
                if((*it)->getDirection()==0) //Change from 0 to 180 etc
                    (*it)->setDirection(180);
                if((*it)->getDirection()==180)
                    (*it)->setDirection(0);
                playSound(SOUND_PED_HURT);
            }
            //Hit Zombie Cab
            if((*it)->isZombieCab())
            {
                if((*it)->gethp()<=0)
                {
                    int ChanceOilSlick = max(150-getLevel() * 10, 40);
                    (*it)->setAlive(false);
                    playSound(SOUND_PED_DIE);
                    if(randInt(0, ChanceOilSlick-1)==0)
                        actors.push_back(new OilSlicks((*it)->getX(),(*it)->getY(),this));
                    increaseScore(200);
                }
                else
                    playSound(SOUND_VEHICLE_HURT);
            }
            return;
        }
            it++;
    }
}



void StudentWorld::levelEnd()
{
    theGR->setAlive(false);
} 

int StudentWorld::init()
{
    lastWhiteY=224;
    theGR= new GhostRacer(128,32,this);
    souls=2*getLevel()+5; //Souls to save is based on the current level
    bonus=5000; //Each level starts with 5000 points
    setupBorderline();
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    if(!theGR->isAlive()) //If player is dead
    {
        decLives();
        playSound(SOUND_PLAYER_DIE);
        return GWSTATUS_PLAYER_DIED;
    }
    else if(getSouls()<=0) //All souls are saved
    {
        increaseScore(bonus);
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    theGR->doSomething();
    AlldoSomething();
    RemoveDead();
    addnewBorderline();
    addnewOilSlicks();
    addHolyWaterGoodies();
    addLostSouls();
    addZombieCab();
    addHumanPed();
    addZombiePed();
    setStats();
  
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete theGR;
    list<Actor*>::iterator it = actors.begin();
    //Remove everything
    while(it!=actors.end())
    {
            delete *it;
            auto toErase = it;
            it++;
            actors.erase(toErase);
    }
}

void StudentWorld::setStats()
{
    if(bonus>0)
        bonus=bonus-1;
    ostringstream oss;
    oss<< setw(7)<< "Score: " << getScore()<< "  Lvl: " << getLevel()<< "  Souls2Save: "<<getSouls() << "  Lives: "<< getLives()<< "  Health: " << theGR->gethp()<< "  Sprays: " <<theGR->getHolyWater() << "  Bonus: "<<bonus;
    
    string s=oss.str();
    setGameStatText(s);
}

int StudentWorld::getSouls()
{
    return souls;
}

void StudentWorld::setSouls()
{
    souls=souls-1;
}

void StudentWorld::MovementPlan() //Need Double Check
{
    int cur_lane=randInt(1, 3);//Randomly evalute one lane
    int startY , startSpeed;
    int helper=0;

    //LEFT LANE
    if(cur_lane==1)
    {
        if(closestBOT(1))
        {
            startY=SPRITE_HEIGHT/2;
            startSpeed=theGR->getVertSpeed()+randInt(2, 4);
            actors.push_back(new ZombieCab(ROAD_CENTER-ROAD_WIDTH/3,startY,this, startSpeed));
            helper=1;
        }
        if(closestTOP(1) && helper!=1)
        {
            startY=VIEW_HEIGHT-SPRITE_HEIGHT/2;
            startSpeed=theGR->getVertSpeed()-randInt(2, 4);
            actors.push_back(new ZombieCab(ROAD_CENTER-ROAD_WIDTH/3,startY,this, startSpeed));
            helper=1;
        }
    }

    //MIDDLE LANE
    if(cur_lane==2)
    {
            if(closestBOT(2))
            {
                startY=SPRITE_HEIGHT/2;
                startSpeed=theGR->getVertSpeed()+randInt(2, 4);
                actors.push_back(new ZombieCab(ROAD_CENTER,startY,this, startSpeed));
                helper=2;
             }
            if(closestTOP(2) && helper!=2)
            {
                startY=VIEW_HEIGHT-SPRITE_HEIGHT/2;
                startSpeed=theGR->getVertSpeed()-randInt(2, 4);
                actors.push_back(new ZombieCab(ROAD_CENTER,startY,this, startSpeed));
                helper=2;
             }
    }
    //RIGHT LANE
    if(cur_lane==3)
    {
           if(closestBOT(3))
            {
                startY=SPRITE_HEIGHT/2;
                startSpeed=theGR->getVertSpeed()+randInt(2, 4);
                actors.push_back(new ZombieCab(ROAD_CENTER+ROAD_WIDTH/3,startY,this, startSpeed));
                helper=3;
             }
            if(closestTOP(3) && helper!=3)
            {
                startY=VIEW_HEIGHT-SPRITE_HEIGHT/2;
                startSpeed=theGR->getVertSpeed()-randInt(2, 4);
                actors.push_back(new ZombieCab(ROAD_CENTER+ROAD_WIDTH/3,startY,this, startSpeed));
                helper=3;
             }
    }
    
        if(helper==0) //No viable lane identified as the "chosen lane"
            return;
}


bool StudentWorld::closestBOT(int value)
{
    list<Actor*>::iterator Iterator = actors.begin();
    Actor* closestActor=nullptr;
    int min=260;
    if(choice(value, theGR)) //If the GR is in this lane, can't generate cab from the bottom
        return false;
    //Loop through to find the closest
        while(Iterator!=actors.end())
        {
            if((choice(value, (*Iterator))==true && (*Iterator)->isCollisionAvoidance()) && (*Iterator)->getY()<min && (*Iterator)->isAlive())
            {
                closestActor=(*Iterator);
                min=(*Iterator)->getY();
            }
            Iterator++;
         }
        if(closestActor==nullptr) //No such an actor
            return true;
        if(closestActor!=nullptr && closestActor->getY()>(VIEW_HEIGHT/3))
                return true;
    return false;
}

bool StudentWorld::closestTOP(int value)
{
    list<Actor*>::iterator it2 = actors.begin();
    int min2=-1;
    Actor* closestActor2=nullptr;
    //Loop through to find the closest
    while(it2!=actors.end())
    {
        if((*it2)->isCollisionAvoidance() && (*it2)->getY()>min2 && choice(value, (*it2))==true && (*it2)->isAlive())
        {
            closestActor2=(*it2);
            min2=(*it2)->getY();
        }
        it2++;
    }
    if(closestActor2==nullptr) //No such an actor
        return true;
    if(closestActor2->getY()<(VIEW_HEIGHT*2/3) && closestActor2!=nullptr)
        return true;
    return false;
    
}


bool StudentWorld::closestFront(Actor* cab)
{
    list<Actor*>:: iterator it=actors.begin();
    int y=260;
    Actor* closestactor=nullptr;
    while(it!=actors.end())
    {
                if(checkSameLane((*it),cab) && (*it)->getY()>cab->getY() && (*it)!=(cab) && (*it)->isCollisionAvoidance())
                {
                    if((*it)->getY()-cab->getY()<y)
                    {
                        closestactor=(*it);
                        y=(*it)->getY()-cab->getY();
                    }
                }
        
        it++;
    }
    if(closestactor==nullptr)
        return false;
    if(closestactor->getY()-cab->getY()<96)
        return true;
    return false;
}

bool StudentWorld::closestBack(Actor *cab)
{
    list<Actor*>:: iterator it=actors.begin();
    int y=260;
    Actor* closestactor=nullptr;
    while(it!=actors.end())
    {
                if(checkSameLane((*it),cab) && cab->getY()>(*it)->getY() && (*it)!=(cab) && (*it)->isCollisionAvoidance())
                {
                    if(cab->getY()-(*it)->getY()<y)
                    {
                        closestactor=(*it);
                        y=cab->getY()-(*it)->getY();
                    }
                }
        
        it++;
    }
    if(closestactor==nullptr)
        return false;
    if(cab->getY()-closestactor->getY()<96)
        return true;
    return false;
}

bool StudentWorld::checkSameLane(Actor *A, Actor *B)
{
    //Used for the cab and the other actor only
    //LEFT LANE
    if(isLeftLane(A) && B->getX()==ROAD_CENTER-ROAD_WIDTH/3)
        return true;
    //MIDDLE LANE
    if(isMiddleLane(A) && B->getX()==ROAD_CENTER)
        return true;
    //RIGHT LANE
    if(isRightLane(A) && B->getX()==ROAD_CENTER+ROAD_WIDTH/3)
        return true;
    return false;
}

bool StudentWorld::isLeftLane(Actor* actor)
{
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    if(actor->getX()<LEFT_EDGE+ROAD_WIDTH/3 && actor->getX()>=LEFT_EDGE)
        return true;
    return false;
}

bool StudentWorld::isMiddleLane(Actor *actor)
{
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    if(actor->getX()>=LEFT_EDGE+ROAD_WIDTH/3 && actor->getX()<RIGHT_EDGE-ROAD_WIDTH/3)
        return true;
    return false;
}

bool StudentWorld::isRightLane(Actor *actor)
{
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    if(actor->getX()>=RIGHT_EDGE-ROAD_WIDTH/3 && actor->getX()<RIGHT_EDGE)
        return true;
    return false;
}


bool StudentWorld::choice(int value, Actor* actor)
{
    //Make th choice of lane for the zombie cabs for be generated
    if(value==1 && isLeftLane(actor))
        return true;
    else if(value==2 && isMiddleLane(actor))
        return true;
    else if(value==3 && isRightLane(actor))
        return true;
    return false;
}
