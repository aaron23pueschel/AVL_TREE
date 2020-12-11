#include <string>
#include <string.h>
#include <iostream>
#include <stack>
using namespace std;



struct Item
{
    string name;
    int idNumber;
    int inStock;
    Item()
    {
        name = "";
        idNumber = 0;
        inStock = 0;
    }
    Item(string name_, int idNumber_, int inStock_)
    {
        name = name_;
        idNumber = idNumber_;
        inStock = inStock_;
    }
};



// --------- Tree Sub-Class ---------
class Tree
{
    // --- Node Sub-Sub-Class ---
    struct Node
    {
        Item classItem;
        Node* right = nullptr;
        Node* left = nullptr;
        int height = 0;
        int data = NULL;
        string stringData = "";
    };

public:
    // Operators
    friend bool operator<(string left, string right)
    {
        if (left.compare(right) < 0)
            return true;
        return false;
    }
    friend bool operator>(string left, string right)
    {
        if (left.compare(right) > 0)
            return true;
        return false;
    }
    friend bool operator==(string left, string right)
    {
        if (left.compare(right) == 0)
            return true;
        return false;
    }

    // Tree functions
    void insert(Item& newItem)
    {
        if (AVL_Root == nullptr)
        {
            Node* temp = new Node();
            temp->classItem = newItem;
            temp->data = newItem.idNumber;
            AVL_Root = temp;
            temp->height = 1;
            return;
        }
        insertRecursive(AVL_Root, newItem.idNumber, newItem);
    }
    void insert(string data, Item& item)
    {
        if (AVL_Root == nullptr)
        {
            Node* temp = new Node();
            temp->classItem = item;
            temp->stringData = data;
            AVL_Root = temp;
            temp->height = 1;
            return;
        }
        insertRecursive(AVL_Root, data, item);
    }

    // Choose:
    // 1. Ascending order
    // 2. Descending order
    // 3. Postorder
    // 4. Preorder
    void printTree(int choice)
    {
        cout << "Available inventory: " << endl;
        auto temp = AVL_Root;
        if (choice == 3)
        {
            printPostorder(temp);
        }
        else if (choice == 1) {
            printInorder(temp);
        }
        else if (choice == 2) {
            printReverseInorder(temp);
        }
        else if (choice == 4) {
            printPreorder(temp);
        }
        else
            cout << "Invalid option! " << endl;


    }
    Item* search(int key)   // Returns nullptr if not found, dereference pointer before using
    {
        auto temp = searchRecursive(AVL_Root, key);
        if (temp == nullptr)
            return nullptr;
        return &temp->classItem;
    }

    Item* search(string key)   // Returns nullptr if not found, dereference pointer before using
    {
        auto temp = searchRecursive(AVL_Root, key);
        if (temp == nullptr)
            return nullptr;
        return &temp->classItem;
    }
    bool delete_(int key)
    {
        auto val = searchRecursive(AVL_Root, key);
        auto predecessor = inorder_predecessor(val);
        if (val == nullptr)
            return false;
        if (isALeaf(val))
        {
            auto parentNode = parent(AVL_Root, val);
            if (height(val) == 1) {
                free(AVL_Root);
                AVL_Root = nullptr;
                return true;

            }
            else if (parentNode->left == val)
                parentNode->left = nullptr;
            else if (parentNode->right == val)
                parentNode->right = nullptr;
            free(val);


            return true;
        }
        else
        {
            deleteRecursive(AVL_Root, AVL_Root, predecessor, val, false);
            return true;
        }
    }
    bool delete_(string key)
    {
        auto val = searchRecursive(AVL_Root, key);
        auto predecessor = inorder_predecessor(val);
        if (val == nullptr)
            return false;
        if (isALeaf(val))
        {
            auto parentNode = parent(AVL_Root, val);
            if (height(val) == 1) {
                free(AVL_Root);
                AVL_Root = nullptr;
                return true;

            }
            else if (parentNode->left == val)
                parentNode->left = nullptr;
            else if (parentNode->right == val)
                parentNode->right = nullptr;
            free(val);

            return true;
        }
        else
        {
            deleteRecursive(AVL_Root, AVL_Root, predecessor, val, true);
            return true;
        }
    }

    bool edit(int id, int newAmount) {

        auto temp = searchRecursive(AVL_Root,id);
        if (temp != nullptr) {
            temp->classItem.inStock = newAmount;
            return true;
        }
        return false;
            
    }
    bool edit(string name, int newAmount) {

        auto temp = searchRecursive(AVL_Root, name);
        if (temp != nullptr) {
            temp->classItem.inStock = newAmount;
            return true;
        }
        return false;

    }



    bool deleteHead(bool is_string) {
        if (is_string)
            return delete_(headValue().name);
        return delete_(headValue().idNumber);
    }


    Item headValue()
    {
        return AVL_Root->classItem;
    }

