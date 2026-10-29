#include "raylib.h"
#include"Agent.h"
#include "Game.h"
#include "GameConfig.h"
#include "raymath.h"
#include <vector>
#include <random>

Vector2 ObjectCentr(Rectangle object)
{
    float centrX = object.x + (object.width)/2;
    float centrY = object.y + (object.height)/2;
    return {centrX , centrY};
}

float Dist(Vector2 object1 , Vector2 object2)
{
    float dist = (object1.x - object2.x)*(object1.x - object2.x) + (object1.y - object2.y)*(object1.y - object2.y);
    return dist;
}

Vector2 ChooseDirection(const Game& game, float centerWeight)
{

    Vector2 agentDirection = {0.0f , 0.0f};
    Vector2 playerCentr = ObjectCentr(game.PlayerBound());

    // Find nearest projectile

    float minDist = -1.0f;
    Rectangle nearestBullet;

    for (const Rectangle& bullet : game.BulletsBound())
    {
        Vector2 bulletCentr = ObjectCentr(bullet);
        float dist = Dist(playerCentr, bulletCentr);

        if (minDist > dist || minDist < 0)
        {   
            minDist = dist;
            nearestBullet = bullet;
        }
    }

    if (minDist != -1.0f)
    {
        Vector2 bulletCentr = ObjectCentr(nearestBullet);

        agentDirection.x = playerCentr.x - bulletCentr.x;
        agentDirection.y = playerCentr.y - bulletCentr.y;

    }

    // Correct direction by weights
    
    Vector2 fieldCentr = {FILE_WIDTH/2 , FILE_HEIGHT/2};
    Vector2 toCentr = {fieldCentr.x - playerCentr.x , fieldCentr.y - playerCentr.y};

    agentDirection = 
    {
        agentDirection.x + toCentr.x * centerWeight,
        agentDirection.y + toCentr.y * centerWeight
    };

    
    return agentDirection;
}

float RunEpisode(float centrWeight , unsigned int seed)
{   
    SetRandomSeed(seed);

    Game game;
    Vector2 agentDirection = {0.0f , 0.0f};

    while (!game.IsGameOver() && game.GetSurvivalTime() < EPISODE_TIME_LIMIT)
    {    
        agentDirection = ChooseDirection(game, centrWeight);
        game.Update(agentDirection, SIMULATION_DT);
    }

    return game.GetSurvivalTime();
}


float EvaluateAgent(float centrWeight, int attemps)
{
    float avarageTime = 0.0f;

    for(int it = 0 ; it < attemps ; it++)
    {   
        unsigned int seed = it;
        float result = RunEpisode(centrWeight, seed);
        avarageTime += result;
    }

    avarageTime /= attemps;
    return avarageTime;
}

float TestEvaluateAgent(float centrWeight, int attemps)
{
    float avarageTime = 0.0f;

    for(int it = 0 ; it < attemps ; it++)
    {   
        unsigned int seed = it+100; // Testing in others seeds
        float result = RunEpisode(centrWeight, seed);
        avarageTime += result;
    }

    avarageTime /= attemps;
    return avarageTime;
}
    