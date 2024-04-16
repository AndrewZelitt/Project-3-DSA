//this is where the AVL tree code will reside
#ifndef AvlTree_cpp
#define AvlTree_cpp
#pragma once
#include <iostream>
#include <regex>
#include <string>
#include <string.h>
#include <cstring>

#include "AVLtree.H"
using namespace std;



TreeNode* AVLtree::insert(TreeNode* root, int pos, int arg, int arg1) {
    // checking if the root is null
    if (root == nullptr) {
        //cout << "successful\n";
        this->num_items++;
        return new TreeNode(pos);
    }
    switch (arg1) {
    case 0:
    {
        // adding the new song into the list based on its decided value
        if (strcasecmp(this->raw_database->at(pos)[arg], this->raw_database->at(root->position_in_vector)[arg]) < 0){
       //if ((string)this->raw_database->at(pos)[arg] < (string)this->raw_database->at(root->position_in_vector)[arg]) {
            // changing the base root of the tree when necessary
            if (root == this->root) {
                this->root->left = insert(root->left, pos, arg, arg1);
            }
            else {
                root->left = insert(root->left, pos, arg, arg1);
            }
        }
        else  if ( strcasecmp(this->raw_database->at(pos)[arg], this->raw_database->at(root->position_in_vector)[arg]) > 0){
       // if ((string)this->raw_database->at(pos)[arg] > (string)this->raw_database->at(root->position_in_vector)[arg]) {
            // changing the base root of the tree when necessary
            if (root == this->root) {
                this->root->right = insert(root->right, pos, arg, arg1);
            }
            else {
                root->right = insert(root->right, pos, arg, arg1);
            }
        }
        else  if ( strcasecmp(this->raw_database->at(pos)[arg], this->raw_database->at(root->position_in_vector)[arg]) == 0){
        //if ((string)this->raw_database->at(pos)[arg] == (string)this->raw_database->at(root->position_in_vector)[arg]) {
            root->dupes.push_back(new TreeNode(pos));

        }
        break;
    }
    case 1:
        // adding the new song into the list based on its decided value
        if ( strcasecmp(this->raw_database->at(pos)[arg], this->raw_database->at(root->position_in_vector)[arg]) > 0){
        //if ((string)this->raw_database->at(pos)[arg] > (string)this->raw_database->at(root->position_in_vector)[arg]) {
            // changing the base root of the tree when necessary
            if (root == this->root) {
                this->root->left = insert(root->left, pos, arg, arg1);
            }
            else {
                root->left = insert(root->left, pos, arg, arg1);
            }
        }
        else  if ( strcasecmp(this->raw_database->at(pos)[arg], this->raw_database->at(root->position_in_vector)[arg]) < 0){
        //if ((string)this->raw_database->at(pos)[arg] < (string)this->raw_database->at(root->position_in_vector)[arg]) {
            // changing the base root of the tree when necessary
            if (root == this->root) {
                this->root->right = insert(root->right, pos, arg, arg1);
            }
            else {
                root->right = insert(root->right, pos, arg, arg1);
            }
        }else  if ( strcasecmp(this->raw_database->at(pos)[arg], this->raw_database->at(root->position_in_vector)[arg]) == 0){
        //if ((string)this->raw_database->at(pos)[arg] == (string)this->raw_database->at(root->position_in_vector)[arg]) {
            root->dupes.push_back(new TreeNode(pos));
        }
        break;
    }
    
    // updates the height and balances the tree
        find_height(root);
    {
        int height_left = root->left == 0 ? 0 : root->left->height;
        int height_right = root->right == 0 ? 0 : root->right->height;
        int height_right_right = 0;
        int height_right_left = 0;
        int height_left_right = 0;
        int height_left_left = 0;
        if (height_right - height_left >= 2) {
            if (root->right != 0) {
                height_right_right =
                    root->right->right == 0 ? 0 : root->right->right->height;
                height_right_left =
                    root->right->left == 0 ? 0 : root->right->left->height;
            }
            if (height_right_right - height_right_left <= -1) {
                // do right left rotation
                // changing the base root of the tree when necessary
                if (root == this->root) {
                    this->root = right_left_balance(root);
                }
                else {
                    root = right_left_balance(root);
                }

            }
            else if (height_right_right - height_right_left >= 1) {
                // do left rotation
                // changing the base root of the tree when necessary
                if (root == this->root) {
                    this->root = left_balance(root);
                }
                else {
                    root = left_balance(root);
                }
            }
        }
        else if (height_right - height_left <= -2) {
            if (root->left != 0) {
                height_left_right =
                    root->left->right == 0 ? 0 : root->left->right->height;
                height_left_left = root->left->left == 0 ? 0 : root->left->left->height;
            }
            if (height_left_right - height_left_left >= 1) {
                // do left right rotation
                // changing the base root of the tree when necessary
                if (root == this->root) {
                    this->root = left_right_balance(root);
                }
                else {
                    root = left_right_balance(root);
                }
            }
            else if (height_left_right - height_left_left <= -1) {
                // do right rotation
                // changing the base root of the tree when necessary
                if (root == this->root) {
                    this->root = right_balance(root);
                }
                else {
                    root = right_balance(root);
                }
            }
        }
    }
    return root;
};

