#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
// #define ORDER 21
//#define ORDER 20
#define ORDER 3

void sortArray(int *&array, int size);

// BP node
struct Node {
    bool leaf;
    // keys
    int keys[ORDER];
    // size
    int size;
    // children
    Node *children[ORDER + 1];

    Node(int size = 1);

    ~Node();
};

// BP tree
class BplusTree {
    Node *findParent(Node *cursor, Node *child);

    void insertIntoInternalNode(Node *parentNode, Node *tempNode, int key);

public:
    Node *root;

    BplusTree();

    void insertar(int x);

    std::vector<int> BFS();

    ~BplusTree();
};


