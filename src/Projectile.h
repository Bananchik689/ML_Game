#pragma once
#include "raylib.h"

class Projectile {

    private:
        bool hasEnteredScreen = false;
        Rectangle body;
        Vector2 velocity;
    
    public:
        void Draw();
        void Update(float dt);
        bool IsOffScreen() const;
        bool ShouldRemove() const;
        Rectangle GetBounds() const;

        Projectile(Rectangle initialBody, Vector2 initialVelocity);
    

};



