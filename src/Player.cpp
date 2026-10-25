#include "Player.h"
#include "raymath.h"

void Player::Draw(){

    DrawRectangleRec(body,BLACK);

}

Rectangle Player::GetBounds() const{
    return body;
}

void Player::Update(Vector2 direction, float dt){

    direction = Vector2Normalize(direction);
    body.x += direction.x * speed * dt;
    body.y += direction.y * speed * dt;

    // Restrictions

    if(body.y < 0)
    {
        body.y = 0;
    }

    if(body.y > GetScreenHeight() - body.height)
    {
        body.y = GetScreenHeight() - body.height;
    }

    if(body.x > GetScreenWidth() - body.width)
    {
        body.x = GetScreenWidth() - body.width;
    }

    if(body.x < 0)
    {
        body.x = 0;
    }

}

void Player::Restart(){
    body.x = startPosition.x;
    body.y = startPosition.y;
}



