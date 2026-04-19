#include "Game.h"

#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <fstream>
#include <ctime>


Game::Game() : gameState(GameState::GAMEOVER), isPausedExit(false), 
    renderer(GetStdHandle(STD_OUTPUT_HANDLE)){
        dir = Direction::STOP;
        mode = ShowMode::FULL;
        score = 0;
        timeSystem = TimeSystem();
        intervalTime = 0.5f;
        intervalTimer = 0.0f;
        gameState = GameState::MENU;
    }

void Game::Run(){
    renderer.hideCursor();
    // 如果存在保存数据，进入暂停界面询问是否继续游戏，否则直接开始新游戏
    if (SaveSystem::HasSaveData()){
        InitPaused();
        Draw();
    }
    else{
        Init();
    }
    while(gameState != GameState::EXIT){
        switch (gameState)
        {
        case GameState::MENU:
            renderer.drawMenu(mode);
            if(input.GetOneKey(Confirm)){
                gameState = GameState::RUNNING;
            }
            else{
                gameState = GameState::EXIT;
            }
            break;
        case GameState::GAMEOVER:
            renderer.drawGameOver(mode);
            if (input.GetOneKey(Confirm)){
                gameState = GameState::RUNNING;
                renderer.Clean();
                Init();
            }
            else{
                gameState = GameState::EXIT;
            }
            break;
        case GameState::EXIT:
            break;
        case GameState::RUNNING:
        case GameState::PAUSED:
            while (gameState == GameState::RUNNING || gameState == GameState::PAUSED){

                if(gameState == GameState::RUNNING){
                    intervalTimer += timeSystem.GetDeltaTime();
                    HandleInput();
                    if(intervalTimer >= intervalTime){
                        Logic();
                        intervalTimer = 0.0f;
                    }
                    Draw();
                }

                else if(gameState == GameState::PAUSED){
                    DrawPaused();
                    HandlePausedInput();
                    LogicPaused();
                }
                else break;
            }
        default:
            break;
        }
    }
    // 游戏结束，判断是否保存游戏
    if (isPausedExit){
        SaveSystem::Save(GetGameData());
    }
    else{
        SaveSystem::ClearSaveData();
    }
    
    renderer.drawExit(mode);
}

void Game::Init(){
    srand(static_cast<unsigned int>(time(nullptr)));

    snake.Init({WIDTH / 2, HEIGHT / 2});

    food.Generate(snake);

    score = 0;
    dir = Direction::STOP;
    intervalTimer = 0.0f;
}

void Game::Logic(){
    if (dir == Direction::STOP){
        return;
    }

    if(!snake.CanMove(dir)){
        gameState = GameState::GAMEOVER;
        return;
    }
    
    Vector2 newHead = snake.GetHead() + DIR_MAP.at(dir);
    bool eat = (newHead == food.GetPosition());
    snake.Move(dir, eat);
    if (eat){
        food.Generate(snake);
        score += 1;
    }
}

void Game::HandleInput(){
    InputCommand cmd = input.GInput();
    Direction newDir = dir;
    switch (cmd){
    case Up: newDir = UP; break;
    case Down: newDir = DOWN; break;
    case Left: newDir = LEFT; break;
    case Right: newDir = RIGHT; break;
    case Exit:
        gameState = GameState::EXIT;
        break;
    case Pause:
        gameState = GameState::PAUSED;
        break;
    case ChangeMode:
        SwitchShowMode();
        break;
    default:
        break;
    }
    if(!IsOpposite(dir, newDir)) dir = newDir;

    if(input.KeyDown(Sprint)){
        intervalTime = GetIntervalInScore().intervalTime * 0.5f;
    }
    else{
        intervalTime = GetIntervalInScore().intervalTime;
    }
}

void Game::Draw(){
    scene.Build(snake, food);
    renderer.drawScene(scene, mode);
    renderer.UpdateGAMEINFO(GetIntervalInScore().name, std::to_string(score), mode);
}

void Game::DrawPaused(){
    renderer.drawPause(mode);
}

void Game::InitPaused(){
    renderer.Clean();
    renderer.Out("继续游戏？（Y/n）");
    if (input.GetOneKey(Confirm)){
        GameData loadedData;
        if (SaveSystem::Load(loadedData)){
            SetGameData(loadedData);
            gameState = GameState::PAUSED;
            intervalTimer = 0.0f;
        }
        else{
            SaveSystem::ClearSaveData();
            Init();
            gameState = GameState::RUNNING;
        }
    }
    else{
        Init();
        gameState = GameState::RUNNING;
    }
}

void Game::LogicPaused(){
}

void Game::HandlePausedInput(){
    InputCommand cmd = input.GInput();
    switch (cmd)
    {
    case Exit:
        isPausedExit = true;
        gameState = GameState::EXIT;
        break;
    case ChangeMode:
        SwitchShowMode();
        break;
    case None: case Confirm:
        break;  // 无输入，继续保持暂停状态
    default:
        gameState = GameState::RUNNING;
        Draw();
        break;
    }
}