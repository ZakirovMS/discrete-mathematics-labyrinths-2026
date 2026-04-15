#include "directedGraph.hpp"
#include <iostream>
#include <list>
#include <limits>
#include <algorithm>

namespace
{
  char nearest(std::map< char, size_t > distances, std::list< char > searched)
  {
    auto i = distances.begin();
    char new_node = i->first;
    size_t dist = std::numeric_limits< size_t >::max();
    for (; i != distances.end(); ++i)
    {
      if ((i->second < dist) && (std::find(searched.begin(), searched.end(), i->first) == searched.end()))
      {
        new_node = i->first;
        dist = i->second;
      }
    }

    return new_node;
  }
}

void zakirov::DirectedGraph::setScopes(char entry, char exit)
{
  scopes_ = std::make_pair(entry, exit);
}

char zakirov::DirectedGraph::getEntry()
{
  return scopes_.first;
}

char zakirov::DirectedGraph::getExit()
{
  return scopes_.second;
}

size_t zakirov::DirectedGraph::getDistance()
{
  std::list< char > searched;
  std::map< char, size_t > distances;
  for (auto i = graph_.begin(); i != graph_.end(); ++i)
  {
    distances[i->first] = std::numeric_limits< size_t >::max();
  }

  distances[getEntry()] = 0;
  char current;
  while (searched.size() != graph_.size())
  {
    current = nearest(distances, searched);
    if (distances[current] == std::numeric_limits< size_t >::max())
    {
      break;
    }

    std::map< char, size_t > curr_ribs = graph_[current].getRibs();
    for (auto i = curr_ribs.begin(); i != curr_ribs.end(); ++i)
    {
      if (distances[current] + i->second < distances[i->first])
      {
        distances[i->first] = distances[current] + i->second;
        graph_[i->first].setPrevious(&graph_[current]);
      }
    }

    searched.push_back(current);
  }

  return distances[getExit()];
}

void zakirov::DirectedGraph::getPath()
{
  std::list< char > sequence;
  Node * current = graph_[getExit()].getPrevious();
  sequence.push_front(getExit());
  while (current != nullptr)
  {
    sequence.push_front(current->getName());
    current = current->getPrevious();
  }

  auto i = sequence.begin();
  std::cout << *i;
  ++i;
  for (; i != sequence.end(); ++i)
  {
    std::cout << ' ' << *i;
  }

  std::cout << '\n';
}

void zakirov::DirectedGraph::addNode(char name)
{
  graph_[name].setName(name);
}

void zakirov::DirectedGraph::connectNodes(char node_from, char node_to, size_t distance)
{
  graph_[node_from].addRib(node_to, distance);
}

void zakirov::DirectedGraph::showGraph()
{
  for (auto i = graph_.begin(); i != graph_.end(); ++i)
  {
    std::cout << i->second.getName() << " -> ";
    i->second.showRibs();
    std::cout << '\n';
  }
}
