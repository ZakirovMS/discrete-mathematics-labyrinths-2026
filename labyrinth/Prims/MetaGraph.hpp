#ifndef METAGRAPH_HPP
#define METAGRAPH_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "MazeNode.hpp"

class MetaGraph
{
public:
  struct Node
  {
    std::string layerName;
    MazeNode* maze;
    int traversalCost;

    Node(std::string name, int w, int h);
    ~Node();
  };

  void addNode(std::string name, int w, int h);
  void addEdge(std::string from, std::string to, int xFrom, int yFrom);
  void printNode(std::string name);
  void printAsCave(std::string caveName, const std::vector< std::string >& order);    
  ~MetaGraph();

private:
  std::map< std::string, Node* > nodes;
};

#endif