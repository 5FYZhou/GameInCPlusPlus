#pragma once

#include "Constants.h"
#include <vector>

class Snake {
public:
    Snake();
    void Init(const Vector2& startPos);
    void SetBody(const std::vector<Vector2>& newBody);
    // 判断蛇是否可以向指定方向移动
    bool CanMove(Direction dir) const;
    // 移动蛇，grow表示是否吃到食物
    void Move(Direction dir, bool grow);
    // 检查蛇是否与指定位置发生碰撞
    bool CheckCollision(const Vector2& pos) const;
    const std::vector<Vector2>& GetBody() const;
    Vector2 GetHead() const;

private:
    std::vector<Vector2> body;
};