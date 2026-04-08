#include "Game.h"

#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <fstream>
#include <ctime>


Game::Game()
    : gameState(GameState::GAMEOVER), isPausedExit(false), 
    h(GetStdHandle(STD_OUTPUT_HANDLE)), renderer(h){
        dir = Direction::STOP;
        mode = ShowMode::FULL;
        score = 0;
    }

void Game::Run(){
    renderer.hideCursor();
    if (SaveSystem::HasSaveData()){
        InitPaused();
    }
    else{
        Init();
    }

    Draw();
    while (gameState != GameState::GAMEOVER){
        switch (gameState){
        case GameState::RUNNING:
            Draw();
            Sleep(DELAY);
            HandleInput();
            Logic();
            break;
        case GameState::PAUSED:
            DrawPaused();
            Sleep(DELAY);
            HandlePausedInput();
            LogicPaused();
            break;
        default:
            break;
        }
    }

    if (isPausedExit){
        SaveSystem::Save(GetGameData());
    }
    else{
        SaveSystem::ClearSaveData();
    }

    renderer.gotoXY(0, SCENE_HEIGHT + 1, mode);
    system("pause");
}

void Game::Init(){
    h = GetStdHandle(STD_OUTPUT_HANDLE);
    gameState = GameState::RUNNING;
    mode = ShowMode::FULL;
    srand(static_cast<unsigned int>(time(nullptr)));

    snake.Init({WIDTH / 2, HEIGHT / 2});
    food.Generate(snake);
    score = 0;
    dir = Direction::STOP;
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
    InputCommand cmd = input.ProcessRunningInput();
    switch (cmd){
    case Up:
        if (dir != DOWN){
            dir = UP;
        }
        break;
    case Down:
        if (dir != UP){
            dir = DOWN;
        }
        break;
    case Left:
        if (dir != RIGHT){
            dir = LEFT;
        }
        break;
    case Right:
        if (dir != LEFT){
            dir = RIGHT;
        }
        break;
    case Exit:
        gameState = GameState::GAMEOVER;
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
}

void Game::Draw(){
    scene.Build(snake, food);
    renderer.drawScene(scene, mode);
    renderer.drawScore(score, mode);
}

void Game::DrawPaused(){
    renderer.drawScore(score, mode);
    renderer.drawPause(mode);
}

void Game::InitPaused(){
    system("cls");
    std::cout << "继续游戏？（Y/n）";
    char a = _getch();
    if (a == 'Y' || a == 'y'){
        GameData loadedData;
        if (SaveSystem::Load(loadedData)){
            SetGameData(loadedData);
            gameState = GameState::PAUSED;
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
    InputCommand cmd = input.ProcessPausedInput();
    switch (cmd)
    {
    case Exit:
        isPausedExit = true;
        gameState = GameState::GAMEOVER;
        break;
    case ChangeMode:
        SwitchShowMode();
        break;
    case Pause:
        break;  // 继续保持暂停状态
    default:
        gameState = GameState::RUNNING;
        Draw();
        break;
    }
}