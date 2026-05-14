#include <iostream>
#include <vector>
#include "matrix/matrix.hpp"

int main()
{
  Labyrinth::Matrix gradualOutput;
  gradualOutput.setName("A");
  gradualOutput.generateKruskalMaze(5, 5, '+', {'B'}, true);
  gradualOutput.waveAlgo();
  gradualOutput.writePath('=');

  return 0;
}