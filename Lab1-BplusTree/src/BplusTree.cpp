#include "BplusTree.h"

Node::Node(int size) {
    this->size = size;
}

Node::~Node() {
    if (leaf) {
        return;
    }
    for (int i = 0; i < size; i++) {
        delete children[i];
    }
}

BplusTree::BplusTree() { root = nullptr; }

BplusTree::~BplusTree() {
    delete root;
}

void BplusTree::insert(int value) {
    if (root == nullptr) {
        root = new Node();
        root->keys[0] = value;
        root->leaf = true;
        return;
    }

    Node *tempPointer = root;
    Node *parentTempPointer = nullptr;

    // iterate until get to a leaf node
    while (!tempPointer->leaf) {
        // assign previous node to parent
        // so that the parent will always be the previous node
        parentTempPointer = tempPointer;
        // find the right node to insert
        for (int i = 0; i < tempPointer->size; i++) {
            // if the value is less than the current node then go to the left
            if (value < tempPointer->keys[i]) {
                tempPointer = tempPointer->children[i];
                break;
            }
            // if the value reachs the end of the node then go to the right
            if (i == tempPointer->size - 1) {
                tempPointer = tempPointer->children[i + 1];
                break;
            }
        }
    }

    // we got to a leaf node
    // if the node is not full then insert the value
    if (tempPointer->size < ORDER) {
        // find where to insert the value
        int correctIndex = 0;
        while (value > tempPointer->keys[correctIndex] &&
               correctIndex < tempPointer->size)
            correctIndex++;

        // shift the values to the right
        for (int actualIndex = tempPointer->size; actualIndex > correctIndex;
             actualIndex--)
            tempPointer->keys[actualIndex] = tempPointer->keys[actualIndex - 1];

        // insert the value
        tempPointer->keys[correctIndex] = value;

        // the child of the last element of a leaft node is the next node (from the
        // leaf_node linked list) when inserting a new value then we have to assign
        // to the node just inserted the next node (from the leaf_node linked list)
        tempPointer->children[tempPointer->size] =
                tempPointer->children[tempPointer->size - 1];

        // and remove the previous one
        tempPointer->children[tempPointer->size - 1] = nullptr;

        // finally, add 1 to the size
        tempPointer->size++;

        return;
    }

    // if the node is full then create a new leaf
    Node *newLeaf = new Node();
    newLeaf->leaf = true;

    // create a fake array of keys which will contain the value to be inserted
    int fakeArray[ORDER + 1];
    // then we will insert everything plus the new value

    for (int i = 0; i < ORDER; i++) {
        fakeArray[i] = tempPointer->keys[i];
    }

    int correctIndex = 0;

    // sort the array
    while (value > fakeArray[correctIndex] && correctIndex < ORDER)
        correctIndex++;

    // shift
    for (int actualIndex = ORDER; actualIndex > correctIndex; actualIndex--)
        fakeArray[actualIndex] = fakeArray[actualIndex - 1];

    // insert the value
    fakeArray[correctIndex] = value;

    // the nodes will be splited in half
    tempPointer->size = (ORDER + 1) / 2;

    // the node that is being splited will have as child the new leaf
    tempPointer->children[tempPointer->size] = newLeaf;
    // the new leaf will have as last child the next node in the liked_list which
    // is the previous children[ORDER
    // ] of the node that is being splited
    newLeaf->children[newLeaf->size] = tempPointer->children[ORDER];
    // the node that is being splited will have nullptr as last child (no
    // linked_list)
    tempPointer->children[ORDER] = nullptr;

    // the new leaf will have the value that is not in the node

    // as the bptree is right biased we will insert at the left elements that are
    // strictly smaller than the new value
    for (int indexTempPointer = 0; indexTempPointer < tempPointer->size;
         indexTempPointer++) {
        tempPointer->keys[indexTempPointer] = fakeArray[indexTempPointer];
    }

    for (int indexTempPointer = tempPointer->size; indexTempPointer < ORDER + 1;
         indexTempPointer++) {
        newLeaf->keys[indexTempPointer - tempPointer->size] =
                fakeArray[indexTempPointer];
    }
    newLeaf->size = ORDER + 1 - tempPointer->size;


    // if the tempNode is the root
    if (tempPointer == root) {
        // new create a new root with only 1 key
        Node *newRoot = new Node();
        newRoot->leaf = false;
        // the new root will have as left child the node that is being splited
        // the new root will have as right child the new leaf
        newRoot->children[0] = tempPointer;
        newRoot->children[1] = newLeaf;

        newRoot->keys[0] = newLeaf->keys[0];
        root = newRoot;
        return;
    }

    // insert into internal nodes
    insertIntoInternalNode(parentTempPointer, newLeaf, newLeaf->keys[0]);
}

