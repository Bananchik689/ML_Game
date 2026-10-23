#include "Projectile.h"

Projectile::Projectile(Rectangle initialBody, Vector2 initialVelocity)
    : body(initialBody), velocity(initialVelocity)
{
}

void Projectile::Draw(){
    DrawRectangleRec(body,RED);
}

void Projectile::Update(){

    float dt = GetFrameTime();
    body.x += velocity.x * dt;
    body.y += velocity.y * dt;

    if(
        (body.x + body.width > 0) &&
        (body.x < GetScreenWidth()) &&
        (body.y + body.height > 0) &&
        (body.y < GetScreenHeight())  
    )
    {
        hasEnteredScreen = true;
    }

}

Rectangle Projectile::GetBounds() const{
    return body;
}

bool Projectile::IsOffScreen() const{
    return 
            (body.x + body.width < 0) ||
            (body.x > GetScreenWidth()) ||
            (body.y + body.height < 0) ||
            (body.y > GetScreenHeight());  
}

bool Projectile::ShouldRemove() const{
    return IsOffScreen() && hasEnteredScreen;
}
