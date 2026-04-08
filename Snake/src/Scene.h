#pragma once

#include "Constants.h"
#include "Snake.h"
#include "Food.h"
#include <vector>

class Scene {
private:
    std::vector<std::vector<CodeType>> grid;

public:
    Scene();
    void Build(const Snake& snake, const Food& food);
    const std::vector<std::vector<CodeType>>& GetScene() const { return grid; }
};