void BplusTree::insertIntoInternalNode(Node *parentNode, Node *tempNode,
                                       int key) {
    if (parentNode->size < ORDER) {
        // find where key should be inserted
        int correctIndex = 0;
        while (key > parentNode->keys[correctIndex] &&
               correctIndex < parentNode->size)
            correctIndex++;

        // shift the keys to the right
        for (int indexParent = parentNode->size; indexParent >= correctIndex;
             indexParent--) {
            if (indexParent == correctIndex) {
                parentNode->keys[indexParent] = key;
                break;
            }
            parentNode->keys[indexParent] = parentNode->keys[indexParent - 1];
        }

        // shift the children to the right
        for (int indexParent = parentNode->size + 1;
             indexParent >= correctIndex + 1; indexParent--) {
            if (indexParent == correctIndex + 1) {
                parentNode->children[indexParent] = tempNode;
                break;
            }
            parentNode->children[indexParent] = parentNode->children[indexParent - 1];
        }

        // add 1 to the size
        parentNode->size++;
        return;
    }


    // if the parent node is full then create a new internal node
    Node *newInternalNode = new Node();

    // create a fake array of keys which will contain the value to be inserted
    // create a fake array of Nodes which will contain the children to be inserted
    int fakeKeys[ORDER + 1];
    Node *fakeChildren[ORDER + 2];

    int correctIndex = 0;

    // insert every key of the parent node into the fake array of keys
    // insert every child of the parent node into the fake array of children
    for (int indexParentNode = 0; indexParentNode < ORDER + 1;
         indexParentNode++) {
        if (indexParentNode < ORDER) {
            fakeKeys[indexParentNode] = parentNode->keys[indexParentNode];
        }
        fakeChildren[indexParentNode] = parentNode->children[indexParentNode];
    }

    // find where key should be inserted
    while (key > fakeKeys[correctIndex] && correctIndex < ORDER)
        correctIndex++;

    // insert the new key and child into the fake array of keys and children
    // shift the keys to the right
    for (int indexFakeKeys = ORDER; indexFakeKeys > correctIndex;
         indexFakeKeys--)
        fakeKeys[indexFakeKeys] = fakeKeys[indexFakeKeys - 1];

    // shift the children to the right
    for (int indexFakeChildren = ORDER + 1; indexFakeChildren > correctIndex + 1;
         indexFakeChildren--)
        fakeChildren[indexFakeChildren] = fakeChildren[indexFakeChildren - 1];

    // assign the new key and child to the fake array of keys and children
    fakeKeys[correctIndex] = key;
    fakeChildren[correctIndex + 1] = tempNode;


    newInternalNode->leaf = false;
    parentNode->size = (ORDER + 1) / 2;

    for (int i = 0; i < ORDER - parentNode->size; i++) {
        parentNode->keys[i] = fakeKeys[i];
    }
    for (int i = 0; i < ORDER - parentNode->size + 1; i++) {
        parentNode->children[i] = fakeChildren[i];
    }

    newInternalNode->size = parentNode->size;


    for (int index = 0; index < ORDER - newInternalNode->size + 1; index++) {
        if (index < ORDER - newInternalNode->size) {
            newInternalNode->keys[index] = fakeKeys[index + parentNode->size + 1];
        }
        newInternalNode->children[index] =
                fakeChildren[index + parentNode->size + 1];
    }

    newInternalNode->size = ORDER - parentNode->size;

    // if parent node is root
    if (parentNode == root) {
        // create a new root with only 1 key
        Node *newRoot = new Node();
        newRoot->leaf = false;
        // the new root will have as left child the parent node
        // the new root will have as right child the new internal node
        newRoot->children[0] = parentNode;
        newRoot->children[1] = newInternalNode;
        // the new root will have as key the first key of the parent node

        newRoot->keys[0] = fakeKeys[parentNode->size];

        root = newRoot;

        return;
    }

    // insert recursively into the internal node
    insertIntoInternalNode(findParent(root, parentNode), newInternalNode,
//                           parentNode->keys[parentNode->size]);
                           fakeKeys[parentNode->size]);
}

