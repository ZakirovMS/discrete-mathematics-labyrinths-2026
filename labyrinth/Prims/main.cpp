#include <iostream>
#include "MetaGraph.hpp"

int main() {
  MetaGraph myWorld;

  myWorld.addNode("A", 5, 5);
  myWorld.addNode("B", 5, 5);
  myWorld.addNode("C", 5, 5);
  myWorld.addNode("D", 5, 5);
  myWorld.addNode("E", 5, 5);
  myWorld.addNode("G", 5, 5);
  myWorld.addNode("Z", 5, 5);
  myWorld.addNode("V", 5, 5);

  myWorld.addEdge("A", "B", 1, 2);
  myWorld.addEdge("A", "C", 3, 0);
  myWorld.addEdge("A", "V", 2, 3);
  myWorld.addEdge("B", "C", 2, 3); 
  myWorld.addEdge("B", "D", 0, 4);
  myWorld.addEdge("D", "E", 4, 4);
  myWorld.addEdge("C", "G", 1, 1);
  myWorld.addEdge("C", "V", 2, 1);
  myWorld.addEdge("V", "G", 2, 3);
  myWorld.addEdge("V", "E", 1, 4);
  myWorld.addEdge("V", "B", 2, 3);
  myWorld.addEdge("E", "C", 0, 2);
  myWorld.addEdge("G", "Z", 2, 2);
  myWorld.addEdge("E", "Z", 4, 4);

  std::vector<std::string> path = {"A", "B", "C", "D", "E", "G", "V", "Z"};
  myWorld.printAsCave("CoolLabyrinth", path);

  return 0;
}