TreeNode* AVLtree::right_balance(TreeNode* root) {
    // right rotation
    TreeNode* grandchild = root->left->right;
    TreeNode* newParent = root->left;
    newParent->right = root;
    root->left = grandchild;
    return newParent;
}

/*
void AVLtree::search_IDInsert(TreeNode* root, string id) {
    if (root != NULL) {
        search_IDInsert(root->left, id);
        if (root->id == id) {
            this->found = 1;
            return;
        }
        search_IDInsert(root->right, id);
    }
    return;
}
*/
void AVLtree::search(TreeNode* root, string input, int arg, int arg1) {
    // based on the input value it will search for the node with the correct type
    // either ID or name
    if (arg >= 3) {
        searchID(root, input, arg, arg1);
    }
    else {
        searchn(root, input, arg);
    }
}

// searching for a specific name, if there are multiple IDs with the same name
// it will print all of them.
void AVLtree::searchn(TreeNode* root, string name, int arg) {
    if (root == nullptr) {
        return;
    }
    if (root == this->root) {
        this->maxid = max_id(root);
    }
    searchn(root->left, name, arg);
    if ( strcasecmp(name.c_str(), this->raw_database->at(root->position_in_vector)[arg]) == 0){
    //if (name == (string)this->raw_database->at(root->position_in_vector)[arg]) {
        this->output_for_search.push_back(root->position_in_vector);
        for (TreeNode* duplicates : root->dupes) {
            if ( strcasecmp(name.c_str(), this->raw_database->at(duplicates->position_in_vector)[arg]) == 0) {
            //if (name == (string)this->raw_database->at(duplicates->position_in_vector)[arg]) {
                this->output_for_search.push_back(duplicates->position_in_vector);
            }
        }
        this->found = 1;
    }
    else {
        for (TreeNode* duplicates : root->dupes) {
            if ( strcasecmp(name.c_str(), this->raw_database->at(duplicates->position_in_vector)[arg]) == 0) {
            //if (name == (string)this->raw_database->at(duplicates->position_in_vector)[arg]) {
                this->output_for_search.push_back(duplicates->position_in_vector);
                this->found = 1;
            }
        }
    }
    searchn(root->right, name, arg);
    if (root->position_in_vector == this->maxid) {
        for (TreeNode* duplicates : root->dupes) {
            if ( strcasecmp(name.c_str(), this->raw_database->at(duplicates->position_in_vector)[arg]) == 0) {
            //if (name == (string)this->raw_database->at(duplicates->position_in_vector)[arg]) {
                this->output_for_search.push_back(duplicates->position_in_vector);
                this->found = 1;
            }
        }
        if (this->found == 0) {
            this->found = 0;
            return;
        }
    }
}

