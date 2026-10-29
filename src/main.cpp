#include "raylib.h"
#include "Agent.h"
#include "Game.h"
#include "GameConfig.h"
#include <iostream>
#include <fstream>
#include <random>

int main(){

    InitWindow(1000,1000,"DodgeGame");
    SetTargetFPS(60);

    bool simulationMode = true;
    bool agentMode = true;

    Game game;

    //read the best weight from file
    std::ifstream readFile("best_agent.txt");
    float weight;
    readFile >> weight;

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
        float centrWeight = 0.25f;
        float bestWeight = centrWeight;

        //Tetsting zero weight
        float avarageTime = EvaluateAgent(0.0f, attemps);
        float bestTime = avarageTime;

        //random devation
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

        // writes the best weight to a file

        std::ofstream writeFile("best_agent.txt");
        writeFile << bestWeight << '\n';

    }


    else
    {

        while(!WindowShouldClose()){

            if(!game.IsGameOver())
            {   
                float dt = GetFrameTime();
                Vector2 playerDirection = {0.0f , 0.0f};

                if (agentMode)
                {   
                    playerDirection = ChooseDirection(game, weight);
                }
                else
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