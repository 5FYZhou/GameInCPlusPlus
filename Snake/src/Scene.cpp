#include "Scene.h"

Scene::Scene():grid(SCENE_HEIGHT, std::vector<CodeType>(SCENE_WIDTH)) {
    // 初始化grid为默认值
    for (int i = 0; i < SCENE_HEIGHT; i++) {
        for (int j = 0; j < SCENE_WIDTH; j++) {
            grid[i][j] = CodeType::DEFAULT;
        }
    }
    // 设置墙壁
    for (int k = 0; k < WALL_WIDTH; k++) {
        for (int i = 0; i < SCENE_HEIGHT; i++) {
            grid[i][k] = CodeType::WALL;
            grid[i][SCENE_WIDTH - 1 - k] = CodeType::WALL;
        }
        for (int j = 0; j < SCENE_WIDTH; j++) {
            grid[k][j] = CodeType::WALL;
            grid[SCENE_HEIGHT - 1 - k][j] = CodeType::WALL;
        }
    }
}

void Scene::Build(const Snake& snake, const Food& food) {
    // 重置内部区域为DEFAULT
    for (int i = WALL_WIDTH; i < SCENE_HEIGHT - WALL_WIDTH; i++) {
        for (int j = WALL_WIDTH; j < SCENE_WIDTH - WALL_WIDTH; j++) {
            grid[i][j] = CodeType::DEFAULT;
        }
    }
    // 设置蛇的身体
    for (const auto& pos : snake.GetBody()) {
        Vector2 scenePos = { pos.x + WALL_WIDTH, pos.y + WALL_WIDTH };
        grid[scenePos.y][scenePos.x] = CodeType::SNAKE;
    }
    // 设置食物
    Vector2 foodScenePos = { food.GetPosition().x + WALL_WIDTH, food.GetPosition().y + WALL_WIDTH };
    grid[foodScenePos.y][foodScenePos.x] = CodeType::FOOD;
}