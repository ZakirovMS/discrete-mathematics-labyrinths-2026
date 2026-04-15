#include "matrix.hpp"
#include <limits>
#include <algorithm>
#include <stdexcept>
#include <functional>

namespace
{

}

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
