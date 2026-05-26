#include "Scene.h"
#include <random>
#include <ctime>
#include <algorithm>
#include <functional>
#include <queue>
#include <iostream>
#include <functional> // for std::hash
#include <map>

Scene::Scene() {
    Reset(9, 9, 10);
}

void Scene::Reset(int w, int h, int m) {
    width = w;
    height = h;
    mineNum = m;
    grid.assign(height, std::vector<int>(width, -1));
    revealed.assign(height, std::vector<bool>(width, false));
    flagged.assign(height, std::vector<bool>(width, false));
    questioned.assign(height, std::vector<bool>(width, false));
    firstClick = true;
    cellsLeft = width * height - mineNum;
    flagCount = 0;
    questionCount = 0;
    openCount = 0;
}

void Scene::PlaceMines(int safeX, int safeY) {
    // 随机布雷，首点及邻居安全
    std::vector<sf::Vector2i> positions;
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            if (!(std::abs(i - safeX) <= 1 && std::abs(j - safeY) <= 1))
                positions.emplace_back(i, j);
    std::shuffle(positions.begin(), positions.end(), std::default_random_engine((unsigned)time(nullptr)));
    for (int k = 0; k < mineNum; ++k) {
        int x = positions[k].x, y = positions[k].y;
        grid[x][y] = 9;
    }
    CalculateNumbers();
    
    tempGrid.resize(height, std::vector<CheckGridNode>(width));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            tempGrid[i][j] = {grid[i][j], CheckState::Default};
        }
    }
    Check1({safeX, safeY});
    Check2();
}

void Scene::CheckMine1(){
    std::vector<std::vector<CheckGridNode>> tempGrid(height, std::vector<CheckGridNode>(width));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            tempGrid[i][j] = {grid[i][j], CheckState::Default};
        }
    }
    std::queue<sf::Vector2i> q;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (tempGrid[i][j].val == 0) {
                tempGrid[i][j].state = CheckState::Safe;
                //q.push({i, j});
                // 值为0时，周围格子必为安全格
                ForEachNeighbor(i, j, [&](int nx, int ny){
                    tempGrid[nx][ny].state = CheckState::Safe;
                    q.push({nx, ny});
                    //std::cout<<"1Enqueued: ("<<nx<<", "<<ny<<") with value " <<tempGrid[nx][ny].val<<" and state "<<(int)tempGrid[nx][ny].state<<std::endl;
                });
            }
            else if(tempGrid[i][j].val >= 1 && tempGrid[i][j].val <= 8){
                q.push({i, j});
            }
        }
    }
    while (!q.empty()) {
        sf::Vector2i current = q.front();
        q.pop();
        //std::cout<<"Processing: ("<<current.x<<", "<<current.y<<") with value " <<tempGrid[current.x][current.y].val<<" and state "<<(int)tempGrid[current.x][current.y].state<<std::endl;
        int cnt = 0;
        ForEachNeighbor(current.x, current.y, [&](int nx, int ny) {
            if (tempGrid[nx][ny].val == 0 && tempGrid[nx][ny].state != CheckState::Safe) {
                tempGrid[nx][ny].state = CheckState::Safe;
                //q.push(sf::Vector2i(nx, ny));
                // 值为0时，周围格子必为安全格
                ForEachNeighbor(nx, ny, [&](int nnx, int nny){
                    // 将未知格设为安全格，并将数字格加入队列
                    if(tempGrid[nnx][nny].state == CheckState::Default){
                        tempGrid[nnx][nny].state = CheckState::Safe;
                        q.push({nnx, nny});
                        //std::cout<<"2Enqueued: ("<<nnx<<", "<<nny<<") with value " <<tempGrid[nnx][nny].val<<" and state "<<(int)tempGrid[nnx][nny].state<<std::endl;
                    }
                });
            }
            if(tempGrid[nx][ny].state == CheckState::Default){
                cnt++;
            }
        });
        //std::cout<<"Unknown neighbors count: "<<cnt<<std::endl;
        // 如果周围未知格数量等于当前格数字，说明这些未知格必为雷
        if(cnt == tempGrid[current.x][current.y].val){
            ForEachNeighbor(current.x, current.y, [&](int nx, int ny) {
                // 将未知格设为雷，并将周围数字格的值-1
                if(tempGrid[nx][ny].state == CheckState::Default){
                    tempGrid[nx][ny].state = CheckState::Mine;
                    // 雷周围的数字-1
                    ForEachNeighbor(nx, ny, [&](int nnx, int nny) {
                        if(tempGrid[nnx][nny].val > 0 && tempGrid[nnx][nny].val <= 8){
                            tempGrid[nnx][nny].val -= 1;
                            if(tempGrid[nnx][nny].val > 0){
                                q.push({nnx, nny});
                                //std::cout<<"3Enqueued: ("<<nnx<<", "<<nny<<") with value "<<tempGrid[nnx][nny].val<<" and state " <<(int)tempGrid[nnx][nny].state<<std::endl;
                            }
                            // 如果减到0，说明周围未知格子都为安全格
                            else{
                                tempGrid[nnx][nny].state = CheckState::Safe;
                                ForEachNeighbor(nnx, nny, [&](int nnnx, int nnny){
                                    if(tempGrid[nnnx][nnny].state == CheckState::Default){
                                        tempGrid[nnnx][nnny].state = CheckState::Safe;
                                        q.push({nnnx, nnny});
                                        //std::cout<<"5Enqueued: ("<<nnnx<<", "<<nnny<<") with value "<<tempGrid[nnnx][nnny].val<<" and state " <<(int)tempGrid[nnnx][nnny].state<<std::endl;
                                    }
                                });
                            }
                        }
                    });
                }
            });
        }
    }
    std::cout<<"--------------------------"<<std::endl;
        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){
                std::cout<<(int)tempGrid[i][j].state<<" ";
            }
            std::cout<<std::endl;
        }
}

