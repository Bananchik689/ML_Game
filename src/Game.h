#pragma once 

#include "raylib.h"
#include "Player.h"
#include "Projectile.h"
#include "GameConfig.h"
#include <cmath>
#include <vector>

class Game {
    
    private:
        Player player;
        std::vector<Projectile> bullets;

        float survivalTime = 0;
        float spawnTime = 0;
        float spawnInterval = 0.2;
        bool gameOver = false;

        float spawnRadius = std::hypot(FILE_WIDTH / 2.0f , FILE_HEIGHT / 2.0f ) + 50;
        int centrX = FILE_WIDTH / 2;
        int centrY = FILE_HEIGHT / 2;

    public:
        void Update(Vector2 playerDirection , float dt);
        void Draw();
        void Reset();
        
        bool IsGameOver() const;
        float GetSurvivalTime() const;
        Rectangle PlayerBound() const;
        std::vector<Rectangle> BulletsBound() const;
        

};

