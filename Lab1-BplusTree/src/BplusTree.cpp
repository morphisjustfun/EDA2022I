#include "BplusTree.h"

void sortArray(int *&array, int size) {
  int i, j, temp;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size - 1; j++) {
      if (array[j] > array[j + 1]) {
        temp = array[j];
        array[j] = array[j + 1];
        array[j + 1] = temp;
      }
    }
  }
}

Node::Node(int size) {
  this->keys = new int[ORDER];
  this->children = new Node *[ORDER + 1];
  this->size = size;
}

Node::~Node() {
  delete[] keys;
  delete[] children;
}

BplusTree::BplusTree() { root = nullptr; }

BplusTree::~BplusTree() {
  if (root != nullptr) {
    delete root;
  }
}

void BplusTree::insertar(int value) {
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
    tempPointer->children[tempPointer->size + 1] =
        tempPointer->children[tempPointer->size];

    // and remove the previous one
    tempPointer->children[tempPointer->size] = nullptr;

    // finally, add 1 to the size
    tempPointer->size++;

    return;
  }

  // if the node is full then create a new leaf
  Node *newLeaf = new Node();
  newLeaf->leaf = true;

  // create a fake array of keys which will contain the value to be inserted
  int *fakeArray = new int[ORDER + 1];
  // then we will insert everything plus the new value

  for (int i = 0; i < ORDER + 1; i++) {
    if (i < ORDER)
      fakeArray[i] = tempPointer->keys[i];
    else
      fakeArray[i] = value;
  }

  // sort the array
  sortArray(fakeArray, ORDER + 1);

  // the nodes will be splited in half
  tempPointer->size = (ORDER + 1) / 2;
  // the new leaf will have the value that is not in the node
  newLeaf->size = (ORDER + 1) / 2;

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

  // the node that is being splited will have as child the new leaf
  tempPointer->children[tempPointer->size] = newLeaf;
  // the new leaf will have as last child the next node in the liked_list which
  // is the previous children[ORDER
  // ] of the node that is being splited
  newLeaf->children[newLeaf->size] = tempPointer->children[ORDER];
  // the node that is being splited will have nullptr as last child (no
  // linked_list)
  tempPointer->children[ORDER] = nullptr;

  // if the tempNode is the root
  if (tempPointer == root) {
    // new create a new root with only 1 key
    Node *newRoot = new Node();
    newRoot->leaf = false;
    // the new root will have as left child the node that is being splited
    // the new root will have as right child the new leaf
    newRoot->children[0] = tempPointer;
    newRoot->children[1] = newLeaf;
    // the new root will have as key the first key of the node that is being
    // splited
    newRoot->keys[0] = tempPointer->keys[0];

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
  int *fakeKeys = new int[ORDER + 1];
  Node **fakeChildren = new Node *[ORDER + 2];

  int correctIndex = 0;
  // find where key should be inserted
  while (key > fakeKeys[correctIndex] && correctIndex < ORDER)
    correctIndex++;

  // insert every key of the parent node into the fake array of keys
  // insert every child of the parent node into the fake array of children
  for (int indexParentNode = 0; indexParentNode < ORDER + 1;
       indexParentNode++) {
    if (indexParentNode < ORDER) {
      fakeKeys[indexParentNode] = parentNode->keys[indexParentNode];
    }
    fakeChildren[indexParentNode] = parentNode->children[indexParentNode];
  }

  // insert the new key and child into the fake array of keys and children
  // shift the keys to the right
  for (int indexFakeKeys = ORDER + 1; indexFakeKeys > correctIndex;
       indexFakeKeys--)
    fakeKeys[indexFakeKeys] = fakeKeys[indexFakeKeys - 1];

  // shift the children to the right
  for (int indexFakeChildren = ORDER + 2; indexFakeChildren > correctIndex + 1;
       indexFakeChildren--)
    fakeChildren[indexFakeChildren] = fakeChildren[indexFakeChildren - 1];

  // assign the new key and child to the fake array of keys and children
  fakeKeys[correctIndex] = key;
  fakeChildren[correctIndex + 1] = tempNode;

  newInternalNode->leaf = false;
  parentNode->size = (ORDER + 1) / 2;
  newInternalNode->size = ORDER - parentNode->size;

  for (int index = 0; index < newInternalNode->size + 1; index++) {
    if (index < newInternalNode->size) {
      newInternalNode->keys[index] = fakeKeys[index + parentNode->size + 1];
    }
    newInternalNode->children[index] =
        fakeChildren[index + parentNode->size + 1];
  }

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
    newRoot->keys[0] = parentNode->keys[parentNode->size];

    root = newRoot;
    return;
  }

  // insert recursively into the internal node
  insertIntoInternalNode(findParent(root, parentNode), newInternalNode,
                         parentNode->keys[parentNode->size]);
}

Node *BplusTree::findParent(Node *refNode, Node *node) {
  Node *parentResult;

  // if the reference node reached the leaf node
  if (refNode->leaf || (refNode->children[0])->leaf) {
    return nullptr;
  }

  // Traverse the current node with
  // all its child
  for (int index = 0; index < refNode->size + 1; index++) {

    // if refNode has a child which is the node
    if (refNode->children[index] == node) {
      return refNode;
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
  // bfs for B+ tree
  std::vector<int> result;
  std::queue<Node *> q;
  std::vector<Node *> visited;
  q.push(root);
  while (!q.empty()) {
    Node *temp = q.front();
    q.pop();

    if (std::find(visited.begin(), visited.end(), temp) == visited.end()) {
      for (int i = 0; i < temp->size; i++) {
        result.push_back(temp->keys[i]);
      }

      for (int i = 0; i < temp->size + 1; i++) {
        if (temp->children[i] != nullptr) {
          q.push(temp->children[i]);
        }
      }
      visited.push_back(temp);
    }
  }
  return result;
}
