#ifndef DIRECTEDGRAPH_HPP
#define DIRECTEDGRAPH_HPP
#include <cstdlib>
#include <map>
#include "node.hpp"

namespace zakirov
{
  class DirectedGraph
  {
  public:
    void setScopes(char entry, char exit);
    char getEntry();
    char getExit();
    void addNode(char name);
    void connectNodes(char node_from, char node_to, size_t distance);
    void showGraph();
    size_t getDistance();
    void getPath();
  private:
    std::pair< char, char > scopes_;
    std::map< char, Node > graph_;
  };
}

#endif
