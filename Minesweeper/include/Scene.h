#pragma once

#include "Constants.h"
#include <vector>

enum class CheckState { Default = 0, Safe, Mine, Processing, InQueue, HasUnkown };
struct CheckNode {
    int x, y;
    CheckState state;
};
struct CheckGridNode{
    int val;
    CheckState state;
};

class Scene {
private:
    // 0: 空格，1-8: 数字格，9: 雷
    std::vector<std::vector<int>> grid;
    std::vector<std::vector<bool>> revealed; // 记录每个格子是否被揭示
    std::vector<std::vector<bool>> flagged;  // 标记旗帜
    std::vector<std::vector<bool>> questioned; // 标记问号
    int width, height, mineNum;
    bool firstClick; // 首次点击避免踩雷
    int cellsLeft; // 未揭示非雷格数量
    int flagCount;
    int questionCount;
    int openCount;

    std::vector<std::vector<CheckGridNode>> tempGrid;
    void CheckMine1();
    void Check1(sf::Vector2i start);
    void Check2();
    void CSP(sf::Vector2i start);
    void CalculateNumbers();
    
    template<typename FragmentFunc>
    void ForEachNeighbor(int x, int y, FragmentFunc fragFunc) {
        for (int dx = -1; dx <= 1; ++dx){
            for (int dy = -1; dy <= 1; ++dy) {
                int nx = x + dx, ny = y + dy;
                if (IsInBounds(nx, ny))
                    fragFunc(nx, ny);
            }
        }
    }
    template<typename FragmentFunc>
    void ForEachNeighborNotMe(int x, int y, FragmentFunc fragFunc) {
        for (int dx = -1; dx <= 1; ++dx){
            for (int dy = -1; dy <= 1; ++dy) {
                int nx = x + dx, ny = y + dy;
                if (IsInBounds(nx, ny) && !(nx == x && ny == y))
                    fragFunc(nx, ny);
            }
        }
    }

    int CCountNeighborUnknown(int x, int y) {
        int cnt = 0;
        ForEachNeighborNotMe(x, y, [&](int nx, int ny) {
            if (tempGrid[nx][ny].state == CheckState::Default)
                ++cnt;
        });
        return cnt;
    }
    int CCountNeighborMine(int x, int y) {
        int cnt = 0;
        ForEachNeighborNotMe(x, y, [&](int nx, int ny) {
            if (tempGrid[nx][ny].state == CheckState::Mine)
                ++cnt;
        });
        return cnt;
    }

public:
    Scene();
    void Reset(int w, int h, int m);
    void PlaceMines(int safeX, int safeY);
    bool Reveal(int x, int y); // 揭示格子，返回是否踩雷
    void RevealAll();
    void ToggleFlag(int x, int y);
    void ToggleQuestion(int x, int y);
    void QuickOpen(int x, int y); // 双击快速展开
    bool IsMine(int x, int y) const;
    bool IsFlagged(int x, int y) const;
    bool IsQuestioned(int x, int y) const;
    bool IsRevealed(int x, int y) const;
    bool IsInBounds(int x, int y) const;
    GridType GetCell(int x, int y) const;
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    int GetMineNum() const { return mineNum; }
    int GetFlagCount() const { return flagCount; }
    int GetQuestionCount() const { return questionCount; }
    int GetOpenCount() const { return openCount; }
    const std::vector<std::vector<int>>& GetScene() const { return grid; }
};