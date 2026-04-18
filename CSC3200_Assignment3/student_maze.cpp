#include "maze.h"
#include <stack>
#include <vector>
#include <iostream>
#include <functional>

// 深度优先搜索：在到达终点后保持栈中存有完整路径
void search_maze(maze_t const& maze, maze_pos start, maze_pos end,
                 std::stack<maze_pos>& stack, visited_t& visited) {
    int rows = static_cast<int>(maze.size());
    if (rows == 0) return;
    int cols = static_cast<int>(maze[0].size());

    auto is_free = [&](int r, int c) -> bool {
        return maze[r][c] == maze_cell::free ||
               maze[r][c] == maze_cell::end ||
               maze[r][c] == maze_cell::start;
    };

    auto valid = [&](int r, int c) -> bool {
        return r >= 0 && c >= 0 && r < rows && c < cols;
    };

    if (!valid(start.x, start.y) || !is_free(start.x, start.y)) return;
    if (!valid(end.x, end.y)) return;

    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};

    // 使用递归回溯法构建路径
    std::function<bool(maze_pos)> dfs = [&](maze_pos cur) -> bool {
        visited[cur.x][cur.y] = true;
        stack.push(cur);

        if (cur == end) return true; // 找到终点，停止递归

        for (int k = 0; k < 4; ++k) {
            int nx = cur.x + dx[k];
            int ny = cur.y + dy[k];
            if (!valid(nx, ny)) continue;
            if (!visited[nx][ny] && is_free(nx, ny)) {
                if (dfs(maze_pos(nx, ny))) return true; // 终点在此路径下
            }
        }

        // 若此路不通，回溯（弹出当前节点）
        stack.pop();
        return false;
    };

    dfs(start);
}
