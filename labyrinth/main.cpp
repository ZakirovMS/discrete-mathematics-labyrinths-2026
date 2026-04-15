#include <iostream>
#include <vector>
#include "matrix/matrix.hpp"

int main()
{
  std::cout << "Cave1\n";

  Labyrinth::Matrix layerZ;
  layerZ.setName("Z");
  layerZ.generateKruskalMaze(13, 13, '+', {'V'});
  layerZ.waveAlgo();
  layerZ.writePath('V');

  Labyrinth::Matrix layerV;
  layerV.setName("V");
  layerV.generateKruskalMaze(17, 17, '+', {'='});
  layerV.waveAlgo();
  layerV.writePath('=');

  std::cout << layerZ.getName() << " " << layerZ.getScopes().second << " " << layerZ.getScopes().first << "\n";
  layerZ.showMatrix();
  std::cout << "\n";

  std::cout << layerV.getName() << " " << layerV.getScopes().second << " " << layerV.getScopes().first << "\n";
  layerV.showMatrix();
  std::cout << "\n";

  return 0;
}