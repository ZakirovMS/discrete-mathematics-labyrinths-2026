#include "node.hpp"
#include <iostream>

void Labyrinth::Node::setName(char name)
{
  name_ = name;
}

char Labyrinth::Node::getName()
{
  return name_;
}

void Labyrinth::Node::addRib(char node_to, size_t distance)
{
  connect_[node_to] = distance;
}

const std::map< char, size_t > Labyrinth::Node::getRibs() const
{
  return connect_;
}

void Labyrinth::Node::showRibs()
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

void Labyrinth::Node::setPrevious(Node * previous)
{
  previous_ = previous;
}

Labyrinth::Node * Labyrinth::Node::getPrevious()
{
  return previous_;
}