void Scene::Check1(sf::Vector2i start){
    std::queue<sf::Vector2i> q;
    // 递归展开[i, j]周围的安全格
    std::function<void(int, int)> dfs = [&](int i, int j) {
        if (i < 0 || i >= height || j < 0 || j >= width) return;
        if (tempGrid[i][j].state != CheckState::Default) return;
        // 将当前格设为安全格
        tempGrid[i][j].state = CheckState::Safe;
        // 将数字格加入队列
        if(tempGrid[i][j].val >= 1 && tempGrid[i][j].val <= 8){
            q.push({i, j});
            tempGrid[i][j].state = CheckState::InQueue;
            //std::cout<<"0Enqueued: ("<<i<<", "<<j<<") with value " <<tempGrid[i][j].val<<" and state "<<(int)tempGrid[i][j].state<<std::endl;
            // 会被该数字格影响的数字格也入队
            ForEachNeighborNotMe(i, j, [&](int nx, int ny){
                if(tempGrid[nx][ny].state != CheckState::Default 
                        && tempGrid[nx][ny].state != CheckState::Mine
                        && tempGrid[nx][ny].state != CheckState::InQueue){
                    //std::cout<<"6Enqueued: ("<<nx<<", "<<ny<<") with value " <<tempGrid[nx][ny].val<<" and state "<<(int)tempGrid[nx][ny].state<<std::endl;
                    q.push({nx, ny});
                    tempGrid[nx][ny].state = CheckState::InQueue;
                }
            });
        }
        // 如果当前格为0，说明周围格子必为安全格，继续递归
        if (tempGrid[i][j].val == 0) {
            ForEachNeighborNotMe(i, j, [&](int nx, int ny) {
                 dfs(nx, ny);
            });
        }
    };
    // 从已知数字格快速展开
    std::function<void(int, int)> quickOpen = [&](int x, int y){
        if (tempGrid[x][y].state == CheckState::Default) return;
        if (tempGrid[x][y].val >= 1 && tempGrid[x][y].val <= 8) {
            ForEachNeighborNotMe(x, y, [&](int nx, int ny) {
                if (tempGrid[nx][ny].state == CheckState::Default)
                    dfs(nx, ny);
            });
        }
    };

    // 如果start为雷，将start周围的数字格加入队列，进行推理；否则正常展开
    if(tempGrid[start.x][start.y].state == CheckState::Mine){
        ForEachNeighborNotMe(start.x, start.y, [&](int nnx, int nny){
            if(tempGrid[nnx][nny].state != CheckState::Default 
                    && tempGrid[nnx][nny].state != CheckState::Mine){
                q.push({nnx, nny});
                tempGrid[nnx][nny].state = CheckState::InQueue;
                std::cout<<"MEnqueued: ("<<nnx<<", "<<nny<<") with value "<<tempGrid[nnx][nny].val<<" and state " <<(int)tempGrid[nnx][nny].state<<std::endl;
            }
        });
    }
    else
        dfs(start.x, start.y);

    bool changed = true;
    while(!q.empty()){
        changed = false;
        sf::Vector2i current = q.front(); q.pop();
        //std::cout<<"Processing: ("<<current.x<<", "<<current.y<<") with value " <<tempGrid[current.x][current.y].val<<" and state "<<(int)tempGrid[current.x][current.y].state<<std::endl;
        // 将当前格设为处理中，避免重复入队
        tempGrid[current.x][current.y].state = CheckState::Processing;
        
        // 推出新雷, 如果周围未知格数量+周围雷数等于当前格数字，说明这些未知格必为雷
        if(CCountNeighborUnknown(current.x, current.y) + CCountNeighborMine(current.x, current.y) == tempGrid[current.x][current.y].val){
            changed = true;
            std::cout<<"A"<< current.x <<", "<<current.y<<")"<<std::endl;
            ForEachNeighborNotMe(current.x, current.y, [&](int nx, int ny){
                if(tempGrid[nx][ny].state == CheckState::Default){
                    tempGrid[nx][ny].state = CheckState::Mine;
                    //std::cout<<"Marked Mine: ("<<nx<<", "<<ny<<") with value "<<tempGrid[nx][ny].val<<" and state " <<(int)tempGrid[nx][ny].state<<std::endl;
                    ForEachNeighborNotMe(nx, ny, [&](int nnx, int nny){
                        if(tempGrid[nnx][nny].state != CheckState::Default 
                                && tempGrid[nnx][nny].state != CheckState::Mine){
                            q.push({nnx, nny});
                            tempGrid[nnx][nny].state = CheckState::InQueue;
                            //std::cout<<"4Enqueued: ("<<nnx<<", "<<nny<<") with value "<<tempGrid[nnx][nny].val<<" and state " <<(int)tempGrid[nnx][nny].state<<std::endl;
                        }
                    });
                }
            });
        }
        
        // 推出新安全格, 如果周围雷数等于当前格数字，说明其他未知格必为安全格
        if(CCountNeighborMine(current.x, current.y) == tempGrid[current.x][current.y].val){
            changed = true;
            std::cout<<"B"<<current.x<<", "<<current.y<<")"<<std::endl;
            quickOpen(current.x, current.y);
        }
        
        //if(changed)
            tempGrid[current.x][current.y].state = CheckState::Safe;
    }
    std::cout<<"----*********------------"<<std::endl;
        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){
                std::cout<<(int)tempGrid[i][j].state<<" ";
            }
            std::cout<<std::endl;
        }
}

