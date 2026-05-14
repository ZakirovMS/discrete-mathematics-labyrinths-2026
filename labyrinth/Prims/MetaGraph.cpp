#include <random>
#include "MetaGraph.hpp"

MetaGraph::Node::Node(std::string name, int w, int h):
  layerName(name)
{
  maze = new MazeNode(name, w, h);
  traversalCost = maze->weight;
}

MetaGraph::Node::~Node()
{
  delete maze;
}

void MetaGraph::addNode(std::string name, int w, int h)
{
  if (nodes.find(name) == nodes.end())
  {
    nodes[name] = new Node(name, w, h);
  }
}

void MetaGraph::printNode(std::string name)
{
  if (nodes.count(name))
  {
    nodes[name]->maze->print();
  }
  else
  {
    std::cout << "Layer " << name << " not found!\n";
  }
}

void MetaGraph::addEdge(std::string from, std::string to, int xFrom, int yFrom) {
  if (nodes.count(from) && nodes.count(to))
  {
    MazeNode* m = nodes[from]->maze;

    if (m->grid[yFrom][xFrom] == '+')
    {
      std::vector< std::pair< int, int > > emptyCells;
      for (int y = 0; y < m->height; ++y)
      {
        for (int x = 0; x < m->width; ++x)
        {
          if (m->grid[y][x] == '.' && (x != xFrom || y != yFrom))
          {
            emptyCells.push_back({x, y});
          }
        }
      }

      if (!emptyCells.empty())
      {
        std::mt19937 rng(std::random_device{}());
        int rIdx = std::uniform_int_distribution< int >(0, emptyCells.size() - 1)(rng);
        m->grid[emptyCells[rIdx].second][emptyCells[rIdx].first] = '+';
      }
    }

    m->grid[yFrom][xFrom] = to[0];

    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};
    for (int i = 0; i < 4; ++i)
    {
      int nx = xFrom + dx[i];
      int ny = yFrom + dy[i];
      if (nx >= 0 && nx < m->width && ny >= 0 && ny < m->height)
      {
        if (m->grid[ny][nx] == '#')
        { 
          m->grid[ny][nx] = '.'; 
        }
      }
    }
  }
}

void MetaGraph::printAsCave(std::string caveName, const std::vector<std::string>& order)
{
  std::cout << caveName << "\n";

  for (size_t i = 0; i < order.size(); ++i)
  {
    std::string name = order[i];
    if (nodes.find(name) == nodes.end()) continue;

    Node* nodePtr = nodes[name];
    if (i == order.size() - 1)
    {
      nodePtr->maze->grid[0][0] = '=';
    }
    else
    {
      if (nodePtr->maze->grid[0][0] == '=')
      {
        nodePtr->maze->grid[0][0] = PATH_CHAR;
      }
    }
    nodePtr->maze->print();
    std::cout << "\n";
  }
}

MetaGraph::~MetaGraph()
{
  for (auto& pair : nodes)
  {
    delete pair.second;
  }
}