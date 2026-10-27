#include "raylib.h"
#include "Game.h"
#include "GameConfig.h"
#include <vector>
#include <iostream>

Vector2 ObjectCentr(Rectangle object){
    float centrX = object.x + (object.width)/2;
    float centrY = object.y + (object.height)/2;
    return {centrX , centrY};
}

float Dist(Vector2 player , Vector2 bullet){
    float dist = (player.x - bullet.x)*(player.x - bullet.x) + (player.y - bullet.y)*(player.y - bullet.y);
    return dist;
}

Vector2 ChooseDirection(const Game& game){

    Vector2 agentDirection = {0.0f , 0.0f};
    Vector2 playerCentr = ObjectCentr(game.PlayerBound());

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
    
    return agentDirection;
    

}

float RunEpisode(){
    Game game;
    Vector2 agentDirection = {0.0f , 0.0f};
    while (!game.IsGameOver() && game.GetSurvivalTime() < EPISODE_TIME_LIMIT)
    {   
        agentDirection = ChooseDirection(game);
        game.Update(agentDirection, SIMULATION_DT);
    }
    return game.GetSurvivalTime();
}

int main(){

    bool simulationMode = false;
    bool agentMode = true;

    InitWindow(1000,1000,"DodgeGame");
    SetTargetFPS(60);

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
        float result = RunEpisode();
        std::cout << "\033[32mResult: " << result << "\033[0m\n";
        return 0;

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
                } else
                {
                    playerDirection = ChooseDirection(game);
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

        return 0;
    }

}