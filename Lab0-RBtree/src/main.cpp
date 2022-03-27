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

    auto inOrderList = testRBT->inorder();
    auto preOrderList = testRBT->preorder();
    auto postOrderList = testRBT->postorder();

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

    std::cout << std::endl << std::endl << "After remove: " << std::endl << std::endl;

    testRBT->remove(5);
    testRBT->remove(3);
    testRBT->remove(9);
    testRBT->remove(1);
    testRBT->remove(7);

    inOrderList = testRBT->inorder();
    std::cout << "Inorder: ";
    for (auto i: inOrderList) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "Preorder: ";
    preOrderList = testRBT->preorder();
    for (auto i: preOrderList) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "Postorder: ";
    postOrderList = testRBT->postorder();
    for (auto i: postOrderList) {
        std::cout << i << " ";
    }

    return 0;
}