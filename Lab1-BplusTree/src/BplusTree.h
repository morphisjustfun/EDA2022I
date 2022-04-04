#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
#include <cmath>
//#define ORDER 20

// ORDER = MAX_KEYS = (REAL ORDER) - 1
#define ORDER 4


// BP node
struct Node {
    bool leaf{};
    // keys
    int keys[ORDER]{};
    // size
    int size;
    // children
    Node *children[ORDER + 1]{};

    explicit Node(int size = 1);

    ~Node();
};

// BP tree
class BplusTree {
    Node *findParent(Node *cursor, Node *child);

    void insertIntoInternalNode(Node *parentNode, Node *tempNode, int key);
    void removeFromInternalNode(Node *parentNode, Node *tempNode, int key);

public:
    Node *root;

    BplusTree();

    void insert(int value);
    void remove(int value);

    std::vector<int> BFS();

    ~BplusTree();
};


