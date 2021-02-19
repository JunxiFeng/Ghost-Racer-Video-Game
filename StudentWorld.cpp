#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
   // lastWhiteY=224;
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

void StudentWorld::AlldoSomething()
{
    list<Actor*>::iterator it=actors.begin();
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

bool StudentWorld::checkOverlapofHoly(Actor* theActor)
{
    list<Actor*>:: iterator it=actors.begin();
    while(it!=actors.end())
    {
        if((*it)->isProj())
        {
            int delta_x=abs((*it)->getX()- theActor->getX());
            int delta_y=abs((*it)->getY()-theActor->getY());
            int radius_sum=(*it)->getRadius()+theActor->getRadius();
            if(delta_x < radius_sum*0.25 && delta_y < radius_sum*0.6)
            {
                (*it)->setAlive(false);
                return true;
            }
            it++;
        }
        else
            it++;
    }
    return false;
}

int StudentWorld::init()
{
    lastWhiteY=224;
    theGR= new GhostRacer(128,32,this);
    
    setupBorderline();
    
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    if(!theGR->isAlive())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    theGR->doSomething();
    AlldoSomething();
    RemoveDead();
    addnewBorderline();
    addnewOilSlicks();
    addHolyWaterGoodies();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete theGR;
    list<Actor*>::iterator it = actors.begin();
    while(it!=actors.end())
    {
            delete *it;
            auto toErase = it;
            it++;
            actors.erase(toErase);
    }
}
