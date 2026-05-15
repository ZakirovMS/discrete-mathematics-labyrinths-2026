#include <iostream>
#include <vector>
#include "matrix/matrix.hpp"
#include "directedGraph/directedGraph.hpp"

int main()
{
  Labyrinth::Matrix layerA;
  layerA.setName("A");
  layerA.generateKruskalMaze(5, 5, '+', {'B'});
  layerA.waveAlgo();
  layerA.writePath('B');

  Labyrinth::Matrix layerB;
  layerB.setName("B");
  layerB.generateKruskalMaze(7, 7, '+', {'C'});
  layerB.waveAlgo();
  layerB.writePath('C');

  Labyrinth::Matrix layerC;
  layerC.setName("C");
  layerC.generateKruskalMaze(9, 13, '+', {'D', 'D', 'D'});
  layerC.waveAlgo();
  layerC.writePath('D');

  Labyrinth::Matrix layerD;
  layerD.setName("D");
  layerD.generateKruskalMaze(13, 13, '+', {'E'});
  layerD.waveAlgo();
  layerD.writePath('E');

  Labyrinth::Matrix layerE;
  layerE.setName("E");
  layerE.generateKruskalMaze(17, 17, '+', {'='});
  layerE.waveAlgo();
  layerE.writePath('=');

  Labyrinth::Matrix matrixes[] = {layerA, layerB, layerC, layerD, layerE};

  for (Labyrinth::Matrix matrix : matrixes) {
    std::cout << matrix.getName() << " " << matrix.getScopes().second << " " << matrix.getScopes().first << "\n";
    matrix.showMatrix();
    std::cout << "\n";
  }

  Labyrinth::DirectedGraph journey;
  journey.setScopes('A', '=');  // глобальный вход и финальный выход

  journey.addNode('A');
  journey.addNode('B');
  journey.addNode('C');
  journey.addNode('D');
  journey.addNode('E');
  journey.addNode('=');

  for (auto& tie : layerA.getConnect()) {
      journey.connectNodes('A', tie.first, tie.second);
  }
  for (auto& tie : layerB.getConnect()) {
      journey.connectNodes('B', tie.first, tie.second);
  }
  for (auto& tie : layerC.getConnect()) {
      journey.connectNodes('C', tie.first, tie.second);
  }
  for (auto& tie : layerD.getConnect()) {
      journey.connectNodes('D', tie.first, tie.second);
  }
  for (auto& tie : layerE.getConnect()) {
      journey.connectNodes('E', tie.first, tie.second);
  }


  std::cout << "Суммарная длина: ";
  std::cout << journey.getDistance() << "\n";

  std::cout << "Кратчайший путь через все уровни: ";
  journey.getPath();

  return 0;
}