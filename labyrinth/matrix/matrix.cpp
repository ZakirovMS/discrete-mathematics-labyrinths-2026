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

bool Labyrinth::Matrix::expandWave(std::vector<std::pair<size_t, size_t>>& from, std::vector<std::pair<size_t, size_t>>& to)
{
  bool added = false;
  to.clear();

  for (const auto& curr : from) {
    size_t r = curr.first;
    size_t c = curr.second;

    // Смещения: вверх, вниз, влево, вправо
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i) {
      size_t nr = r + dr[i];
      size_t nc = c + dc[i];

      // Проверка границ и проходимости (не стена)
      if (nr < scopes_.first && nc < scopes_.second) {
        if (field_(nr, nc).isPassable() && !field_(nr, nc).isVisited()) {
          field_(nr, nc).visitCell(); // Помечаем как посещенную
          to.push_back({nr, nc});
          added = true;
        }
      }
    }
  }
  return added;
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

      if (field_(wave[i].first, wave[i].second).isExit()) {
          char exitChar = field_(wave[i].first, wave[i].second).getCell();
          // update **every** still‑unfilled entry that has this character
          for (auto it = ties_.begin(); it != ties_.end(); ++it) {
              if (it->first == exitChar && it->second == std::numeric_limits< size_t >::max()) {
                  it->second = counter;            // distance of this exit instance
              }
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

// Генерация БЕЗ окантовки стенами
void Labyrinth::Matrix::generateKruskalMaze(size_t grid_rows, size_t grid_cols, char entry_sym, std::vector<char> exits_sym)
{
  scopes_ = std::make_pair(grid_rows, grid_cols);
  field_ = ArrayWrapper< CaveComponent >(grid_rows, grid_cols);
  ties_.clear();

  // Инициализация (сброс всего)
  for (size_t i = 0; i < grid_rows; ++i) {
    for (size_t j = 0; j < grid_cols; ++j) {
      field_(i, j).setCell('#');
      field_(i, j).setDist(std::numeric_limits<size_t>::max());
      // ВАЖНО: нужно обнулять visit_ в объекте CaveComponent, если там нет метода,
      // убедитесь, что при создании ArrayWrapper объекты создаются с visit_ = false
    }
  }

  size_t rooms_rows = (grid_rows + 1) / 2;
  size_t rooms_cols = (grid_cols + 1) / 2;

  // Сетка комнат (0, 2, 4...)
  for (size_t r = 0; r < rooms_rows; ++r)
    for (size_t c = 0; c < rooms_cols; ++c)
      field_(r * 2, c * 2).setCell('.');

  std::vector< Edge > edges;
  for (size_t r = 0; r < rooms_rows; ++r) {
    for (size_t c = 0; c < rooms_cols; ++c) {
      if (c < rooms_cols - 1 && c * 2 + 1 < grid_cols) edges.push_back({r, c, r, c + 1, r * 2, c * 2 + 1});
      if (r < rooms_rows - 1 && r * 2 + 1 < grid_rows) edges.push_back({r, c, r + 1, c, r * 2 + 1, c * 2});
    }
  }

  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(edges.begin(), edges.end(), g);

  DSU dsu(rooms_rows * rooms_cols);
  auto get_id = [&](size_t r, size_t c) { return r * rooms_cols + c; };

  for (const auto& e : edges) {
    if (dsu.find(get_id(e.r1, e.c1)) != dsu.find(get_id(e.r2, e.c2))) {
      dsu.unite(get_id(e.r1, e.c1), get_id(e.r2, e.c2));
      field_(e.wr, e.wc).setCell('.');
    }
  }

  // Расстановка точек входа/выхода
  std::vector<std::pair<size_t, size_t>> pts;
  for (size_t i = 0; i < grid_rows; ++i) {
    for (size_t j = 0; j < grid_cols; ++j) {
      if (field_(i, j).isPath()) {
        pts.push_back({i, j});
      }
    }
  }

  std::shuffle(pts.begin(), pts.end(), g);

  if (!pts.empty()) {
    entry_ = pts.back();
    field_(entry_.first, entry_.second).setCell(entry_sym);
    pts.pop_back();
  }

  if (pts.size() < exits_sym.size()) {
    throw std::runtime_error("generateKruskalMaze: not enough walkable cells for the requested exits");
  }


  for (char ex : exits_sym) {
    if (!pts.empty()) {
      field_(pts.back().first, pts.back().second).setCell(ex);
      setConnect(ex);
      pts.pop_back();
    }
  }
}

void Labyrinth::Matrix::writePath(char to) {
    // ---------- 1. locate the exit cell with minimal distance ----------
    std::pair<size_t, size_t> curr = {0, 0};
    size_t d = std::numeric_limits<size_t>::max();
    bool found = false;

    for (size_t i = 0; i < scopes_.first; ++i) {
        for (size_t j = 0; j < scopes_.second; ++j) {
            if (field_(i, j).getCell() == to) {
                size_t cellDist = field_(i, j).getDist();
                if (!found || cellDist < d) {
                    curr = {i, j};
                    d = cellDist;
                    found = true;
                }
            }
        }
    }

    if (!found || d == std::numeric_limits<size_t>::max())
        return;                 // nothing to do

    // ---------- 2. walk back to distance 1 (the entry) ----------
    while (d > 1) {
        --d;
        size_t r = curr.first;
        size_t c = curr.second;
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        for (int k = 0; k < 4; ++k) {
            size_t nr = r + dr[k];
            size_t nc = c + dc[k];
            if (nr < scopes_.first && nc < scopes_.second) {
                if (field_(nr, nc).getDist() == d && field_(nr, nc).isPassable()) {
                    curr = {nr, nc};
                    if (field_(nr, nc).isPath())          // only ordinary corridor
                        field_(nr, nc).setCell('*');
                    break;                                 // next step
                }
            }
        }
    }
}