Node *BplusTree::findParent(Node *refNode, Node *node) {
    Node *parentResult = nullptr;

    // if the reference node reached the leaf node
    if (refNode->leaf || (refNode->children[0])->leaf) {
        return nullptr;
    }

    // Traverse the current node with
    // all its child
    for (int index = 0; index < refNode->size + 1; index++) {

        // if refNode has a child which is the node
        if (refNode->children[index] == node) {
            parentResult = refNode;
            return parentResult;
        }

        // if refNode has a child which is not the node
        // recursively call the function
        parentResult = findParent(refNode->children[index], node);
        // If parent is found, then return
        if (parentResult != nullptr)
            return parentResult;
    }

    // Return parent node
    return parentResult;
}

std::vector<int> BplusTree::BFS() {
    std::vector<int> result;
    if (root == nullptr)
        return result;
    std::queue<Node *> q;
    q.push(root);
    while (!q.empty()) {
        const Node *current = q.front();
        if (!current->leaf) {
            for (int i = 0; i < current->size + 1; ++i) {
                if (current->children[i] != nullptr) {
                    q.push(current->children[i]);
                }
            }
        }
        for (int i = 0; i < current->size; ++i) {
            result.push_back(current->keys[i]);
        }
        q.pop();
    }
    return result;
}

