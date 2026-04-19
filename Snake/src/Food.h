#pragma once

#include "Constants.h"
#include "Snake.h"

class Food {
public:
    Food();
    // 生成食物，确保不与蛇身重叠
    void Generate(const Snake& snake);
    void SetPosition(const Vector2& pos);
    Vector2 GetPosition() const;

private:
    Vector2 position;
};