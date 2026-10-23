#pragma once

#include "raylib.h"

class Player {

    private:
    Rectangle body = {380.0f, 380.0f, 50.0f, 50.0f};
    float speed = 600.0f;
    Vector2 startPosition = {380.0f, 380.0f};

    public:
    void Draw();
    void Update();
    void Restart();

    Rectangle GetBounds() const;
};