    bool searchInorder(string key) {
        stack<Node*> temp;
        if (AVL_Root == nullptr)
            return false;
        searchInorderHelp(key, AVL_Root, temp);
        while (!temp.empty()) {
            if (temp.top()->stringData == key) {
                printItem(temp.top());
                return true;
            }

            temp.pop();
        }
        return false;

    }
    bool searchInorder(int key) {
        stack<Node*>temp;
        if (AVL_Root == nullptr)
            return false;
        searchInorderHelp(key, AVL_Root, temp);
        while (!temp.empty()) {
            if (temp.top()->data == key) {
                printItem(temp.top());
                return true;
            }

            temp.pop();
        }
        return false;
    }






private:

    // Tree structure
    Node* AVL_Root = nullptr;
    Node* rotateRightLeft(Node* node)
    {

        if (node == nullptr)
            return node;
        Node* rightChild = node->right;
        node->right = rightChild->left;
        rightChild->left = node->right->right;
        node->right->right = rightChild;

        auto newRightChild = node->right;
        node->right = newRightChild->left;
        newRightChild->left = node;

        if (node == AVL_Root)
            AVL_Root = newRightChild;
        newRightChild->left->height -= 2;
        newRightChild->height++;
        return newRightChild;
    }
    Node* rotateLeft(Node* node)
    {
        if (node == nullptr)
            return node;
        auto rightChild = node->right;
        node->right = node->right->left;
        rightChild->left = node;

        if (node == AVL_Root)
            AVL_Root = rightChild;
        rightChild->left->height -= 2;
        return rightChild;
    }
    Node* rotateRight(Node* node)
    {
        if (node == nullptr)
            return node;
        auto leftChild = node->left;
        node->left = node->left->right;
        leftChild->right = node;

        if (node == AVL_Root)
            AVL_Root = leftChild;
        leftChild->right->height -= 2;
        return leftChild;
    }
    Node* rotateLeftRight(Node* node)
    {
        if (node == nullptr)
            return node;
        Node* leftChild = node->left;
        node->left = leftChild->right;
        leftChild->right = node->left->left;
        node->left->left = leftChild;

        auto newLeftChild = node->left;
        node->left = newLeftChild->right;
        newLeftChild->right = node;

        if (node == AVL_Root)
            AVL_Root = newLeftChild;
        newLeftChild->right->height -= 2;
        newLeftChild->height++;
        return newLeftChild;
    }
    Node* insertRecursive(Node* root, int data, Item& item)
    {

        if (root == NULL)
        {
            Node* temp = new Node();
            temp->classItem = item;
            temp->data = data;
            temp->height = 1;
            return temp;
        }
        if (data < root->data)
            root->left = insertRecursive(root->left, data, item);
        else if (data > root->data)
            root->right = insertRecursive(root->right, data, item);

        root->height = max_height(root);
        root = balanceNodes(root);
        return root;
    }
    Node* insertRecursive(Node* root, string data, Item& item)
    {

        if (root == NULL)
        {
            Node* temp = new Node();
            temp->classItem = item;
            temp->stringData = data;
            temp->height = 1;
            return temp;
        }
        if (data < root->stringData)
            root->left = insertRecursive(root->left, data, item);
        else if (data > root->stringData)
            root->right = insertRecursive(root->right, data, item);

        root->height = max_height(root);
        root = balanceNodes(root);
        return root;
    }
    int max_height(Node* root)
    {
        if (root == nullptr)
            return 0;
        int heightLeft, heightRight;
        heightLeft = root->left == nullptr ? 0 : root->left->height;
        heightRight = root->right == nullptr ? 0 : root->right->height;

        return 1 + (heightLeft > heightRight ? heightLeft : heightRight);
    }
    int height(Node* root)
    {
        if (root == nullptr)
            return 0;
        return root->height;
    }
    Node* balanceNodes(Node* parent)
    {
        if (parent == nullptr)
            return parent;

        Node* temp = parent;

        if (height(parent->left) - height(parent->right) > 1)
        {
            if (parent->left != nullptr && height(parent->left->right) < height(parent->left->left))
                parent = rotateRight(parent);
            else if (parent->left != nullptr && height(parent->left->right) > height(parent->left->left))
                parent = rotateLeftRight(parent);
            else if (height(parent->right) == 0)
                parent = rotateRight(parent);
        }
        else if (height(parent->left) - height(parent->right) < -1)
        {
            if (parent->right != nullptr && height(parent->right->left) < height(parent->right->right))
                parent = rotateLeft(parent);
            else if (parent->right != nullptr && height(parent->right->left) > height(parent->right->right))
                parent = rotateRightLeft(parent);
            else if (height(parent->left) == 0)
                parent = rotateLeft(parent);
        }
        return parent;
    }



    void printItem(Node* item) {
        if (item != nullptr) {
            cout << "Name: " << item->classItem.name << endl;
            cout << "idNumber: " << item->classItem.idNumber << endl;
            cout << "Number of items: " << item->classItem.inStock << endl;
            cout << endl;
        }


    }
    void printPostorder(Node* root)
    {
        if (root == nullptr)
            return;

        printPostorder(root->left);
        printPostorder(root->right);
        printItem(root);
    }

