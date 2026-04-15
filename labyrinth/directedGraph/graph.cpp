#include <iostream>
#include <list>
#include <map>
#include <set>
#include <limits>
#include <algorithm>
char nearest(std::map< char, size_t > distances, std::list< char > searched)
{
  auto i = distances.begin();
  char new_node = i->first;
  size_t dist = i->second;
  for (; i != distances.end(); ++i)
  {
    if (i->second < dist && std::find(searched.begin(), searched.end(), new_node) == searched.end())
    {
        char new_node = i->first;
        size_t dist = i->second;
    }
  }

  return new_node;
}

void Node::setName(char name)
{
  name_ = name;
}

char Node::getName()
{
  return name_;
}

void Node::addRib(char node_to, size_t distance)
{
  connect_[node_to] = distance;
}

void Node::showRibs()
{
  if (connect_.empty())
  {
    std::cout << "isolated node";
    return;
  }

  auto i = connect_.begin();
  std::cout << '[' << i->first << "; " << i->second << ']';
  ++i;
  for (; i != connect_.end(); ++i)
  {
    std::cout << " [" << i->first << "; " << i->second << ']';
  }
}

void DirectedGraph::setHead(char head)
{
  head_ = head;
}

char DirectedGraph::getHead()
{
  return head_;
}

size_t DirectedGraph::getPath()
{
  std::list< char > searched;
  std::map< char, size_t > distances;
  distances[head_] = 0;
  for (auto i = graph_.begin(); i != graph_.end(); ++i)
  {
    distances[i->first] = std::numeric_limits< size_t >::max();
  }

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
        graph_[i->first].addPrevious(current);
      }
    }

    searched.push_back(current);
  }
}

void DirectedGraph::addNode(char name)
{
  graph_[name].setName(name);
}

void DirectedGraph::connectNodes(char node_from, char node_to, size_t distance)
{
  graph_[node_from].addRib(node_to, distance);
}

void DirectedGraph::showGraph()
{
  for (auto i = graph_.begin(); i != graph_.end(); ++i)
  {
    std::cout << i->second.getName() << " -> ";
    i->second.showRibs();
    std::cout << '\n';
  }
}
