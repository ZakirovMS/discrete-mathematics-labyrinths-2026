#include <iostream>
#include "matrix/matrix.hpp"

int main()
{
  Labyrinth::Matrix maze;

  // Генерируем лабиринт размером 10х10 КОМНАТ (итоговое поле будет 21x21)
  maze.generateKruskalMaze(10, 10);

  std::cout << "Сгенерированный лабиринт:\n";
  maze.showMatrix();

  // Запускаем ваш волновой алгоритм для поиска пути
  maze.waveAlgo();

  std::cout << "\nЛабиринт с просчитанными дистанциями:\n";
  maze.showTechInfo();

  return 0;
}