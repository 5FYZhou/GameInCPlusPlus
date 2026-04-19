#include "Snake.h"

Snake::Snake() {}

void Snake::Init(const Vector2& startPos) {
    body.clear();
    body.push_back(startPos);
}

void Snake::SetBody(const std::vector<Vector2>& newBody) {
    body = newBody;
}

bool Snake::CanMove(Direction dir) const {
    Vector2 newHead = body[0] + DIR_MAP.at(dir);
    // 判断新位置是否撞墙或撞到自己
    bool isHitWall = (newHead.x < 0 || newHead.x >= WIDTH || newHead.y < 0 || newHead.y >= HEIGHT);
    return !(isHitWall || CheckCollision(newHead));
}

void Snake::Move(Direction dir, bool grow) {
    Vector2 newHead = body[0] + DIR_MAP.at(dir);
    // 在头部插入新位置
    body.insert(body.begin(), newHead);
    // 如果不吃食物且蛇身长度大于1，移除尾部
    if (!grow && !body.empty()) {
        body.pop_back();
    }
}

bool Snake::CheckCollision(const Vector2& pos) const {
    for (const auto& segment : body) {
        if (segment == pos) {
            return true;
        }
    }
    return false;
}

const std::vector<Vector2>& Snake::GetBody() const {
    return body;
}

Vector2 Snake::GetHead() const {
    return body.empty() ? Vector2() : body[0];
}