// searching for a specific number value
void AVLtree::searchID(TreeNode* root, string input, int arg, int arg1) {

    if (root != NULL) {
        if (input == (string)this->raw_database->at(root->position_in_vector)[arg]) {
            this->output_for_search.push_back(root->position_in_vector);
            for (TreeNode* duplicates : root->dupes) {
                searchID(duplicates, input, arg, arg1);
                //this->output_for_search.push_back(duplicates->position_in_vector);
            }
        }
        else {
            if (arg1) {
                //ascending
                if (input < (string)this->raw_database->at(root->position_in_vector)[arg]) {
                    searchID(root->left, input, arg, arg1);
                }
                else {
                    searchID(root->right, input, arg, arg1);
                }
            }
            else {
                //descending
                if ((string)this->raw_database->at(root->position_in_vector)[arg] > input) {
                    searchID(root->left, input, arg, arg1);
                }
                else {
                    searchID(root->right, input, arg, arg1);
                }
            }
        }
    }
    else {
        return;
        // cout << "unsuccessful\n";
    }
}


void AVLtree::printLevelCount(void) {
    // prints the height of the tree
    int height = 0;
    if (this->root == nullptr) {
        cout << height << "\n";
    }
    else {
        this->root->height = find_height(this->root);
        height = this->root->height;
    }
    cout << height << "\n";
}

TreeNode* AVLtree::left_balance(TreeNode* root) {
    // left rotation
    TreeNode* grandchild = root->right->left;
    TreeNode* newParent = root->right;
    newParent->left = root;
    root->right = grandchild;
    return newParent;
}

TreeNode* AVLtree::right_left_balance(TreeNode* root) {
    // right left rotation
    TreeNode* node = root;
    TreeNode* grandchild1 = node->right->left->left;
    TreeNode* grandchild2 = node->right->left->right;
    TreeNode* newParent = node->right->left;
    newParent->left = node;
    newParent->right = node->right;
    node->right->left = grandchild2;
    node->right = grandchild1;
    return newParent;
}

TreeNode* AVLtree::left_right_balance(TreeNode* root) {
    // left right rotation
    TreeNode* node = root;
    TreeNode* grandchild1 = node->left->right->left;
    TreeNode* grandchild2 = node->left->right->right;
    TreeNode* newParent = node->left->right;
    newParent->right = node;
    newParent->left = node->left;
    node->left->right = grandchild1;
    node->left = grandchild2;
    return newParent;
}

// using the boolean value to remove the final comma
// was made by user Andrew Semashev on stack overflow
// https://stackoverflow.com/a/68040568
void AVLtree::inorder(TreeNode* root) {
    bool first = true;
    inorder(root, first);
    //cout << "\n";
}

void AVLtree::preorder(TreeNode* root) {
    bool first = true;
    preorder(root, first);
    //cout << "\n";
}

void AVLtree::postorder(TreeNode* root) {
    bool first = true;
    postorder(root, first);
    //cout << "\n";
}

// inorder output of names
void AVLtree::inorder(TreeNode* root, bool& first) {
    if (root == nullptr) {
        cout << "";
    }
    else {
        inorder(root->left, first);
        this->output_for_search.push_back(root->position_in_vector);
        for (TreeNode* duplicates : root->dupes) {
            this->output_for_search.push_back(duplicates->position_in_vector);
        }
        inorder(root->right, first);
    }
}

// postorder output of names;
void AVLtree::postorder(TreeNode* root, bool& first) {
    if (root == nullptr) {
        cout << "";
    }

    if (root->left != nullptr) {
        postorder(root->left, first);
    }
    if (root->right != nullptr) {
        postorder(root->right, first);
    }
    if (first)
        first = false;
    else
    this->output_for_search.push_back(root->position_in_vector);
    for (TreeNode* duplicates : root->dupes) {
        this->output_for_search.push_back(duplicates->position_in_vector);
    }
}

// preorder output of names
void AVLtree::preorder(TreeNode* root, bool& first) {
    if (root == nullptr) {
        cout << "";
    }
    this->output_for_search.push_back(root->position_in_vector);
    for (TreeNode* duplicates : root->dupes) {
        this->output_for_search.push_back(duplicates->position_in_vector);
    }
    if (root->left != nullptr) {
        preorder(root->left, first);
    }
    if (root->right != nullptr) {
        preorder(root->right, first);
    }
}

