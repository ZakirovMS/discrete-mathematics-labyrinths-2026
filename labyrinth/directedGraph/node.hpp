#ifndef NODE_HPP
#define NODE_HPP
#include <cstdlib>
#include <map>

namespace zakirov
{
  class Node
  {
  public:
    void setName(char name);
    char getName();
    void addRib(char node_to, size_t distance);
    void setPrevious(Node * previous);
    Node * getPrevious();
    const std::map< char, size_t > getRibs() const;
    void showRibs();
  private:
    char name_;
    Node * previous_;
    std::map< char, size_t > connect_;
  };
}

#endif
