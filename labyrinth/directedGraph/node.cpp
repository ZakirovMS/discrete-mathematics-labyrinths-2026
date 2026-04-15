#include "node.hpp"
#include <iostream>

void zakirov::Node::setName(char name)
{
  name_ = name;
}

char zakirov::Node::getName()
{
  return name_;
}

void zakirov::Node::addRib(char node_to, size_t distance)
{
  connect_[node_to] = distance;
}

const std::map< char, size_t > zakirov::Node::getRibs() const
{
  return connect_;
}

void zakirov::Node::showRibs()
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

void zakirov::Node::setPrevious(Node * previous)
{
  previous_ = previous;
}

zakirov::Node * zakirov::Node::getPrevious()
{
  return previous_;
}
