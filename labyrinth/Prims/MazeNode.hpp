#ifndef MAZENODE_HPP
#define MAZENODE_HPP

#include <vector>
#include <string>

constexpr char WALL_CHAR = '#';
constexpr char PATH_CHAR = '.';
constexpr char START_CHAR = '+';
constexpr char EXIT_CHAR = '=';

class MazeNode
{
public:
  std::string layerName;
  int width, height;
  std::vector<std::vector<char>> grid;
  int weight;

  MazeNode(std::string name, int w, int h);
  void print(); 
  void addTransition(int x, int y, char targetLayer);

private:
  void generatePrims();
  void calculateWeight();
};

#endif