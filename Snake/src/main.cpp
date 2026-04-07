#define UNICODE
#include <windows.h>
#include <iostream>
#include <unordered_map>
#include <conio.h>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <time.h>
#include <iomanip>
#include <fstream>
#include "Constants.h"
#include "SaveSystem.h"
using namespace std;

#pragma region GameData
GameData gameData;
unordered_map<CodeType, string> curCodeMap;
GameState gameState;
bool isPausedExit = false;
CodeType scene[SCENE_HEIGHT][SCENE_WIDTH]; // Game scene representation
HANDLE h;
#pragma endregion

void DrawScene();

#pragma region tools
void SetShowMode(ShowMode mode) {
    if (mode == ShowMode::HALF) {
        curCodeMap = codeMapHalf;
    }
    else {
        curCodeMap = codeMapFull;
    }
    gameData.mode = mode;
}
void ChangeShowMode() {
    if (gameData.mode == ShowMode::FULL)
        SetShowMode(ShowMode::HALF);
    else
        SetShowMode(ShowMode::FULL);
    system("cls");
    DrawScene();
}

Vector2 GetScenePos(int x, int y) {
    return { x + WALL_WIDTH, y + WALL_WIDTH }; // Adjust for wall offset
}
Vector2 GetScenePos(Vector2 pos) {
    return { pos.x + WALL_WIDTH, pos.y + WALL_WIDTH }; // Adjust for wall offset
}

bool Check(Vector2 pos, CodeType type) {
    Vector2 scenePos = GetScenePos(pos);
    //return scene[scenePos.y][scenePos.x] == curCodeMap[CodeType::WALL];
    return scene[scenePos.y][scenePos.x] == type;
}

Vector2 GetFoodPosition() {
    Vector2 foodPos;
    // Generate random food position
    foodPos.x = rand() % (WIDTH);
    foodPos.y = rand() % (HEIGHT);
    while (!Check(foodPos, CodeType::DEFAULT)) {
        foodPos.x = rand() % (WIDTH);
        foodPos.y = rand() % (HEIGHT);
    }
    return foodPos;
}

