#include "Player.h"
#include "GameConfig.h"
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
    
        if(body.x < 0)
    {
        body.x = 0;
    }

    if(body.y > FILE_HEIGHT - body.height)
    {
        body.y = FILE_HEIGHT - body.height;
    }

    if(body.x > FILE_WIDTH - body.width)
    {
        body.x = FILE_WIDTH - body.width;
    }

}

void Player::Restart(){
    body.x = startPosition.x;
    body.y = startPosition.y;
}



