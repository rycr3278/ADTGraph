#include "Node.h"

Node::Node(string s) {
  // DONE FOR YOU
  data = s;
}

Node::~Node() {
  // "DONE" FOR YOU
  //
  // This is equivalent to the default destructor the compiler will
  // generate for you. As a rule of thumb, you only need to `delete`
  // member variables that were created with `new` inside this
  // class. So, you don't need to do anything here. It could have been
  // left out of the header/impl file entirely.
}

string Node::getData() {
  // DONE FOR YOU
  return data;
}

void Node::setRank(int r) {
  // DONE FOR YOU
  rank = r;
}

void Node::clear() {
  color = WHITE;
  discovery_time = -1;
  completion_time = -1;
  rank = -1;
  predecessor = nullptr;
}

void Node::setColor(int search_color, int time) {
  color = search_color;
  if (search_color == GRAY) {
    discovery_time = time;
  } else if (search_color == BLACK) {
    completion_time = time;
  } else if (search_color == WHITE) {
    discovery_time = -1; // Reset discovery time to -1 when color is set to WHITE
    completion_time = -1;
  }
}

void Node::getDiscoveryInformation(int& thecolor, int& disco_time, int& finish_time, int& bfs_rank) {
  thecolor = color;
  disco_time = discovery_time;
  finish_time = completion_time;
  bfs_rank = rank;
}

bool Node::isAncestor(shared_ptr<Node> other) {
  shared_ptr<Node> current = predecessor; // Start from the predecessor
  while (current != nullptr) {
    if (current == other) {
      return true; // Found the "other" node in the predecessor list
    }
    current = current->predecessor;
  }
  return false; // "other" node not found in the predecessor list
}

void Node::setPredecessor(shared_ptr<Node> other) {
  predecessor = other;
}

int Node::getColor() const {
  return color;
}

int Node::getRank() const {
  return rank;
}

// Overloading operator << lets you put a Node object into an output stream.
ostream &operator<<(std::ostream &out, Node node) {
  out << node.data;
  return out;
}