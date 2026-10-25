#include "raylib.h"
#include "Game.h"
#include <vector>

int main(){

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


    while(!WindowShouldClose()){

        if(!game.IsGameOver())
        {   
            float dt = GetFrameTime();

            Vector2 playerDirection = {0.0f , 0.0f};

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