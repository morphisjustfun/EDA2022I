#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
#define ORDER 21

void sortArray(int *&array, int size);

// BP node
struct Node {
  bool leaf;
  // keys
  int *keys;
  // size
  int size;
  // children
  Node **children;

  Node(int size = 1);

  ~Node();
};

// BP tree
class BplusTree {
  Node *root;
  Node *findParent(Node *cursor, Node *child);
  void insertIntoInternalNode(Node *parentNode, Node *tempNode, int key);

public:
  BplusTree();
  void insertar(int x);
  std::vector<int> BFS();

  ~BplusTree();
};


