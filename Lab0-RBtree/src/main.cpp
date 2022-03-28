#include "rbtree.h"
#include <iostream>

int main() {
    auto testRBT = new RBTree();

    testRBT->insert(5);
    testRBT->insert(3);
    testRBT->insert(9);
    testRBT->insert(1);
    testRBT->insert(7);
    testRBT->insert(8);
    testRBT->insert(10);
    testRBT->insert(6);
    testRBT->insert(4);
    testRBT->insert(2);

    auto inOrderList = testRBT->inorden();
    auto preOrderList = testRBT->preorden();
    auto postOrderList = testRBT->posorden();

    std::cout << "Inorder: ";
    for (auto i: inOrderList) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "Preorder: ";
    for (auto i: preOrderList) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "Postorder: ";
    for (auto i: postOrderList) {
        std::cout << i << " ";
    }

    std::cout << std::endl << std::endl << "After eliminarNodo: " << std::endl << std::endl;

    testRBT->eliminarNodo(5);
    testRBT->eliminarNodo(3);
    testRBT->eliminarNodo(9);
    testRBT->eliminarNodo(1);
    testRBT->eliminarNodo(7);

    inOrderList = testRBT->inorden();
    std::cout << "Inorder: ";
    for (auto i: inOrderList) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "Preorder: ";
    preOrderList = testRBT->preorden();
    for (auto i: preOrderList) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "Postorder: ";
    postOrderList = testRBT->posorden();
    for (auto i: postOrderList) {
        std::cout << i << " ";
    }

    return 0;
}