#pragma once

#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

enum Direction { STOP = 0, UP, DOWN, LEFT, RIGHT };
// ShowMode is used to determine how to display the scene (half-width or full-width characters)
enum class ShowMode { HALF, FULL };
// CodeType is used to determine the character and color to display for each cell in the scene
enum class CodeType { DEFAULT, WALL, SNAKE, FOOD, TEXT, SCORE, DIFFICULTY };
enum class GameState { MENU, GAMEOVER, RUNNING, PAUSED, EXIT };

struct Vector2 {
    int x, y;
    Vector2(int a = 0, int b = 0) : x(a), y(b) {}
    Vector2 operator+(const Vector2& other) const { return { x + other.x, y + other.y }; }
    Vector2& operator+=(const Vector2& other) { x += other.x; y += other.y; return *this; }
    bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }
};

struct GameData {
    ShowMode mode = ShowMode::FULL;
    std::vector<Vector2> snakeBody;
    Vector2 foodPos;
    Direction dir = Direction::STOP;
    int score = 0;
};

inline constexpr int WIDTH = 20;
inline constexpr int HEIGHT = 20;
inline constexpr int WALL_WIDTH = 2;
inline constexpr int SCENE_WIDTH = WIDTH + WALL_WIDTH * 2;
inline constexpr int SCENE_HEIGHT = HEIGHT + WALL_WIDTH * 2;
inline constexpr int INFOSETOFF = 5;
inline const std::string SAVE_FILE_NAME = "data/GameData.txt";

inline const std::unordered_map<Direction, Vector2> DIR_MAP = {
    {UP, {0, -1}},
    {DOWN, {0, 1}},
    {LEFT, {-1, 0}},
    {RIGHT, {1, 0}}
};

// codeMapHalf and codeMapFull are used to determine the character to display for each CodeType in half-width and full-width modes respectively
inline const std::unordered_map<CodeType, std::string> codeMapHalf = {
    {CodeType::DEFAULT, " "},
    {CodeType::WALL, "#"},
    {CodeType::SNAKE, "O"},
    {CodeType::FOOD, "*"}
};

inline const std::unordered_map<CodeType, std::string> codeMapFull = {
    {CodeType::DEFAULT, "  "},
    {CodeType::WALL, "＃"},
    {CodeType::SNAKE, "Ｏ"},
    {CodeType::FOOD, "＊"}
};
// colorMap is used to determine the color to display for each CodeType
inline const std::unordered_map<CodeType, int> colorMap = {
    {CodeType::DEFAULT, 0x00},
    {CodeType::WALL, 0x0f},
    {CodeType::SNAKE, 0x03},
    {CodeType::FOOD, 0x0d},
    {CodeType::TEXT, 0x0f}
};

inline const std::vector<std::tuple<std::string, CodeType, int>> INFO = {
    {"■操作说明：", CodeType::TEXT, 0},
    {"□开始游戏：WASD", CodeType::SNAKE, INFOSETOFF * 0.5},
    {"□加速：按住Alt", CodeType::SNAKE, INFOSETOFF * 0.5},
    {"□暂停游戏：空格", CodeType::TEXT, INFOSETOFF * 0.5},
    {"□继续游戏：任意键", CodeType::TEXT, INFOSETOFF * 0.5},
    {"□退出游戏：Esc", CodeType::TEXT, INFOSETOFF * 0.5},
    {"□显示模式切换：回车", CodeType::TEXT, INFOSETOFF * 0.5},
    {"\n", CodeType::TEXT, 0},
    {"■存档说明：", CodeType::TEXT, 0},
    {"□暂停后Esc退出游戏，会存档", CodeType::TEXT, INFOSETOFF * 0.5},
    {"□关闭窗口后再次打开，会尝试读档", CodeType::TEXT, INFOSETOFF * 0.5},
    {"\n", CodeType::TEXT, 0},
};

inline const std::vector<std::tuple<std::string, CodeType, CodeType, int>> GAMEINFO = {
    {"■当前分数：", CodeType::SCORE, CodeType::TEXT, 0},
    {"\n", CodeType::DEFAULT, CodeType::TEXT, 0},
    {"■当前难度：", CodeType::DIFFICULTY, CodeType::TEXT, 0}
};

struct Difficulty {
    int scoreThreshold;
    float intervalTime;
    std::string name;
};

inline const std::vector<Difficulty> difficultyTable = {
    {0, 0.5f, "1"},
    {5, 0.4f, "2"},
    {10, 0.3f, "高级"},
    {20, 0.2f, "难"},
    {30, 0.1f, "地狱"}
};

inline const std::vector<std::tuple<std::string, CodeType, int>> MENU = {
    {"■贪吃蛇小游戏■", CodeType::TEXT, 0},
    {"\n", CodeType::DEFAULT, 0},
    {"\n", CodeType::DEFAULT, 0},
    {"开始游戏？（Y/n）", CodeType::TEXT, INFOSETOFF * 0.5},
};