struct CheckSetNode{
    sf::Vector2i pos; // 数字格位置
    std::vector<uint64_t> bitset; // 使用位集表示未知格子集合
    int mineCountLeft; // 周围剩余的雷数
};

void Scene::Check2(){
    int n = (width * height + 63) / 64; // 位集大小
    std::vector<CheckSetNode> checkSet;
    std::unordered_map<int, std::vector<int>> pToc;
    std::unordered_map<int, std::vector<uint64_t>> pTocBitset;
    std::unordered_map<int, std::vector<int>> cTop;
    
    // 构建集合，将有未知格的数字格加入集合
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if(tempGrid[i][j].state == CheckState::Safe &&
                CCountNeighborUnknown(i, j) > 0){
                //int idx = i * width + j;
                CheckSetNode node;
                node.bitset.resize(n); // 初始化位集大小
                node.pos = {i, j};
                ForEachNeighborNotMe(i, j, [&](int nx, int ny){
                    if(tempGrid[nx][ny].state == CheckState::Default){
                        //int nidx = nx * width + ny;
                        //if(pTocBitset.find(idx) == pTocBitset.end()){
                        //    pTocBitset[idx].resize(n);
                        //}
                        //pToc[idx].push_back(nidx);
                        //pTocBitset[idx][nidx / 64] |= (1ULL << (nidx));
                        //cTop[nidx].push_back(idx);
                        int pos = nx * width + ny;
                        node.bitset[pos / 64] |= (1ULL << (pos % 64));
                    }
                });
                node.mineCountLeft = tempGrid[i][j].val - CCountNeighborMine(i, j);
                checkSet.push_back(node);
            }
        }
    }

    std::cout<<"Count"<<checkSet.size()<<std::endl;
    for(auto& t : checkSet){
        std::cout<<t.pos.x<<" "<<t.pos.y<<std::endl;
    }
    int lasta = 0, lastb = 0;
    int c = 0;
    while(c < 5){
        c++;
        // 遍历集合 检查是否有A属于B的情况，
        int a = -1, b = -1;
        int a_bC = 0; // A\B的格子数
        for(int i = 0; i < checkSet.size(); ++i){
            for(int j = 0; j < checkSet.size(); ++j){
                if(i == j) continue;
                // 如果checkSet[i]的位集属于checkSet[j]的位集，
                // 说明j - (i并j)的格子必为雷或安全格
                bool isSubset = true;
                int diffCount = 0; // A\B的格子数
                for(int k = 0; k < n; ++k){
                    //std::cout<<"a"<<checkSet[i].bitset[k]<<" "<<checkSet[j].bitset[k]<<std::endl;
                    //std::cout<<"b"<<(checkSet[i].bitset[k] & ~checkSet[j].bitset[k])<<std::endl;
                    //std::cout<<"c"<<(checkSet[j].bitset[k] & ~checkSet[i].bitset[k])<<std::endl;
                    diffCount += __builtin_popcountll(checkSet[j].bitset[k] & ~checkSet[i].bitset[k]);
                    // i 不属于 j
                    if((checkSet[i].bitset[k] & ~checkSet[j].bitset[k]) != 0){
                        isSubset = false;
                        break;
                    }
                }
                if(isSubset && diffCount != 0){
                    // 检查是否能推出新信息 若能 退出循环
                    if((checkSet[j].mineCountLeft - checkSet[i].mineCountLeft == diffCount)
                        ||(checkSet[j].mineCountLeft == checkSet[i].mineCountLeft)){
                    if(i != lasta || j != lastb){
                        lasta = i;
                        lastb = j;
                        c = 0;
                    }
                    a = i; b = j; a_bC = diffCount;
                    std::cout<<a<<"("<<checkSet[a].pos.x<<" "<<checkSet[a].pos.y<<")"<<b<<"("<<checkSet[b].pos.x<<" "<<checkSet[b].pos.y<<")"<<a_bC<<std::endl;
                        std::cout<<"exit"<<std::endl;
                        break;
                    }
                }
            }
            if(a != -1 && b != -1) break;
        }
        if(a == -1 || b == -1 || a_bC == 0){
            std::cout<<"STOP"<<std::endl;
            break; // 没有A属于B的情况 结束while循环
        }
        
        bool changed = false;
        std::vector<sf::Vector2i> affectedCells; // ab差集的格子位置
        for(int k = 0; k < n; ++k){
            uint64_t t = checkSet[b].bitset[k] & ~checkSet[a].bitset[k];
            for(int bit = 0; bit < 64; ++bit){
                if(t & (1ULL << bit)){
                    int pos = k * 64 + bit;
                     if(pos >= width * height)
                        continue;
                    affectedCells.push_back({pos / width, pos % width});
                    std::cout<<"chaji"<<pos / width<<" "<<pos % width<<std::endl;
                }
            }
        }
        // 已得到a属于b
        // 如果b的剩余雷数减去a的剩余雷数等于ab差集的格子数量，说明差集的格子必为雷
        if(checkSet[b].mineCountLeft - checkSet[a].mineCountLeft == a_bC){
            changed = true;
            for(const auto& cell :affectedCells){
                if(tempGrid[cell.x][cell.y].state == CheckState::Default){
                    tempGrid[cell.x][cell.y].state = CheckState::Mine;
                }
            }
        }
        // 如果A的剩余雷数等于B的剩余雷数，说明差集的格子必为安全格；
        if(checkSet[a].mineCountLeft == checkSet[b].mineCountLeft){
            changed = true;
            for(const auto& cell :affectedCells){
                if(tempGrid[cell.x][cell.y].state == CheckState::Default){
                    tempGrid[cell.x][cell.y].state = CheckState::Safe;
                }
            }
        }

        if(changed){
            for(const auto& cell : affectedCells){
                std::cout<<"check1 "<<cell.x<<" "<<cell.y<<std::endl;
                Check1(cell);
            }
        }

        // 更新集合
        checkSet.clear();
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if(tempGrid[i][j].state == CheckState::HasUnkown){
                    CheckSetNode node;
                    node.bitset.resize(n); // 初始化位集大小
                    node.pos = {i, j};
                    ForEachNeighborNotMe(i, j, [&](int nx, int ny){
                        if(tempGrid[nx][ny].state == CheckState::Default){
                            int pos = nx * width + ny;
                            node.bitset[pos / 64] |= (1ULL << (pos % 64));
                        }
                    });
                    node.mineCountLeft = tempGrid[i][j].val - CCountNeighborMine(i, j);
                    checkSet.push_back(node);
                }
            }
        }
    }
        
        std::cout<<"--------------------------"<<std::endl;
        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){
                std::cout<<(int)tempGrid[i][j].state<<" ";
            }
            std::cout<<std::endl;
        }
}