void GotoXY(int x, int y) {
    COORD pos;
    if (gameData.mode == ShowMode::HALF)
        pos = { (SHORT)x, (SHORT)y };
    else
        pos = { (SHORT)(x * 2), (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void GotoXY(Vector2 p) {
    GotoXY(p.x, p.y);
}

void SetGameScene(Vector2 pos, CodeType type) {
    Vector2 scenePos = GetScenePos(pos);
    scene[scenePos.y][scenePos.x] = type;

    SetConsoleTextAttribute(h, colorMap.at(type));
    GotoXY(scenePos.x, scenePos.y);
    cout << curCodeMap.at(scene[scenePos.y][scenePos.x]);
}

void HideCursor() {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(handle, &cursorInfo);
    cursorInfo.bVisible = FALSE; // 隐藏
    SetConsoleCursorInfo(handle, &cursorInfo);
}
#pragma endregion

#pragma region Data
void ResetScene() {
    h = GetStdHandle(STD_OUTPUT_HANDLE);
    HideCursor();
    for (int k = 0; k < WALL_WIDTH; k++) {
        for (int i = 0; i < SCENE_HEIGHT; i++) {
            scene[i][k] = CodeType::WALL;
            scene[i][SCENE_WIDTH - 1 - k] = CodeType::WALL;
        }
        for (int j = 0; j < SCENE_WIDTH; j++) {
            scene[k][j] = CodeType::WALL;
            scene[SCENE_HEIGHT - 1 - k][j] = CodeType::WALL;
        }
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            scene[i + WALL_WIDTH][j + WALL_WIDTH] = CodeType::DEFAULT;
        }
    }
}
#pragma endregion

#pragma region RUNNING
void Init() {
    // Initialize game state
    h = GetStdHandle(STD_OUTPUT_HANDLE);
    gameState = GameState::RUNNING;
    SetShowMode(ShowMode::FULL);
    srand((time(nullptr)));	//设置随机种子
    HideCursor();
    for (int k = 0; k < WALL_WIDTH; k++) {
        for (int i = 0; i < SCENE_HEIGHT; i++) {
            scene[i][k] = CodeType::WALL;
            scene[i][SCENE_WIDTH - 1 - k] = CodeType::WALL;
        }
        for (int j = 0; j < SCENE_WIDTH; j++) {
            scene[k][j] = CodeType::WALL;
            scene[SCENE_HEIGHT - 1 - k][j] = CodeType::WALL;
        }
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            SetGameScene({ j, i }, CodeType::DEFAULT);
        }
    }

    gameData.snake.clear();
    gameData.snake.push_back({ WIDTH / 2, HEIGHT / 2 }); // Start in the middle
    SetGameScene(gameData.snake[0], CodeType::SNAKE);

    gameData.food = GetFoodPosition();
    SetGameScene(gameData.food, CodeType::FOOD);

    gameData.score = 0;
}

void DrawInfo() {
    for (int i = 0; i < INFO.size(); i++) {
        const auto& [text, code, setoff] = INFO[i];
        SetConsoleTextAttribute(h, colorMap.at(code));
        GotoXY(SCENE_WIDTH + INFOSETOFF + setoff, INFOSETOFF + i);
        cout << text;
    }
}

void DrawScene() {
    // Draw the game state based on the show mode
    GotoXY(0, 0);
    for (int i = 0; i < SCENE_HEIGHT; i++) {
        for (int j = 0; j < SCENE_WIDTH; j++) {
            //cout << colorMap[scene[i][j]];
            SetConsoleTextAttribute(h, colorMap.at(scene[i][j]));
            //cout << scene[i][j];
            std::cout << curCodeMap[scene[i][j]];
        }
        std::cout << endl;
    }
    DrawInfo();
}

void DrawScore() {
    SetConsoleTextAttribute(h, colorMap.at(CodeType::SCORE) );
    GotoXY(0, SCENE_HEIGHT);
    std::cout << "Score:" << setw(5) << gameData.score;
}

void Input() {
    // Handle user input
    if (_kbhit()) {
        switch (_getch()) {
        case 'w':
            if (gameData.dir != DOWN)
                gameData.dir = UP;
            break;
        case 's':
            if (gameData.dir != UP)
                gameData.dir = DOWN;
            break;
        case 'a':
            if (gameData.dir != RIGHT)
                gameData.dir = LEFT;
            break;
        case 'd':
            if (gameData.dir != LEFT)
                gameData.dir = RIGHT;
            break;
        case 27:
            gameState = GameState::GAMEOVER;
            break;
        case 32:
            gameState = GameState::PAUSED;
            break;
        case 13:
            ChangeShowMode();
            break;
        }
    }
}

void Logic() {
    // Update game logic
    if (gameData.dir == Direction::STOP) return; // Don't move if direction is STOP
    Vector2 newHead = gameData.snake[0] + DIR_MAP.at(gameData.dir);

    if (Check(newHead, CodeType::WALL) || Check(newHead, CodeType::SNAKE)) {
        gameState = GameState::GAMEOVER;
        return;
    }
    gameData.snake.insert(gameData.snake.begin(), newHead);
    if (newHead == gameData.food) {
        gameData.food = GetFoodPosition();
        SetGameScene(gameData.food, CodeType::FOOD);
        gameData.score += 1;
    }
    else {
        Vector2 tail = gameData.snake.back();
        SetGameScene(tail, CodeType::DEFAULT);
        gameData.snake.pop_back();
    }
    SetGameScene(newHead, CodeType::SNAKE);
}
#pragma endregion

#pragma region PAUSED
void InitPaused() {
    system("cls");
    cout << "继续游戏？（Y/n）";
    char a = _getch();
    if (a == 'Y' || a == 'y') {
        if (SaveSystem::Load(gameData)) {
            SetShowMode(gameData.mode);
            ResetScene();
            for (auto& pos : gameData.snake) {
                SetGameScene(pos, CodeType::SNAKE);
            }
            SetGameScene(gameData.food, CodeType::FOOD);
            gameState = GameState::PAUSED;
        }
        else {
            Init();
            gameState = GameState::RUNNING;
        }
    }
    else {
        Init();
        gameState = GameState::RUNNING;
    }
}

void InputPause() {
    if (_kbhit()) {
        switch (_getch()) {
        case 27:
            isPausedExit = true;
            gameState = GameState::GAMEOVER;
            break;
        case 13:
            ChangeShowMode();
            break;
        default:
            gameState = GameState::RUNNING;
            DrawScene();
            break;
        }
    }
}

void LogicPause() {

}

void DrawPause() {
    if (gameData.mode == ShowMode::HALF)
        GotoXY(GetScenePos(WIDTH / 2 - 3, HEIGHT / 2));
    else
        GotoXY(GetScenePos(WIDTH / 2 - 1.5, HEIGHT / 2));
    SetConsoleTextAttribute(h, colorMap.at(CodeType::SCORE));
    std::cout << "Paused";
    return;

    for (int i = 0; i < HEIGHT; i++) {
        GotoXY(GetScenePos(0, i));
        for (int j = 0; j < WIDTH; j++) {
            if (i == HEIGHT / 2 && j == WIDTH / 2 - 3) {
                SetConsoleTextAttribute(h, colorMap.at(CodeType::SCORE));
                std::cout << "Paused";
                j += 5;
                continue;
            }
            SetConsoleTextAttribute(h, colorMap.at(CodeType::DEFAULT));
            std::cout << curCodeMap.at(CodeType::DEFAULT);
        }
        std::cout << endl;
    }
}
#pragma endregion

int main() {
    // 设置控制台输出编码为UTF-8
    SetConsoleOutputCP(CP_UTF8);
    //有存档
    if (SaveSystem::HasSaveData()) {
        InitPaused();
    }
    else {
        Init();
    }
    DrawScene();
    while (gameState != GameState::GAMEOVER) {
        switch (gameState)
        {
        case GameState::RUNNING:
            DrawScore();
            Sleep(DELAY);
            Input();
            Logic();
            break;
        case GameState::PAUSED:
            DrawPause();
            Sleep(DELAY);
            InputPause();
            LogicPause();
            break;
        default:
            break;
        }
    }
    if (isPausedExit) {
        SaveSystem::Save(gameData);
    }
    else {
        // 清空文件
        ofstream out(SAVE_FILE_NAME, ios::trunc);
        out.close();
    }
    GotoXY(0, SCENE_HEIGHT + 1);
    system("pause");
    return 0;
}