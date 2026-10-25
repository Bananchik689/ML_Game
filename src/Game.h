#pragma once 

#include "raylib.h"
#include "Player.h"
#include "Projectile.h"
#include <cmath>
#include <vector>

class Game {
    
    private:
        Player player;
        std::vector<Projectile> bullets;

        float survivalTime = 0;
        float spawnTime = 0;
        float spawnInterval = 1.0;
        bool gameOver = false;

        float spawnRadius = std::hypot(GetScreenHeight() / 2.0f , GetScreenWidth() / 2.0f ) + 50;
        int centrX = GetScreenWidth() / 2;
        int centrY = GetScreenHeight() / 2;

    public:
        void Update(Vector2 playerDirection , float dt);
        void Draw();
        void Reset();
        
        bool IsGameOver() const;
        float GetSurvivalTime() const;
        

};

