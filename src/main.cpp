#include "raylib.h"
#include "raymath.h"
#include "Player.h"
#include "Projectile.h"
#include <vector>
#include <cmath>

int main(){

    InitWindow(1000,1000,"DodgeGame");
    float spawnRadius = std::hypot(GetScreenHeight() / 2.0f , GetScreenWidth() / 2.0f ) + 50;
    SetTargetFPS(120);

    Player player;

    std::vector<Projectile> bullets;

    


    bool gameOver = false;

    //Text for the end of the game

    const char *final_text = "GAME OVER";
    
    int centrX = GetScreenWidth() / 2;
    int centrY = GetScreenHeight() / 2;

    int size_final_text = 90;
    int width_final_text = MeasureText(final_text , size_final_text);

    int posX = centrX - width_final_text/2;
    int posY = centrY - size_final_text/2;


    float survivalTime = 0;
    float spawnTime = 0;
    float interval = 0.2;
    int Record = 0;





    






    while(!WindowShouldClose()){

        


        if(!gameOver)
        {
            survivalTime += GetFrameTime();
            spawnTime += GetFrameTime();

            player.Update();

            if (spawnTime >= interval)
            {
                float angle = GetRandomValue(0, 359) * DEG2RAD;

                float spawnX = centrX + spawnRadius * cos(angle);
                float spawnY = centrY + spawnRadius * sin(angle);

                float targetX = GetRandomValue(50, GetScreenWidth() - 50);
                float targetY = GetRandomValue(50, GetScreenHeight() - 50);
                Vector2 direction = {targetX - spawnX, targetY - spawnY};
                direction = Vector2Normalize(direction);

                int speedBullet = GetRandomValue(400,700);


                bullets.push_back(Projectile(
                    Rectangle{spawnX, spawnY, 20, 20},
                    Vector2{direction.x * speedBullet, direction.y * speedBullet}
                ));

                spawnTime -= interval;
            }

            for(Projectile& bullet : bullets )
            {
                bullet.Update();
                bool hit = CheckCollisionRecs(player.GetBounds(), bullet.GetBounds());
                if (hit == true)
                {
                    gameOver = true;
                    break;
                }
            }

            for (auto it = bullets.begin(); it != bullets.end();)
            {
                if(it -> ShouldRemove())
                {
                    it = bullets.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            

            
           

        }

        if(IsKeyPressed(KEY_R) && gameOver)
        {   
            player.Restart();
            bullets.clear();
            
            survivalTime = 0;
            spawnTime = 0;
            gameOver = false;
            
        }
        BeginDrawing();

       
        ClearBackground(RAYWHITE);

        for(Projectile& bullet : bullets )
        {
            bullet.Draw();
        }
        
        player.Draw();

        if(gameOver)
        {
            DrawText(final_text,posX,posY,size_final_text,RED);


            Record = (Record > survivalTime) ? Record : survivalTime;
            const char *record_txt = TextFormat("Record: %i" , static_cast<int>(Record));
            int size_record_txt = 50;
            int width_record_txt = MeasureText(record_txt,size_record_txt);
            int posXrecord = centrX - width_record_txt / 2;
            int posYrecord = posY + 100;

            DrawText(record_txt,posXrecord,posYrecord,size_record_txt,GREEN);
        }



        //Time text

        const char *time_txt = TextFormat("Time: %i" , static_cast<int>(survivalTime));

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