#include "raylib.h"
#include "Game.h"
#include "GameConfig.h"
#include <vector>
#include <iostream>
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
        //std::cout << "\033[32mResult: " << result << "\033[0m\n";
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
        unsigned int seed = it+100;
        float result = RunEpisode(centrWeight, seed);
        //std::cout << "\033[32mResult: " << result << "\033[0m\n";
        avarageTime += result;
    }

    avarageTime /= attemps;
    return avarageTime;
}


int main(){

    InitWindow(1000,1000,"DodgeGame");
    SetTargetFPS(60);

    bool simulationMode = true;
    bool agentMode = true;

    Game game;

    //Text for the end of the game

    const char *final_text = "GAME OVER";
    
    int centrX = GetScreenWidth() / 2;
    int centrY = GetScreenHeight() / 2;

    int size_final_text = 90;
    int width_final_text = MeasureText(final_text , size_final_text);

    int posX = centrX - width_final_text/2;
    int posY = centrY - size_final_text/2;


    int Record = 0;

    if (simulationMode)
    {   
        int attemps = 100;

        float centrWeight = 0.0f;

        float avarageTime = EvaluateAgent(centrWeight , attemps);
        float bestTime = avarageTime;

        centrWeight = 0.25;
        float bestWeight = centrWeight;


        std::mt19937 mutationGenerator(42);
        std::uniform_real_distribution<float> mutationDistribution(-0.1f, 0.1f);


        for (int it = 0 ; it < 20 ; it++)
        {   
            float mutation = mutationDistribution(mutationGenerator);
            centrWeight = bestWeight + mutation;
            avarageTime = EvaluateAgent(centrWeight , attemps);
         
            if (avarageTime > bestTime)
            {
                bestTime = avarageTime;
                bestWeight = centrWeight; 
            }

        }

        // compare the average difference between generations

        int testAttemps = 50;
            
        float gen0Weight = 0.0f;
        float gen1Weight = bestWeight;

        float gen0Time = TestEvaluateAgent(gen0Weight , testAttemps);
        float gen1Time = TestEvaluateAgent(gen1Weight , testAttemps);

        std::cout << "\n\033[36mAvavrage generation0 time: " << gen0Time << "\033[0m\n";
        std::cout << "\033[32mWeight: " << gen0Weight << "\033[0m\n";
        
        std::cout << "\n\033[36mAvavrage generation1 time: " << gen1Time << "\033[0m\n";
        std::cout << "\033[32mWeight: " << gen1Weight << "\033[0m\n";




    }


    else
    {

        while(!WindowShouldClose()){

            if(!game.IsGameOver())
            {   
                float dt = GetFrameTime();
                Vector2 playerDirection = {0.0f , 0.0f};

                if (!agentMode)
                {
                    if(IsKeyDown(KEY_S))
                    {
                        playerDirection.y += 1.0f;
                    }
                    if(IsKeyDown(KEY_W))
                    {
                        playerDirection.y -= 1.0f;
                    }

                    if(IsKeyDown(KEY_D))
                    {
                        playerDirection.x += 1.0f;
                    }
                    if(IsKeyDown(KEY_A))
                    {
                        playerDirection.x -= 1.0f;
                    }
                }

                else
                {   
                    float centrWeight = 0.14f;
                    playerDirection = ChooseDirection(game, centrWeight);
                }

                game.Update(playerDirection,dt);        

            }


            if(IsKeyPressed(KEY_R) && game.IsGameOver())
            {   
                game.Reset();          
            }


            BeginDrawing();

            ClearBackground(RAYWHITE);
            game.Draw();

            if(game.IsGameOver())
            {
                DrawText(final_text,posX,posY,size_final_text,RED);


                // Record Text

                Record = (Record > game.GetSurvivalTime()) ? Record : game.GetSurvivalTime();
                const char *record_txt = TextFormat("Record: %i" , static_cast<int>(Record));

                int size_record_txt = 50;
                int width_record_txt = MeasureText(record_txt,size_record_txt);

                int posXrecord = centrX - width_record_txt / 2;
                int posYrecord = posY + 100;

                DrawText(record_txt,posXrecord,posYrecord,size_record_txt,GREEN);
            }


            //Time text

            const char *time_txt = TextFormat("Time: %i" , static_cast<int>(game.GetSurvivalTime()));

            int size_time_txt = 30;
            int width_time_txt = MeasureText(time_txt,size_time_txt);

            int posXtime = centrX - width_time_txt / 2;
            int posYtime = 15;

            DrawText(time_txt, posXtime, posYtime, size_time_txt, BLACK);
            

            EndDrawing();

        }

        CloseWindow();
    }

    return 0;

}