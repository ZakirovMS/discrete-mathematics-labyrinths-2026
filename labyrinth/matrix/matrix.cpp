#include "matrix.hpp"
#include <limits>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <random>
#include <numeric>

namespace {
 class DSU {
  private:
    std::vector<size_t> parent;
    std::vector<size_t> rank;
  public:
    DSU(size_t n) {
      parent.resize(n);
      rank.resize(n, 0);
      std::iota(parent.begin(), parent.end(), 0);
    }
    size_t find(size_t i) {
      if (parent[i] == i) return i;
      return parent[i] = find(parent[i]);
    }
    void unite(size_t i, size_t j) {
      size_t root_i = find(i);
      size_t root_j = find(j);
      if (root_i != root_j) {
        if (rank[root_i] < rank[root_j]) {
          parent[root_i] = root_j;
        } else if (rank[root_i] > rank[root_j]) {
          parent[root_j] = root_i;
        } else {
          parent[root_j] = root_i;
          rank[root_i]++;
        }
      }
    }
 };

 struct Edge {
    size_t r1, c1; // ID комнаты 1
    size_t r2, c2; // ID комнаты 2
    size_t wr, wc; // Координаты стены в матрице
  };
};

void Labyrinth::Matrix::clearMetadata()
{
  for (auto i = ties_.begin(); i != ties_.end(); ++i)
  {
    i->second = std::numeric_limits< size_t >::max();
  }

  for (size_t i = 0; i < scopes_.first; ++i)
  {
    for (size_t j = 0; j < scopes_.second; ++j)
    {
      field_(i, j).setDist(std::numeric_limits< size_t >::max());
    }
  }
}

//Get rid of duplication
bool Labyrinth::Matrix::expandWave(std::vector< std::pair < size_t, size_t > > & from, std::vector< std::pair < size_t, size_t > > & to)
{
  size_t addedCells = 0;
  bool returnable = false;

  std::vector< std::pair < size_t, size_t > > stub;

  for (size_t i = 0; i < from.size(); ++i)
  {
    size_t row = from[i].first;
    size_t col = from[i].second;

    if (field_(row - 1, col).isPassable() && !field_(row - 1, col).isVisited())
    {
      stub.push_back(std::make_pair(row - 1, col));
      field_(row - 1, col).visitCell();
      ++addedCells;
    }
    if (field_(row + 1, col).isPassable() && !field_(row + 1, col).isVisited())
    {
      stub.push_back(std::make_pair(row + 1, col));
      field_(row + 1, col).visitCell();
      ++addedCells;
    }
    if (field_(row, col - 1).isPassable() && !field_(row, col - 1).isVisited())
    {
      stub.push_back(std::make_pair(row, col - 1));
      field_(row, col - 1).visitCell();
      ++addedCells;
    }
    if (field_(row, col + 1).isPassable() && !field_(row, col + 1).isVisited())
    {
      stub.push_back(std::make_pair(row, col + 1));
      field_(row, col + 1).visitCell();
      ++addedCells;
    }


    if (!addedCells)
    {
      stub.push_back(std::make_pair(from[i].first, from[i].second));
    }
    else
    {
      addedCells = 0;
      returnable = true;
    }
  }

  //tech remove
  for (size_t i = 0; i < stub.size(); ++i)
  {
    std::cout << '(' << stub[i].first << "; " << stub[i].second << ") ";
  }
  std::cout << '\n';

  std::swap(stub, to);
  return returnable;
}

void Labyrinth::Matrix::setScopes(std::pair< size_t, size_t > scope)
{
  scopes_ = scope;
}

std::pair< size_t, size_t > Labyrinth::Matrix::getScopes()
{
  return scopes_;
}

void Labyrinth::Matrix::setEntry(std::pair< size_t, size_t > entry)
{
  entry_ = entry;
}

std::pair< size_t, size_t > Labyrinth::Matrix::getEntry()
{
  return entry_;
}

void Labyrinth::Matrix::setConnect(char connect)
{
  ties_.push_back(std::make_pair(connect, std::numeric_limits< size_t >::max()));
}

std::list< std::pair < char, size_t > > & Labyrinth::Matrix::getConnect()
{
  return ties_;
}

void Labyrinth::Matrix::setField(ArrayWrapper< CaveComponent > & field)
{
  field_ = field;
}

Labyrinth::ArrayWrapper< Labyrinth::CaveComponent > & Labyrinth::Matrix::getField()
{
  return field_;
}

void Labyrinth::Matrix::showMatrix()
{
  std::cout << scopes_.first << ' ' << scopes_.second << '\n' << "connects:";
  for (auto i = ties_.begin(); i != ties_.end(); ++i)
  {
    std::cout << ' ' << i->first;
  }

  std::cout << '\n';
  if (scopes_.first == 0 || scopes_.second == 0)
  {
    return;
  }

  for (size_t i = 0; i < scopes_.first - 1; ++i)
  {
    std::cout << field_(i, 0).getCell();
    for (size_t j = 1; j < scopes_.second; ++j)
    {
      std::cout << ' ' << field_(i, j).getCell();
    }
    std::cout << '\n';
  }

  std::cout << field_(scopes_.first - 1, 0).getCell();
  for (size_t j = 1; j < scopes_.second; ++j)
  {
    std::cout << ' ' << field_(scopes_.first - 1, j).getCell();
  }

  std::cout << '\n';
}