void BplusTree::remove(int value) {
    if (root == nullptr) {
        return;
    }

    Node *tempPointer = root;
    Node *parentTempPointer = nullptr;

    int indexLeftSibling = -1;
    int indexRightSibling = -1;


    // traverse until the leaf node is reached
    while (!tempPointer->leaf) {
        bool foundLess = false;
        parentTempPointer = tempPointer;
        for (int index = 0; index < tempPointer->size; index++) {
            // if the value is less than the current key then go to the left
            if (value < tempPointer->keys[index]) {
                foundLess = true;
                indexLeftSibling = index - 1;
                indexRightSibling = index + 1;
                tempPointer = tempPointer->children[index];
                break;
            }
        }
        if (foundLess) continue;
        // if index is the last index then go to the right
        indexLeftSibling = tempPointer->size - 1;
        indexRightSibling = tempPointer->size + 1;
        tempPointer = tempPointer->children[tempPointer->size];
    }

    // check if the value is present in the leaf node
    bool found = false;
    int indexValue = -1;
    for (int index = 0; index < tempPointer->size; index++) {
        if (tempPointer->keys[index] == value) {
            indexValue = index;
            found = true;
            break;
        }
    }

    // if the value is not present in the leaf node
    if (!found) {
        std::cout << "Value not found" << std::endl;
        return;
    }

    // shift to remove the value
    for (int index = indexValue; index < tempPointer->size - 1; index++) {
        tempPointer->keys[index] = tempPointer->keys[index + 1];
    }

    // fix location of last child
    tempPointer->children[tempPointer->size - 1] = tempPointer->children[tempPointer->size];
    tempPointer->children[tempPointer->size] = nullptr;

    // remove the last key (if exists)
    tempPointer->keys[tempPointer->size - 1] = 0;
    tempPointer->size--;

    // i3 the leaf node is root then remove its children
    if (tempPointer == root) {
        for (auto &index: tempPointer->children) {
            index = nullptr;
        }
        if (tempPointer->size == 0) {
            root = nullptr;
            return;
        }
        return;
    }

    // more than min keys then we are done (after remove)
    // TODO
    if (tempPointer->size >= (ORDER + 1) / 2) {
        int indexCurrentNode = 0;
        if (indexLeftSibling < 0)
            indexCurrentNode = indexRightSibling - 1;
        else
            indexCurrentNode = indexLeftSibling;

        if (parentTempPointer->keys[indexCurrentNode] == value)
            parentTempPointer->keys[indexCurrentNode] = tempPointer->keys[0];

        return;
    }

    // first check if some sibling can be borrowed from
    // first left then right

    // if there is left sibling
    if (indexLeftSibling >= 0) {
        auto leftSibling = parentTempPointer->children[indexLeftSibling];
        // if left sibling has 1 or more than min keys then we are done
        if (leftSibling->size >= (ORDER + 1) / 2 + 1) {
            // shift to borrow from left sibling
            for (int index = tempPointer->size; index > 0; index--) {
                tempPointer->keys[index] = tempPointer->keys[index - 1];
            }
            tempPointer->size++;
            // pass the child
            tempPointer->children[tempPointer->size] = tempPointer->children[tempPointer->size - 1];
            tempPointer->children[tempPointer->size - 1] = nullptr;
            // pass the key (borrowed) - max key from left sibling
            tempPointer->keys[0] = leftSibling->keys[leftSibling->size - 1];
            // remove the key from left sibling
            leftSibling->size--;
            // update children of left sibling
            leftSibling->children[leftSibling->size] = tempPointer;
            leftSibling->children[leftSibling->size + 1] = nullptr;
            // update the parent
            parentTempPointer->keys[indexLeftSibling] = tempPointer->keys[0];
            return;
        }
    }

    // if there is right sibling
    if (indexRightSibling < parentTempPointer->size + 1) {
        auto rightSibling = parentTempPointer->children[indexRightSibling];
        // if right sibling has 1 or more than min keys then we are done
        if (rightSibling->size >= (ORDER + 1) / 2 + 1) {
            tempPointer->size++;
            // assign the key of the right sibling to the leaf node - min key from right sibling
            tempPointer->keys[tempPointer->size - 1] = rightSibling->keys[0];
            // pass the child
            tempPointer->children[tempPointer->size] = tempPointer->children[tempPointer->size - 1];
            tempPointer->children[tempPointer->size - 1] = nullptr;
            // remove from right sibling
            rightSibling->size--;
            rightSibling->children[rightSibling->size] = rightSibling->children[rightSibling->size + 1];
            rightSibling->children[rightSibling->size + 1] = nullptr;

            // update the parent
            for (int index = 0; index < rightSibling->size; index++) {
                rightSibling->keys[index] = rightSibling->keys[index + 1];
            }

            parentTempPointer->keys[indexRightSibling - 1] = rightSibling->keys[0];
            parentTempPointer->keys[indexRightSibling - 2] = tempPointer->keys[0];
            return;
        }
    }

    // if there is no sibling that can be borrowed from
    // then we need to merge with some sibling

    // check if there is left sibling
    if (indexLeftSibling >= 0) {
        // keep elements in left sibling
        auto leftSibling = parentTempPointer->children[indexLeftSibling];

        // merge with left sibling
        for (int index = 0; index < tempPointer->size; index++) {
            leftSibling->keys[leftSibling->size + index] = tempPointer->keys[index];
        }
        // update children
        leftSibling->children[leftSibling->size] = nullptr;
        leftSibling->children[leftSibling->size + tempPointer->size] = tempPointer->children[tempPointer->size];
        // update size
        leftSibling->size += tempPointer->size;
        // remove from internal node
        removeFromInternalNode(parentTempPointer, leftSibling, parentTempPointer->keys[indexLeftSibling]);
        delete tempPointer;
        return;
    }

    // check if there is right sibling
    if (indexRightSibling < parentTempPointer->size + 1) {
        // keep elements tempPointer
        auto rightSibling = parentTempPointer->children[indexRightSibling];

        for (int index = 0; index < rightSibling->size; index++) {
            tempPointer->keys[tempPointer->size + index] = rightSibling->keys[index];
        }
        // update children
        tempPointer->children[tempPointer->size] = nullptr;
        tempPointer->children[tempPointer->size + rightSibling->size] = rightSibling->children[rightSibling->size];
        // update size
        tempPointer->size += rightSibling->size;
        // remove from internal node
        removeFromInternalNode(parentTempPointer, tempPointer, parentTempPointer->keys[indexRightSibling - 1]);
        delete rightSibling;
        return;
    }
}

