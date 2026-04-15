#include <iostream>
#include <vector>
#include "matrix/matrix.hpp"

int main()
{
  std::cout << "Cave1\n";

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

  return 0;
}