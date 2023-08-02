#include "Graph.h"
#include <stack>
#include <iostream>
#include <sstream>

using namespace std;

// fwd declarations
string make_dot(Graph* g);
string what(int& v);

Graph::Graph() {
  // DONE FOR YOU
  directed = false; // graphs are undirected by default
}

Graph::~Graph() {
  // "DONE" FOR YOU
  // no implementation needed
}

vector<shared_ptr<Node>> Graph::getNodes() {
  // DONE FOR YOU
  return nodes;
}

vector<shared_ptr<Edge>> Graph::getEdges() {
  // DONE FOR YOU
  return edges;
}

int Graph::getClock() {
  // DONE FOR YOU
  return clock;
}

void Graph::addNode(shared_ptr<Node> n) {
  // DONE FOR YOU
  nodes.push_back(n);
}

void Graph::addEdge(shared_ptr<Edge> e) {
  // DONE FOR YOU
  edges.push_back(e);
}
 
void Graph::removeNode(shared_ptr<Node> n) {
  // DONE FOR YOU
  for (vector<shared_ptr<Node>>::iterator it = nodes.begin(); it != nodes.end(); it++) {
    if (n == *it) {
      nodes.erase(it);
      break;
    }
  }
}
 
void Graph::removeEdge(shared_ptr<Edge> e) {
  // DONE FOR YOU
  for (vector<shared_ptr<Edge>>::iterator it = edges.begin();
       it != edges.end(); 
       it++) {
    if (e == *it) {
      edges.erase(it);
      break;
    }
  }
}

void Graph::setDirected(bool val) {
  // DONE FOR YOU
  directed = val;
}

bool Graph::isDirected() {
  // DONE FOR YOU
  return directed;
}

set<shared_ptr<Edge>> Graph::getAdjacentEdges(shared_ptr<Node> n) {
  // DONE FOR YOU
  set<shared_ptr<Edge>> ret;
  for (vector<shared_ptr<Edge>>::iterator it = edges.begin(); it != edges.end(); it++) {
    shared_ptr<Edge> edge = *it;
    if (edge->getStart() == n) {
      ret.insert(edge);
    }
    if (!directed && (edge->getEnd() == n)) {
      ret.insert(edge);
    }
  }
  return ret;
}

void Graph::clear() {
  // Reset all nodes to initial state (WHITE color, -1 discovery/finish time, rank = -1, predecessor = nullptr)
  for (auto node : nodes) {
    node->clear();
  }
  
  // Reset all edges to UNDISCOVERED_EDGE type
  for (auto edge : edges) {
    edge->setType(UNDISCOVERED_EDGE);
  }

  // Reset the graph clock to 0
  clock = 0;
}

void Graph::tick(string message) {
  // DONE FOR YOU
  //
  // optional debugging function, disabled with if(false) to prevent
  // unused var warning. Change this however you want.
  //
  // recommended use: call it just after incrementing the clock.
  if (false) {
    cout << "// " << message << endl << make_dot(this) << endl;
  }
}

void Graph::dfs(shared_ptr<Node> start) {
  stack<shared_ptr<Node>> nodeStack; // Use stack for the stack
  nodeStack.push(start);

  // Reset the clock to 0 at the beginning of the dfs traversal
  clock = 0;

  start->setColor(GRAY, ++clock); // Mark the start node as discovered (GRAY) and update discovery time
  start->setRank(0); // The rank of the start node is 0

  while (!nodeStack.empty()) {
    shared_ptr<Node> current = nodeStack.top();
    nodeStack.pop();

    // Get the adjacent edges of the current node
    set<shared_ptr<Edge>> adjEdges = getAdjacentEdges(current);

    for (auto edge : adjEdges) {
      shared_ptr<Node> neighbor = edge->getEnd();
      if (neighbor->getColor() == WHITE) {
        neighbor->setColor(GRAY, ++clock); // Mark the neighbor as discovered (GRAY) and update discovery time
        neighbor->setRank(current->getRank() + 1); // The rank of the neighbor is one more than the current node
        nodeStack.push(neighbor);
      }
    }

    current->setColor(BLACK, ++clock); // Mark the current node as finished (BLACK) and update finish time
  }
}

void Graph::bfs(shared_ptr<Node> start) {
  queue<shared_ptr<Node>> nodeQueue; // Use queue for the queue
  nodeQueue.push(start);

  // Reset the clock to 0 at the beginning of the bfs traversal
  clock = 0;

  start->setColor(GRAY, ++clock); // Mark the start node as discovered (GRAY) and update discovery time
  start->setRank(0); // The rank of the start node is 0

  while (!nodeQueue.empty()) {
    shared_ptr<Node> current = nodeQueue.front();
    nodeQueue.pop();

    // Get the adjacent edges of the current node
    set<shared_ptr<Edge>> adjEdges = getAdjacentEdges(current);

    for (auto edge : adjEdges) {
      shared_ptr<Node> neighbor = edge->getEnd();
      if (neighbor->getColor() == WHITE) {
        neighbor->setColor(GRAY, ++clock); // Mark the neighbor as discovered (GRAY) and update discovery time
        neighbor->setRank(current->getRank() + 1); // The rank of the neighbor is one more than the current node
        nodeQueue.push(neighbor);
      }
    }

    current->setColor(BLACK, ++clock); // Mark the current node as finished (BLACK) and update finish time
  }
}