void Scene::CSP(sf::Vector2i start){
    
}

void Scene::CalculateNumbers() {
    // 计算数字格
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (grid[i][j] == 9) continue;
            int cnt = 0;
            ForEachNeighbor(i, j, [&](int nx, int ny) {
                if (grid[nx][ny] == 9)
                    ++cnt;
            });
            grid[i][j] = cnt;
        }
    }
}

bool Scene::Reveal(int x, int y) {
    if (flagged[x][y] || revealed[x][y]) return false;
    if (firstClick) {
        PlaceMines(x, y);
        firstClick = false;
    }
    if (grid[x][y] == 9) {
        revealed[x][y] = true;
        grid[x][y] = 10; // 踩雷显示特殊图
        RevealAll();
        return true;
    }
    // 递归展开
    std::function<void(int, int)> dfs = [&](int i, int j) {
        if (i < 0 || i >= height || j < 0 || j >= width) return;
        if (revealed[i][j] || flagged[i][j]) return;
        revealed[i][j] = true;
        ++openCount;
        --cellsLeft;
        if (grid[i][j] == 0) {
            ForEachNeighborNotMe(i, j, [&](int nx, int ny) {
                 dfs(nx, ny);
            });
        }
    };
    dfs(x, y);
    return false;
}

void Scene::RevealAll() {
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            revealed[i][j] = true;
}

