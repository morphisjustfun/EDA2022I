#include <algorithm>
#include "rbtree.h"
#include <iostream>

Node::Node(int value) {
    this->value = value;

    color = RED;
    left = nullptr;
    right = nullptr;
    parent = nullptr;
}


int RBTree::getColor(Node *&node) {
    if (node == nullptr) return BLACK;
    return node->color;
}

void RBTree::setColor(Node *&node, int color) {
    if (node == nullptr) return;
    node->color = color;
}

/*          
 *        X                            Y
 *       / \     Rotar izquierda      / \
 *      A   Y    --------------->    X   C
 *         / \                      / \
 *        B   C                    A   B
 * 
 * La función se aplica a X
 */
void RBTree::rotateLeft(Node *&node) {
    // Node Y
    Node *rightChild = node->right;


    node->right = rightChild->left;         // Asignar como hijo derecho de X a B
    if (node->right != nullptr)
        node->right->parent = node;       // Asignar como nuevo padre de B a X


    rightChild->parent = node->parent;      // Asignar que el nuevo padre de Y sea el padre de X
    // Esto es para conectar con el resto del arbol
    // Asignar la Y como hijo derecho o izquierdo 
    // En el paso anterior contectamos a Y con el resto del arbol, ahora Y será asignado como hijo del nodo superior
    if (node->parent == nullptr)
        root = rightChild;                     // Si no hay padre, significa que llegamos a la raiz
    else if (node == node->parent->left)
        node->parent->left = rightChild;    // Antes de la rotacion, X era el hijo izquiero
    else
        node->parent->right = rightChild;   // Antes de la rotacion, X era el hijo derecho

    rightChild->left = node;                // Asignar a X como hijo izquierdo de Y
    node->parent = rightChild;              // Asignar a Y como padre de X
}

/*         
 *        Y                        X
 *       / \     Rotar derecha    / \
 *      X   C    ------------->  A   Y
 *     / \                          / \
 *    A   B                        B   C
 * 
 * La función se aplica a Y
 */
void RBTree::rotateRight(Node *&node) {
    // Node X
    Node *leftChild = node->left;


    node->left = leftChild->right;       // Asignar como hijo izquierdo de Y a B
    if (node->left != nullptr)
        node->left->parent = node;        // Asignar como nuevo padre de B a Y


    leftChild->parent = node->parent;    // Asignar que el nuevo padre de X sea el padre Y
    // Esto es para conectar con el resto del arbol
    // Asignar la X como hijo derecho o izquierdo 
    // En el paso anterior contectamos a X con el resto del arbol, ahora X será asignado como hijo del nodo superior
    if (node->parent == nullptr)
        root = leftChild;                   // Si no hay padre, significa que llegamos a la raiz
    else if (node == node->parent->left)
        node->parent->left = leftChild;  // Antes de la rotacion, Y era el hijo izquiero
    else
        node->parent->right = leftChild;  // Antes de la rotacion, Y era el hijo derecho


    leftChild->right = node;              // Asignar a Y como hijo derecho de X
    node->parent = leftChild;             // Asignar a X como padre de Y
}


Node *RBTree::insertUtil(Node *&origin, Node *&node) {
    // Arbol vacio
    if (origin == nullptr) return node;

    // Si el nuevo numero es menor al origin
    if (node->value < origin->value) {
        origin->left = insertUtil(origin->left, node);
        origin->left->parent = origin;
    }

        // Si el nuevo numero es mayo al origin
    else if (node->value > origin->value) {
        origin->right = insertUtil(origin->right, node);
        origin->right->parent = origin;
    }

    return origin;
}


void RBTree::fixTree(Node *&node) {
    Node *parent = nullptr;
    Node *grandParent = nullptr;
    while (node != root && getColor(node) == RED && getColor(node->parent) == RED) {
        parent = node->parent;
        grandParent = parent->parent;

        // El parent esta a la izquierda
        if (parent == grandParent->left) {
            Node *uncle = grandParent->right;

            // CASO I: parent y uncle son rojos
            if (getColor(uncle) == RED) {
                setColor(parent, BLACK);
                setColor(uncle, BLACK);
                setColor(grandParent, RED);
                node = grandParent;
            } else {
                // CASO II: parent y el hijo tienen distintas direcciones
                if (node == parent->right) {
                    rotateLeft(parent);
                    node = parent;
                    parent = node->parent;
                }

                // CASO III: parent y el hijo tienen la misma dirección
                rotateRight(grandParent);
                std::swap(parent->color, grandParent->color);
                node = parent;
            }
        }

            // El parent esta a la derecha
        else {
            Node *uncle = grandParent->left;

            // CASO I: parent y uncle son rojos
            if (getColor(uncle) == RED) {
                setColor(parent, BLACK);
                setColor(uncle, BLACK);
                setColor(grandParent, RED);
                node = grandParent;
            } else {
                // CASO II: parent y el hijo tienen distintas direcciones
                if (node == parent->left) {
                    rotateRight(parent);
                    node = parent;
                    parent = node->parent;
                }

                // CASO III: parent y el hijo tienen la misma dirección
                rotateLeft(grandParent);
                std::swap(parent->color, grandParent->color);
                node = parent;
            }
        }
    }
    setColor(root, BLACK);
}

