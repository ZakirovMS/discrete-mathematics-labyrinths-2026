#include "MazeNode.hpp"
#include <iostream>
#include <queue>
#include <algorithm>
#include <random>

MazeNode::MazeNode(std::string name, int w, int h):
  layerName(name),
  width(w),
  height(h),
  weight(0)
{
  grid.assign(height, std::vector< char >(width, WALL_CHAR));
  generatePrims();
}

void MazeNode::generatePrims()
{
  grid.assign(height, std::vector< char >(width, WALL_CHAR));
  std::mt19937 rng(std::random_device{}());
    
  struct Point
  {
    int x, y, px, py;
  };
  std::vector< Point > walls;

  int startX = 0, startY = 0;
  grid[startY][startX] = PATH_CHAR;

  auto addNeighbors = [&](int x, int y)
  {
    int dx[] = {0, 0, 2, -2};
    int dy[] = {2, -2, 0, 0};
      for (int i = 0; i < 4; ++i)
      {
      int nx = x + dx[i], ny = y + dy[i];
      if (nx  >= 0 && nx < width && ny  >= 0 && ny < height && grid[ny][nx] == WALL_CHAR)
      {
        walls.push_back({nx, ny, x, y});
      }
    }
  };

  addNeighbors(startX, startY);

  while (!walls.empty())
  {
    int idx = std::uniform_int_distribution< int >(0, walls.size() - 1)(rng);
    Point w = walls[idx];
    walls.erase(walls.begin() + idx);

    if (grid[w.y][w.x] == WALL_CHAR)
    {
      grid[w.y][w.x] = PATH_CHAR;
      grid[(w.y + w.py) / 2][(w.x + w.px) / 2] = PATH_CHAR;
      addNeighbors(w.x, w.y);
    }
  }

  std::vector< std::pair< int, int > > pathCells;
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      if (grid[y][x] == PATH_CHAR)
      {
        pathCells.push_back({x, y});
      }
    }
  }

  if (!pathCells.empty())
  {
    int rIdx = std::uniform_int_distribution< int >(0, pathCells.size() - 1)(rng);
    auto [rx, ry] = pathCells[rIdx];
    grid[ry][rx] = START_CHAR;
  }
}

void MazeNode::addTransition(int x, int y, char targetLayer)
{
  if (y  >= 0 && y < height && x  >= 0 && x < width)
  {
    grid[y][x] = targetLayer;
  }
}

void MazeNode::print()
{
  std::cout << layerName << " " << width << " " << height << "\n";
  for (const auto& row : grid)
  {
    for (char cell : row) std::cout << cell;
    std::cout << "\n";
  }
}