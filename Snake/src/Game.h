#pragma once

#include "Constants.h"
#include "SaveSystem.h"
#include "Renderer.h"
#include <windows.h>
#include <string>
#include <unordered_map>

class Game {
public:
    Game();
    void Run();

private:
    GameData gameData;
    GameState gameState;
    bool isPausedExit;
    HANDLE h;
    Renderer renderer;
    CodeType scene[SCENE_HEIGHT][SCENE_WIDTH];

    void SetShowMode(ShowMode mode);
    void ChangeShowMode();
    void Init();
    void Input();
    void Logic();
    void InitPaused();
    void InputPause();
    void LogicPause();

    Vector2 GetFoodPos() const {
        Vector2 pos;
        do {
            pos.x = rand() % WIDTH;
            pos.y = rand() % HEIGHT;
        } while (Check(pos, CodeType::SNAKE));
        return pos;
    }

    void SetGameScene(const Vector2& pos, CodeType type) {
        Vector2 scenePos = GetScenePos(pos);
        scene[scenePos.y][scenePos.x] = type;

        //SetConsoleTextAttribute(h, colorMap.at(type));
        //gotoXY(scenePos.x, scenePos.y);
        //std::cout << curCodeMap.at(scene[scenePos.y][scenePos.x]);
    }

    Vector2 GetScenePos(int x, int y) const {
        return { x + WALL_WIDTH, y + WALL_WIDTH };
    }

    Vector2 GetScenePos(const Vector2& pos) const {
        return GetScenePos(pos.x, pos.y);
    }

    bool Check(const Vector2& pos, CodeType type) const {
        Vector2 scenePos = GetScenePos(pos);
        return scene[scenePos.y][scenePos.x] == type;
    }

    void ResetScene() {
        for (int k = 0; k < WALL_WIDTH; k++) {
            for (int i = 0; i < SCENE_HEIGHT; i++) {
                scene[i][k] = CodeType::WALL;
                scene[i][SCENE_WIDTH - 1 - k] = CodeType::WALL;
            }
        
            for (int j = 0; j < SCENE_WIDTH; j++) {
                scene[k][j] = CodeType::WALL;
                scene[SCENE_HEIGHT - 1 - k][j] = CodeType::WALL;
            }
        
            for (int i = 0; i < HEIGHT; i++) {
                for (int j = 0; j < WIDTH; j++) {
                    SetGameScene({ j, i }, CodeType::DEFAULT);
                }
            }
        }
    }
};