void RBTree::inorderUtil(Node *&node, std::vector<int> &vec) {
    if (node == nullptr) return;
    inorderUtil(node->left, vec);
    vec.push_back(node->value);
    inorderUtil(node->right, vec);
}

std::vector<int> RBTree::inorder() {
    std::vector<int> vec;
    inorderUtil(root, vec);
    return vec;
}

void RBTree::preorderUtil(Node *&node, std::vector<int> &vec) {
    if (node == nullptr) return;
    vec.push_back(node->value);
    preorderUtil(node->left, vec);
    preorderUtil(node->right, vec);
}

std::vector<int> RBTree::preorder() {
    std::vector<int> vec;
    preorderUtil(root, vec);
    return vec;
}

void RBTree::postorderUtil(Node *&node, std::vector<int> &vec) {
    if (node == nullptr) return;
    postorderUtil(node->left, vec);
    postorderUtil(node->right, vec);
    vec.push_back(node->value);
}

std::vector<int> RBTree::postorder() {
    std::vector<int> vec;
    postorderUtil(root, vec);
    return vec;
}


void RBTree::insert(int value) {
    Node *newNode = new Node(value);
    root = insertUtil(root, newNode);
    fixTree(newNode);
}

Node *RBTree::searchNode(Node *&node, int value) {
    if (node == nullptr) return nullptr;
    if (node->value == value) return node;
    if (value < node->value) return searchNode(node->left, value);
    return searchNode(node->right, value);
}

Node *RBTree::findSucesor(Node *&node) {
    Node *sucesor = node->right;
    while (sucesor->left != nullptr) {
        sucesor = sucesor->left;
    }
    return sucesor;
}


Node *RBTree::removeUtil(Node *&origin, int value) {
    // classic BST remove but it returns the node that replaces the deleted one
    if (origin == nullptr) return nullptr;

    if (value > origin->value) {
        return removeUtil(origin->right, value);
    }

    if (value < origin->value) {
        return removeUtil(origin->left, value);
    }

    if (origin->left == nullptr || origin->right == nullptr) {
        return origin;
    }

    Node *sucesor = findSucesor(origin);
    origin->value = sucesor->value;
    return removeUtil(origin->right, sucesor->value);
}