// finds the height of a given node
int AVLtree::find_height(TreeNode* root) {
    int h = 0;
    if (root != NULL) {
        h = 1 + max(root->left == 0 ? 0 : find_height(root->left),
            root->right == 0 ? 0 : find_height(root->right));
        root->height = h;
    }

    return h;
    /*
    //trying possibly faster function of finding heights
    queue<TreeNode*> nodesInLevel;
    int height = 0;
    int nodeCount = 0;
    TreeNode* currentNode;
    if (root == NULL) {
        return 0;
    }
    nodesInLevel.push(root);
    while (!nodesInLevel.empty()) {
        height++;
        nodeCount = nodesInLevel.size();
        while (nodeCount--) {
            currentNode = nodesInLevel.front();

            // Check if the current nodes has left child and
            // insert it in the queue.

            if (currentNode->left != NULL) {
                nodesInLevel.push(currentNode->left);
            }

            // Check if the current nodes has right child
            // and insert it in the queue.
            if (currentNode->right != NULL) {
                nodesInLevel.push(currentNode->right);
            }

            // Once the children of the current node are
            // inserted. Delete the current node.

            nodesInLevel.pop();
        }
    }
    return height;
    */
};



// finds the farthest right value used with the searchname function
int AVLtree::max_id(TreeNode* root) {
    if (root->right == NULL) {
        return root->position_in_vector;
    }
    else {
         max_id(root->right);
    }
    return  root->position_in_vector;
}
/*
// removes a node with a given ID
TreeNode* AVLtree::removeID(TreeNode* root, string id) {
    if (id.length() != 8 || regex_search(id, regex("([A-Za-z\\s])+"))) {
        cout << "unsuccessful\n";
        return root;
    }
    if (root == nullptr) {
        if (this->num_items == 0) {
            cout << "successful\n";
            return root;
        }
        cout << "unsuccessful\n";
        return root;
    }
    if (id < root->id) {
        root->left = removeID(root->left, id);
        return root;
    }
    else if (id > root->id) {
        root->right = removeID(root->right, id);
        return root;
    }
    if (root->left == nullptr && root->right == nullptr) {
        // has no children
        this->num_items--;
        cout << "successful\n";
        return nullptr;
    }
    if (root->left == nullptr || root->right == nullptr) {
        // has 1 root
        this->num_items--;
        cout << "successful\n";
        return root->left != nullptr ? root->left : root->right;
    }
    else {
        // has 2 children
        TreeNode* parent = root;
        TreeNode* successor = root->right;
        while (successor->left != nullptr) {
            parent = successor;
            successor = successor->left;
        }
        parent != root ? parent->left = successor->right
            : parent->right = successor->right;
        root->id = successor->id;
        root->name = successor->name;
        delete successor;
        this->num_items--;
        cout << "successful\n";
        return root;
    }
}

// removes the Nth Node from the tree based on an inorder search
void AVLtree::removeInorder(TreeNode* root, int num) {
    this->deleting = nullptr;
    if (this->num_items <= num) {
        cout << "unsuccessful\n";
        return;
    }
    if (num == 0 && root->left == nullptr) {
        this->deleting = root;
    }
    else {
        findNthnode(root, num + 1);
        if (this->deleting == nullptr) {
            cout << "unsuccessful\n";
            return;
        }
    }
    this->root = removeID(root, this->deleting->id);
    this->count = 0;
}
*/

// prints out the Nth node based on an inorder search
void AVLtree::findNthnode(TreeNode* root, int num) {
    if (root == nullptr) {
        return;
    }
    if (this->count <= num) {
        findNthnode(root->left, num);
        this->count++;
        if (this->count == num) {
            this->deleting = root;
        }
        findNthnode(root->right, num);
    }
    return;
}

void AVLtree::deleteAll(TreeNode* root) {
    if (root == nullptr) {
        cout << "";
    }

    if (root->left != nullptr) {
        deleteAll(root->left);
        if (root->left->left != nullptr)
        delete root->left->left;
    }
    if (root->right != nullptr) {
        deleteAll(root->right);
        if (root->right->right != nullptr) {
            delete root->right->right;
        }
    }
    root->dupes.clear();
    
}


#endif
