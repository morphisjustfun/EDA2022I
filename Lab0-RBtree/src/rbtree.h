#pragma

#include <vector>

enum Color {
    RED, BLACK, DOUBLE_BLACK
};

struct Node {
    int value;
    int color;
    Node *left;
    Node *right;
    Node *parent;

    explicit Node(int value);
};

class RBTree {
private:
    Node *root;

    Node *insertUtil(Node *&origin, Node *&node);

    Node *removeUtil(Node *&origin, int value);

    void fixTree(Node *&node);

    int getColor(Node *&node);

    void setColor(Node *&node, int color);

    void rotateLeft(Node *&node);

    void rotateRight(Node *&node);

    void inorderUtil(Node *&node, std::vector<int> &vec);

    void preorderUtil(Node *&node, std::vector<int> &vec);

    void postorderUtil(Node *&node, std::vector<int> &vec);

    Node *searchNode(Node *&node, int value);

    Node *findSucesor(Node *&node);

public:
    RBTree();

    std::vector<int> inorden();

    std::vector<int> preorden();

    std::vector<int> posorden();

    void eliminarNodo(int value);

    void insert(int value);

    ~RBTree();
};