    void printInorder(Node* root) {
        if (root == nullptr)
            return;

        printInorder(root->left);
        printItem(root);
        printInorder(root->right);


    }

    void printReverseInorder(Node* root) {
        if (root == nullptr)
            return;

        printReverseInorder(root->right);
        printItem(root);
        printReverseInorder(root->left);

    }

    void printPreorder(Node* root) {
        if (root == nullptr)
            return;

        printItem(root);
        printPreorder(root->left);
        printPreorder(root->right);

    }

    void searchInorderHelp(string key, Node* root, stack<Node*>& stack_) {
        if (root == nullptr)
            return;
        searchInorderHelp(key, root->left, stack_);
        if (root->stringData == key)
            stack_.push(root);
        searchInorderHelp(key, root->right, stack_);

    }
    void searchInorderHelp(int key, Node* root, stack<Node*>& stack_) {
        if (root == nullptr)
            return;
        searchInorderHelp(key, root->left, stack_);
        if (root->data == key)
            stack_.push(root);
        searchInorderHelp(key, root->right, stack_);

    }



    Node* searchRecursive(Node* root, int key)
    {
        if (root == nullptr)
            return nullptr;
        if (key == root->data)
            return root;
        else if (key < root->data)
            return searchRecursive(root->left, key);
        else if (key > root->data)
            return searchRecursive(root->right, key);
        return nullptr;
    }
    Node* searchRecursive(Node* root, string key)
    {
        if (root == nullptr)
            return nullptr;
        if (key == root->stringData)
            return root;
        else if (key < root->stringData)
            return searchRecursive(root->left, key);
        else if (key > root->stringData)
            return searchRecursive(root->right, key);
        return nullptr;
    }
    bool isALeaf(Node* node)
    {
        if (node == nullptr)
            return false;
        else if (node->left == nullptr && node->right == nullptr)
            return true;
        return false;
    }
    Node* deleteLeaf(Node* root, Node* parent)
    {

        if (root->data == parent->data)
        {
            root->height = max_height(root);
            root = balanceNodes(root);
            return root;
        }
        if (parent->data < root->data)
            root->left = deleteLeaf(root->left, parent);
        else if (parent->data > root->data)
            root->right = deleteLeaf(root->right, parent);

        root->height = max_height(root);
        root = balanceNodes(root);
    }
    Node* deleteRecursive(Node* root, Node* rootNonRecursive, Node*& inorder, Node* deleting, bool is_string)
    {
        if (inorder == nullptr) {
            auto parentSmallest = parent(AVL_Root, root);
            if (parentSmallest != nullptr) {
                parentSmallest->left = root->right;
                free(root);
                return nullptr;

            }
            else {
                AVL_Root = root->right;
                free(root);
                return nullptr;
            }
        }
        else if (root->data == inorder->data && !is_string)
        {
            auto temp = inorder->left;
            swap(deleting->data, inorder->data);
            swap(deleting->classItem, inorder->classItem);
            swap(deleting->stringData, inorder->stringData);
            free(inorder);
            if (temp == nullptr)
                return nullptr;
            return temp;
        }
        else if (root->stringData == inorder->stringData && is_string)
        {
            auto temp = inorder->left;
            swap(deleting->data, inorder->data);
            swap(deleting->classItem, inorder->classItem);
            swap(deleting->stringData, inorder->stringData);
            free(inorder);
            if (temp == nullptr)
                return nullptr;
            return temp;
        }
        else if (is_string) {
            if (inorder->stringData < root->stringData)
                root->left = deleteRecursive(root->left, rootNonRecursive, inorder, deleting, is_string);
            else if (inorder->stringData > root->stringData)
                root->right = deleteRecursive(root->right, rootNonRecursive, inorder, deleting, is_string);
        }
        else if (!is_string) {
            if (inorder->data < root->data)
                root->left = deleteRecursive(root->left, rootNonRecursive, inorder, deleting, is_string);
            else if (inorder->data > root->data)
                root->right = deleteRecursive(root->right, rootNonRecursive, inorder, deleting, is_string);
        }


        root->height = max_height(root);
        root = balanceNodes(root);
        return root;
    }
    Node* parent(Node* root, Node* key)
    {
        if (root == nullptr || key == AVL_Root)
            return nullptr;
        if (root->left == key)
            return root;
        else if (root->right == key)
            return root;
        if (root->data > key->data)
            return parent(root->left, key);
        if (root->data < key->data)
            return parent(root->right, key);
    }
    Node* inorder_predecessor(Node* root)
    {

        if (root == nullptr)
            return nullptr;
        if (root->left == nullptr)
            return nullptr;
        auto temp = root->left;
        while (temp->right != nullptr)
            temp = temp->right;
        return temp;
    }
};