#pragma once

#include "Constants.h"
#include "SaveSystem.h"
#include "TimeSystem.h"
#include "Renderer.h"
#include "Input.h"
#include "Snake.h"
#include "Food.h"
#include "Scene.h"
#include <windows.h>
#include <string>
#include <unordered_map>
#include <vector>

class Game {
public:
    Game();
    void Run();

private:
    Snake snake; Food food; Scene scene;
    Direction dir; ShowMode mode; int score;
    GameState gameState;
    bool isPausedExit;

    Renderer renderer;
    Input input;
    TimeSystem timeSystem;
    float intervalTime;
    float intervalTimer;

    void Init();
    void Logic();
    void HandleInput();
    void Draw();

    void InitPaused();
    void LogicPaused();
    void HandlePausedInput();
    void DrawPaused();
    
    void SwitchShowMode(){
        if (mode == ShowMode::FULL){
            mode = ShowMode::HALF;
        }
        else{
            mode = ShowMode::FULL;
        }
        system("cls");
        Draw();
    };

    GameData GetGameData() const {
        GameData data;
        data.mode = mode;
        data.snakeBody = snake.GetBody();
        data.foodPos = food.GetPosition();
        data.dir = dir;
        data.score = score;
        return data;
    }

    void SetGameData(const GameData& data) {
        mode = data.mode;
        snake.SetBody(data.snakeBody);
        food.SetPosition(data.foodPos);
        dir = data.dir;
        score = data.score;    
    }

    bool IsOpposite(Direction dir1, Direction dir2) {
        return (dir1 == UP && dir2 == DOWN) || (dir1 == DOWN && dir2 == UP) ||
               (dir1 == LEFT && dir2 == RIGHT) || (dir1 == RIGHT && dir2 == LEFT);
    }

    Difficulty GetIntervalInScore() const {
        Difficulty currentDifficulty = difficultyTable[0];
        for(const auto& diff: difficultyTable){
            if(score >= diff.scoreThreshold){
                currentDifficulty = diff;
            }
            else{
                break;
            }
        }
        return currentDifficulty;
    }


};