void BplusTree::removeFromInternalNode(Node *parentNode, Node *tempNode, int key) {
    if (parentNode == root && parentNode->size == 1) {
        if (parentNode->children[0] == tempNode) {
            root = parentNode->children[0];
            return;
        } else if (parentNode->children[1] == tempNode) {
            root = parentNode->children[1];
            return;
        }
    }
    int indexValue = 0;
    for (int index = 0; index < parentNode->size; index++) {
        if (parentNode->keys[index] == key) {
            indexValue = index;
            break;
        }
    }

    for (int index = indexValue; index < parentNode->size; index++) {
        parentNode->keys[index] = parentNode->keys[index + 1];
    }

    for (indexValue = 0; indexValue < parentNode->size + 1; indexValue++) {
        if (parentNode->children[indexValue] == tempNode) {
            break;
        }
    }

    // TODO i = indexValue -> i=indexValue+1
    for (int i = indexValue + 1; i < parentNode->size + 1; i++) {
        parentNode->children[i] = parentNode->children[i + 1];
    }

    parentNode->size--;
    // if the node has one remaining key (or more)
    if (parentNode->size >= (ORDER + 1) / 2 - 1)
        return;
    if (parentNode == root)
        return;

    Node *parent = findParent(root, parentNode);
    int indexLeftSibling = 0;
    int indexRightSibling = 0;

    for (indexValue = 0; indexValue < parent->size + 1; indexValue++) {
        if (parent->children[indexValue] == parentNode) {
            indexLeftSibling = indexValue - 1;
            indexRightSibling = indexValue + 1;
            break;
        }
    }
    // if there is left sibling
    if (indexLeftSibling >= 0) {
        auto leftSibling = parent->children[indexLeftSibling];
        if (leftSibling->size >= (ORDER + 1) / 2) {
            for (int index = parentNode->size; index > 0; index--) {
                parentNode->keys[index] = parentNode->keys[index - 1];
            }
            parentNode->keys[0] = parent->keys[indexLeftSibling];
            parent->keys[indexLeftSibling] = leftSibling->keys[leftSibling->size - 1];
            for (int index = parentNode->size + 1; index > 0; index--) {
                parentNode->children[index] = parentNode->children[index - 1];
            }
            parentNode->children[0] = leftSibling->children[leftSibling->size];
            parentNode->size++;
            leftSibling->size--;
            return;
        }
    }

    // if there is right sibling
    if (indexRightSibling <= parent->size) {
        auto rightSibling = parent->children[indexRightSibling];
        if (rightSibling->size >= (ORDER + 1) / 2) {
            parentNode->keys[parentNode->size] = parent->keys[indexValue];
            parent->keys[indexValue] = rightSibling->keys[0];
            for (int index = 0; index < rightSibling->size - 1; index++) {
                rightSibling->keys[index] = rightSibling->keys[index + 1];
            }
            parentNode->children[parentNode->size + 1] = rightSibling->children[0];
            for (int index = 0; index < rightSibling->size; ++index) {
                rightSibling->children[index] = rightSibling->children[index + 1];
            }
            parentNode->size++;
            rightSibling->size--;
            return;
        }
    }
    // if it can not be borrowed
    if (indexLeftSibling >= 0) {
        auto leftSibling = parent->children[indexLeftSibling];
        leftSibling->keys[leftSibling->size] = parent->keys[indexLeftSibling];
        for (int index = leftSibling->size + 1, j = 0; j < parentNode->size; j++) {
            leftSibling->keys[index] = parentNode->keys[j];
        }
        for (int index = leftSibling->size + 1, j = 0; j < parentNode->size + 1; j++) {
            leftSibling->children[index] = parentNode->children[j];
            parentNode->children[j] = nullptr;
        }
        leftSibling->size += parentNode->size + 1;
        parentNode->size = 0;
        removeFromInternalNode(parent, parentNode, parent->keys[indexLeftSibling]);
    } else if (indexRightSibling <= parent->size) {
        auto rightSibling = parent->children[indexRightSibling];
        parentNode->keys[parentNode->size] = parent->keys[indexRightSibling - 1];
        for (int index = parentNode->size + 1, j = 0; j < rightSibling->size; j++) {
            parentNode->keys[index] = rightSibling->keys[j];
        }
        for (int index = parentNode->size + 1, j = 0; j < rightSibling->size + 1; j++) {
            parentNode->children[index] = rightSibling->children[j];
            rightSibling->children[j] = nullptr;
        }
        parentNode->size += rightSibling->size + 1;
        rightSibling->size = 0;
        removeFromInternalNode(parent, rightSibling, parent->keys[indexRightSibling - 1]);
    }
}