void RBTree::remove(int value) {
    Node *nodeToBeRemoved = removeUtil(root, value);

    if (nodeToBeRemoved == nullptr) return;

    if (nodeToBeRemoved == root) {
        root = nullptr;
        return;
    }

    // if node to be removed is red or has one child which is red
    if (getColor(nodeToBeRemoved) == RED || getColor(nodeToBeRemoved->left) == RED ||
        getColor(nodeToBeRemoved->right) == RED) {
        // child (can be nullptr)
        Node *child = nullptr;
        if (nodeToBeRemoved->left != nullptr) {
            child = nodeToBeRemoved->left;
        } else {
            child = nodeToBeRemoved->right;
        }

        // if child is nullptr
        if (child == nullptr) {
            // if node is at the left of its parent reassign its child
            if (nodeToBeRemoved->parent->left == nodeToBeRemoved) {
                nodeToBeRemoved->parent->left = nullptr;
            } else {
                nodeToBeRemoved->parent->right = nullptr;
            }
            // then delete the node
            delete nodeToBeRemoved;
        } else {
            // if node is at the left of its parent reassign its child
            if (nodeToBeRemoved->parent->left == nodeToBeRemoved) {
                nodeToBeRemoved->parent->left = child;
            } else {
                nodeToBeRemoved->parent->right = child;
            }
            // then assign color black to child
            setColor(child, BLACK);
            // then delete the node
            delete nodeToBeRemoved;
        }
        return;
    }

    // if the node to be deleted and the parent are black then a double black node is created
    setColor(nodeToBeRemoved, DOUBLE_BLACK);
    Node *parent = nullptr;
    Node *sibling = nullptr;
    Node *nodeTemp = nodeToBeRemoved;

    // if the double black node is the root then we can simply turn it into a black node
    // otherwise we need to fix the tree

    while (nodeTemp != root && getColor(nodeTemp) == DOUBLE_BLACK) {
        parent = nodeTemp->parent;
        // to find sibling we need to check if nodeTemp is at the left or right of its parent
        if (parent->left == nodeTemp) {
            sibling = parent->right;
            // if the sibling is BLACK
            if (getColor(sibling) == BLACK) {
                // if at least one child of sibling is red then we make rotation
                if (getColor(sibling->left) == RED || getColor(sibling->right) == RED) {
                    // if the red child of sibling is on the right or both children are red
                    if (getColor(sibling->right) == RED ||
                        (getColor(sibling->right) == RED && getColor(sibling->left) == RED)) {
                        // assign parent's color to sibling
                        setColor(sibling, parent->color);
                        // make parent black
                        setColor(parent, BLACK);
                        // make sibling's right child black
                        setColor(sibling->right, BLACK);
                        // then we need to rotate left
                        rotateLeft(parent);
                        nodeTemp = parent;
                    }
                        // if the red child of the sibling is on the left
                    else {
                        // need to make double rotation
                        // make sibling left black
                        setColor(sibling->left, BLACK);
                        // make sibling red
                        setColor(sibling, RED);
                        // rotate right the sibling
                        rotateRight(sibling);
                        // work with the right child of the parent
                        sibling = parent->right;
                        // assign parent's color to sibling
                        setColor(sibling, parent->color);
                        // make parent black
                        setColor(parent, BLACK);
                        // make sibling's right child black
                        setColor(sibling->right, BLACK);
                        // then we need to rotate left
                        rotateLeft(parent);
                        nodeTemp = parent;
                    }
                }
                    // if both children of sibling are black then we make a color flip
                else {
                    setColor(sibling, RED);
                    // if parent is black then we can simply make it double black
                    if (getColor(parent) == BLACK) {
                        setColor(parent, DOUBLE_BLACK);
                    }
                        // if parent is red then we need to make it black
                    else {
                        setColor(parent, BLACK);
                    }
                    // now fix parent
                    nodeTemp = parent;
                }
            }
                // otherwise, if its red
            else {
                // rotation to move sibling (left) and swap the color of sibling and parent
                setColor(sibling, BLACK);
                setColor(parent, RED);
                rotateLeft(parent);
            }

        } else {
            sibling = parent->left;
            if (getColor(sibling) == BLACK) {
                if (getColor(sibling->left) == RED || getColor(sibling->right) == RED) {
                    if (getColor(sibling->left) == RED ||
                        (getColor(sibling->left) == RED && getColor(sibling->right) == RED)) {
                        setColor(sibling, parent->color);
                        setColor(parent, BLACK);
                        setColor(sibling->left, BLACK);
                        rotateRight(parent);
                        nodeTemp = parent;
                    } else {
                        setColor(sibling->right, BLACK);
                        setColor(sibling, RED);
                        rotateLeft(sibling);
                        sibling = parent->left;
                        setColor(sibling, parent->color);
                        setColor(parent, BLACK);
                        setColor(sibling->left, BLACK);
                        rotateRight(parent);
                        nodeTemp = parent;
                    }
                } else {
                    setColor(sibling, RED);
                    if (getColor(parent) == BLACK) {
                        setColor(parent, DOUBLE_BLACK);
                    } else {
                        setColor(parent, BLACK);
                    }
                    nodeTemp = parent;
                }
            } else {
                setColor(sibling, BLACK);
                setColor(parent, RED);
                rotateRight(parent);
            }
        }
        // clean up nodeToBeRemoved's children
        if (nodeToBeRemoved == nodeToBeRemoved->parent->left)
            nodeToBeRemoved->parent->left = nullptr;
        else
            nodeToBeRemoved->parent->right = nullptr;
        // delete
        delete nodeToBeRemoved;
        // root is always black (we could make it double black because of the while loop)
        setColor(root, BLACK);
    }

}


RBTree::RBTree() {
    root = nullptr;
}

RBTree::~RBTree() {
}
