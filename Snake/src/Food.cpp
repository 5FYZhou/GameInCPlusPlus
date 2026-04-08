#include "Food.h"
#include "Snake.h"
#include <cstdlib>

Food::Food() {}

void Food::Generate(const Snake& snake) {
    do {
        position.x = rand() % WIDTH;
        position.y = rand() % HEIGHT;
    } while (snake.CheckCollision(position));
}

void Food::SetPosition(const Vector2& pos) {
    position = pos;
}

Vector2 Food::GetPosition() const {
    return position;
}