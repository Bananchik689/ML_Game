#include "Game.h"
#include "GameConfig.h"
#include "raymath.h"
#include <cmath>

void Game::Update(Vector2 playerDirection , float dt){
    if(!gameOver)
    {
        survivalTime += dt;
        spawnTime += dt;

        player.Update(playerDirection , dt);

        //create projectiles at a specific interval and with random direction and speed
        if (spawnTime >= spawnInterval)
            {
                float angle = GetRandomValue(0, 359) * DEG2RAD;

                float spawnX = centrX + spawnRadius * cos(angle);
                float spawnY = centrY + spawnRadius * sin(angle);

                float targetX = GetRandomValue(50, FILE_WIDTH - 50);
                float targetY = GetRandomValue(50, FILE_HEIGHT - 50);
                Vector2 bulletDirection = {targetX - spawnX, targetY - spawnY};
                bulletDirection = Vector2Normalize(bulletDirection);

                int speedBullet = GetRandomValue(400,700);


                bullets.push_back(Projectile(
                    Rectangle{spawnX, spawnY, 20, 20},
                    Vector2{bulletDirection.x * speedBullet, bulletDirection.y * speedBullet}
                ));

                spawnTime -= spawnInterval;
            }


            for(Projectile& bullet : bullets )
            {
                bullet.Update(dt);
                bool hit = CheckCollisionRecs(player.GetBounds(), bullet.GetBounds());
                if (hit == true)
                {
                    gameOver = true;
                    break;
                }
            }

            // remove projectiles outside the field
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

}

void Game::Reset(){

    player.Restart();
    bullets.clear();
            
    survivalTime = 0;
    spawnTime = 0;
    gameOver = false;
}

bool Game::IsGameOver() const{
    return gameOver;
}

float Game::GetSurvivalTime() const{
    return survivalTime;
}

void Game::Draw(){
        for(Projectile& bullet : bullets )
        {
            bullet.Draw();
        }
        player.Draw();
}