std::istream & Labyrinth::operator>>(std::istream & in, Labyrinth::Matrix & layer)
{
  std::pair< size_t, size_t > scope;
  in >> scope.first >> scope.second;
  layer.setScopes(scope);
  ArrayWrapper< CaveComponent > field(scope.first + 1, scope.second + 1);
  char el;
  for (size_t i = 0; i < scope.first; ++i)
  {
    field(i, 0).setCell('#');
  }

  for (size_t j = 0; j < scope.second; ++j)
  {
    field(0, j).setCell('#');
  }

  for (size_t i = 1; i < scope.first - 1; ++i)
  {
    for (size_t j = 1; j < scope.second - 1; ++j)
    {
      in >> el;
      if (el == '+')
      {
        layer.setEntry(std::make_pair(i, j));
      }
      else if ((el >= 'A' && el <= 'Z') || (el >= 'a' && el <= 'z') || (el == '='))
      {
        layer.setConnect(el);
      }
      else
      {
        throw std::logic_error("ERROR: Invalid input");
      }

      field(i, j).setCell(el);
    }
  }

  layer.setField(field);
  return in;
}

void Labyrinth::Matrix::waveAlgo()
{
  size_t counter = 0;
  field_(entry_.first, entry_.second).setDist(counter);
  std::vector< std::pair < size_t, size_t > > wave, old_wave;
  old_wave.push_back(entry_);
  while (expandWave(old_wave, wave))
  {
    counter += 1;
    for (size_t i = 0; i < wave.size(); ++i)
    {
      if (field_(wave[i].first, wave[i].second).getDist() == std::numeric_limits< size_t >::max())
      {
        field_(wave[i].first, wave[i].second).setDist(counter);
      }

      if (field_(wave[i].first, wave[i].second).isExit())
      {
        auto it = std::find(ties_.begin(), ties_.end(), std::make_pair(field_(wave[i].first, wave[i].second).getCell(), std::numeric_limits< size_t >::max()));
        if (it != ties_.end())
        {
          it->second = counter;
        }
      }
    }

    std::swap(wave, old_wave);
    wave.clear();
  }
}

void Labyrinth::Matrix::showTechInfo()
{
  for (size_t i = 0; i < scopes_.first; ++i)
  {
    for (size_t j = 0; j < scopes_.second; ++j)
    {
      if (field_(i, j).getDist() == std::numeric_limits< size_t >::max())
      {
        std::cout << "# ";
      }
      else if (field_(i, j).getDist() < 10)
      {
        std::cout << field_(i, j).getDist() << ' ';
      }
      else
      {
        std::cout << field_(i, j).getDist();
      }
      std::cout << ' ';
    }

    std::cout << '\n';
  }
}

void Labyrinth::Matrix::generateKruskalMaze(size_t rooms_rows, size_t rooms_cols)
{
  size_t grid_rows = rooms_rows * 2 + 1;
  size_t grid_cols = rooms_cols * 2 + 1;

  // 1. Инициализируем размеры и выделяем память под поле
  scopes_ = std::make_pair(grid_rows, grid_cols);
  field_ = ArrayWrapper< CaveComponent >(grid_rows, grid_cols);
  ties_.clear(); // Очищаем старые связи, если матрица переиспользуется

  // 2. Заполняем все поле стенами
  for (size_t i = 0; i < grid_rows; ++i)
  {
    for (size_t j = 0; j < grid_cols; ++j)
    {
      field_(i, j).setCell('#');
      field_(i, j).setDist(std::numeric_limits< size_t >::max());
    }
  }

  // 3. Вырезаем изолированные комнаты (клетки с нечетными индексами)
  for (size_t r = 0; r < rooms_rows; ++r)
  {
    for (size_t c = 0; c < rooms_cols; ++c)
    {
      field_(r * 2 + 1, c * 2 + 1).setCell('.');
    }
  }

  // 4. Собираем список всех внутренних стен
  std::vector< Edge > edges;
  for (size_t r = 0; r < rooms_rows; ++r)
  {
    for (size_t c = 0; c < rooms_cols; ++c)
    {
      if (c < rooms_cols - 1) // Вертикальная стена справа
      {
        edges.push_back({r, c, r, c + 1, r * 2 + 1, c * 2 + 2});
      }
      if (r < rooms_rows - 1) // Горизонтальная стена снизу
      {
        edges.push_back({r, c, r + 1, c, r * 2 + 2, c * 2 + 1});
      }
    }
  }

  // 5. Перемешиваем стены для рандомизации
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(edges.begin(), edges.end(), g);

  // 6. Применяем DSU (Алгоритм Крускала)
  DSU dsu(rooms_rows * rooms_cols);
  auto get_id = [&](size_t r, size_t c) { return r * rooms_cols + c; };

  for (size_t i = 0; i < edges.size(); ++i)
  {
    size_t id1 = get_id(edges[i].r1, edges[i].c1);
    size_t id2 = get_id(edges[i].r2, edges[i].c2);

    if (dsu.find(id1) != dsu.find(id2))
    {
      dsu.unite(id1, id2);
      field_(edges[i].wr, edges[i].wc).setCell('.'); // Ломаем стену
    }
  }

  // 7. Устанавливаем вход (+) и выход (=)
  entry_ = std::make_pair(0, 1);
  field_(0, 1).setCell('+');

  std::pair<size_t, size_t> exit_pos = std::make_pair(grid_rows - 1, grid_cols - 2);
  field_(exit_pos.first, exit_pos.second).setCell('=');

  // Добавляем выход в список ties_ (как это делает ваш оператор >>)
  setConnect('=');
}