#pragma once

#include "raylib.h"
#include "Game.h"

//auxiliary functions
Vector2 ObjectCentr(Rectangle object);
float Dist(Vector2 object1 , Vector2 object2);

//for agents
Vector2 ChooseDirection(const Game& game, float centerWeight);
float RunEpisode(float centrWeight , unsigned int seed);

float EvaluateAgent(float centrWeight, int attemps);
float TestEvaluateAgent(float centrWeight, int attemps);