void Graph::bfs(shared_ptr<Node> start, shared_ptr<Node> finish) {
  queue<shared_ptr<Node>> nodeQueue;
  nodeQueue.push(start);

  start->setColor(GRAY, ++clock); // Mark the start node as discovered (GRAY) and update discovery time
  start->setRank(0); // The rank of the start node is 0

  while (!nodeQueue.empty()) {
    shared_ptr<Node> current = nodeQueue.front();
    nodeQueue.pop();

    if (current == finish) {
      // The finish node is found, mark it as finished (BLACK) and update finish time
      current->setColor(BLACK, ++clock);
      return;
    }

    // Get the adjacent edges of the current node
    set<shared_ptr<Edge>> adjEdges = getAdjacentEdges(current);

    for (auto edge : adjEdges) {
      shared_ptr<Node> neighbor = edge->getEnd();
      if (neighbor->getColor() == WHITE) {
        neighbor->setColor(GRAY, ++clock); // Mark the neighbor as discovered (GRAY) and update discovery time
        neighbor->setRank(current->getRank() + 1); // The rank of the neighbor is one more than the current node
        neighbor->setPredecessor(current);
        nodeQueue.push(neighbor);
      }
    }

    current->setColor(BLACK, ++clock); // Mark the current node as finished (BLACK) and update finish time
  }
}



// overloading operator << lets you put a Graph object into an output
// stream.
ostream &operator << (ostream& out, Graph graph) {
  // DONE FOR YOU
  out << graph.nodes.size() << " Nodes:" << endl;
  out << "[";
  for (vector<shared_ptr<Node>>::iterator it = graph.nodes.begin(); it != graph.nodes.end(); it++) {
    shared_ptr<Node> n = *it;
    out << *n << ", ";
  }
  out << "]" << endl;
  out << graph.edges.size() << " Edges:" << endl;
  out << "[";
  for (vector<shared_ptr<Edge>>::iterator it = graph.edges.begin(); it != graph.edges.end(); it++) {
    shared_ptr<Edge> e = *it;
    out << *e << ", ";
  }
  out << "]";
  return out;
}

// what is a helper function that turns a constant (e.g. WHITE or
// FORWARD_EDGE) into a string for debugging goodness. 
string what(int& v) {
  // DONE FOR YOU
  string ret = "Unknown";
  if (v == WHITE) {
    ret = "white";
  } else if (v == GRAY) {
    ret = "gray";
  } else if (v == BLACK) {
    ret = "black";
  } else if (v == UNDISCOVERED_EDGE) {
    ret = "undiscovered";
  } else if (v == TREE_EDGE) {
    ret = "tree";
  } else if (v == BACK_EDGE) {
    ret = "back";
  } else if (v == FORWARD_EDGE) {
    ret = "forward";
  } else if (v == CROSS_EDGE) {
    ret = "cross";
  }
  return ret;
}

// make_dot generates a dotfile string for viewing your graph. Have
// tick() print out the results of this function, and then use
// http://www.webgraphviz.com/ (or get a copy of GraphViz) to view the
// result. If you're stuck, this can help you get un-stuck.
string make_dot(Graph* g) {
  // DONE FOR YOU
  stringstream ss;
  vector<shared_ptr<Node>> nodes = g->getNodes();
  vector<shared_ptr<Edge>> edges = g->getEdges();
  string gt = "graph";
  string con = "--";
  if (g->isDirected()) {
    con = "->";
    gt = "digraph";
  }
  ss << gt << " homework {" << endl;
  int c, dt, ft, r;
  string textColor = "black";
  for (auto it=nodes.begin(); it != nodes.end(); ++it) {
    shared_ptr<Node> n = *it;
    n->getDiscoveryInformation(c, dt, ft, r);
    if (c == BLACK) {
      textColor = "white";
    } else {
      textColor = "black";
    }
    ss << "  " << n->getData() << " [ style=\"filled\" fontcolor=\"" << textColor << "\" fillcolor=\"" << what(c) << "\"" << " ]" << endl;
  }
  
  string edgeColor = "black";
  for (auto it=edges.begin(); it != edges.end(); ++it) {
    shared_ptr<Edge> e = *it;
    if (e->getType() == TREE_EDGE) {
      edgeColor = "black";
    } else if (e->getType() == FORWARD_EDGE) {
      edgeColor = "purple";
    } else if (e->getType() == BACK_EDGE) {
      edgeColor = "blue";
    } else if (e->getType() == CROSS_EDGE) {
      edgeColor = "green";
    } else {
      edgeColor = "gray";
    }
    ss << "  " << e->getStart()->getData() << con << e->getEnd()->getData() << " [color=\"" << edgeColor << "\"]" << endl;
  }
  ss << "}" << endl;
  return ss.str();
}