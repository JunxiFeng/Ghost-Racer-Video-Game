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

void StudentWorld::holywaterproj(int x, int y, int direction)
{
    actors.push_back(new HolyWaterProjectiles(x, y, direction, this, theGR));
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
            actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE,RIGHT_EDGE - ROAD_WIDTH/3, j *(4*SPRITE_HEIGHT), this, theGR));
            actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE,LEFT_EDGE + ROAD_WIDTH/3, j *(4*SPRITE_HEIGHT), this, theGR));
        }
    }
    //Yellow BorderLine
    for(int N=0;N<=VIEW_HEIGHT / SPRITE_HEIGHT;N++)
    {
        for(int j=0;j<=N-1;j++)
        {
            actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, LEFT_EDGE,j * SPRITE_HEIGHT, this, theGR));
            actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE,RIGHT_EDGE,j * SPRITE_HEIGHT, this, theGR));
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
        actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE,ROAD_CENTER - ROAD_WIDTH/2,new_border_y, this, theGR));
        actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH/2, new_border_y, this, theGR));
    }
    if(delta_y >= 4*SPRITE_HEIGHT)
    {
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH/2+ROAD_WIDTH/3, new_border_y, this, theGR));
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH/2-ROAD_WIDTH/3, new_border_y, this, theGR));
        lastWhiteY=248;
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
    
    theGR->doSomething();
    if(!theGR->isAlive())
    {
      //  decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    AlldoSomething();
    RemoveDead();
    addnewBorderline();
    
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