void Scene::ToggleFlag(int x, int y) {
    if (revealed[x][y]) return;
    if (flagged[x][y]) {
        flagged[x][y] = false;
        --flagCount;
    } else {
        flagged[x][y] = true;
        ++flagCount;
        questioned[x][y] = false;
    }
}

void Scene::ToggleQuestion(int x, int y) {
    if (revealed[x][y]) return;
    if (questioned[x][y]) {
        questioned[x][y] = false;
        --questionCount;
    } else {
        questioned[x][y] = true;
        ++questionCount;
        flagged[x][y] = false;
    }
}

void Scene::QuickOpen(int x, int y) {
    if (!revealed[x][y]) return;
    int cnt = 0;
    ForEachNeighborNotMe(x, y, [&](int nx, int ny) {
        if (flagged[nx][ny]) ++cnt;
    });
    int cellNum = grid[x][y];
    if (grid[x][y] >= 1 && grid[x][y] <= 8 && cnt == cellNum) {
        ForEachNeighbor(x, y, [&](int nx, int ny) {
            if (!flagged[nx][ny] && !revealed[nx][ny])
                Reveal(nx, ny);
        });
    }
}

bool Scene::IsMine(int x, int y) const {
    return grid[x][y] == 9;
}

bool Scene::IsFlagged(int x, int y) const {
    return flagged[x][y];
}

bool Scene::IsQuestioned(int x, int y) const {
    return questioned[x][y];
}

bool Scene::IsRevealed(int x, int y) const {
    return revealed[x][y];
}

bool Scene::IsInBounds(int x, int y) const {
    return x >= 0 && x < height && y >= 0 && y < width;
}

GridType Scene::GetCell(int x, int y) const {
    if (IsInBounds(x, y)) {
        if (flagged[x][y]) return GridType::FLAG;
        if (questioned[x][y]) return GridType::QUESTION;
        if (!revealed[x][y]) return GridType::DEFAULT;
        switch (grid[x][y])
        {
        case 0:
            return GridType::EMPTY;
        case 1:
            return GridType::ONE;
        case 2:
            return GridType::TWO;
        case 3:
            return GridType::THREE;
        case 4:
            return GridType::FOUR;
        case 5:
            return GridType::FIVE;
        case 6:
            return GridType::SIX;
        case 7:
            return GridType::SEVEN;
        case 8:
            return GridType::EIGHT;
        case 9:
            return GridType::MINE;
        case 10:
            return GridType::BOMB;
        default:
            return GridType::DEFAULT;
        }
    }
    return GridType::DEFAULT;
}
