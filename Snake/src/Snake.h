#pragma once

#include "Constants.h"
#include <vector>

class Snake {
public:
    Snake();
    void Init(const Vector2& startPos);
    void SetBody(const std::vector<Vector2>& newBody);
    bool CanMove(Direction dir) const;
    void Move(Direction dir, bool grow);
    bool CheckCollision(const Vector2& pos) const;
    const std::vector<Vector2>& GetBody() const;
    Vector2 GetHead() const;

private:
    std::vector<Vector2> body;
};