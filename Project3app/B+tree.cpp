//written by Garrett McClay
// addding pragma once to make sure stuff doesn't break - Andrew

//#pragma once
#include <regex>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <string.h>
#include <algorithm>
#include <iterator>
#include <map>
/*
#ifdef _WIN32
int strcasecmp(const char* str1, const char* str2) {
    return strcmpi(str1, str2);
}
#endif
*/
using namespace std;
int MAX_SONGS_PER_NODE = 5;
int MIN_SONGS_PER_NODE = 1;

// Define your B+ tree node structure
struct BPlusTreeNode {
    vector<vector<string>> songs; // Vector to store songs
    vector<BPlusTreeNode*> children; // Vector to store child nodes
    BPlusTreeNode* next;
    bool isLeaf; // Flag to check if the node is a leaf
};

class BPlusTree {
private:
    BPlusTreeNode* root;
    // You might need additional private methods for insertion, deletion, searching, etc.

public:
    BPlusTree() {
        root = nullptr;
    }

    // Method to insert a song into the tree
    void insert(vector<string> song, int sortBy) {
        if (root == nullptr) {
            root = new BPlusTreeNode();
            root->songs.push_back(song);
            root->isLeaf = true;
        }
        else {
            insertHelper(root, song, sortBy);
        }
        //printTree();
    }

    // Helper function for insertion
    void insertHelper(BPlusTreeNode* node, const vector<string>& song, int sortBy) {
        //cout << "insert helper" << endl;
        if (node->children.empty()) {
            // Insert into leaf node
            // Find the appropriate position to maintain order
            int i = 0;
            auto iter = node->songs.begin();
            for (; iter != node->songs.end(); iter++) {
                if (node->songs[i][sortBy] > song[sortBy]) {
                    break;
                }
                i++;
            }
            node->songs.insert(iter, song);

            // Split the leaf node if necessary
            if (node->songs.size() >= MAX_SONGS_PER_NODE) {
                splitLeafNode(node, sortBy);
            }
        }
        else {
            // Find the child node to descend into
            int indexI;
            int indexJ;
            bool end = false;
            for (int i = 0; i < node->children.size(); i++) {
                for (int j = 0; j < node->children[i]->songs.size(); j++){
                    //if (node->children[i]->songs[j].title > song.title) {
                    if (strcasecmp(node->children[i]->songs[j][sortBy].c_str(), song[sortBy].c_str()) >= 0) {
                        end = true;
                        indexJ = j;
                        break;
                    }
                    indexJ = j;
                }
                if (end == true) {
                    indexI = i;
                    break;
                }
                indexI = i;
            }
            if (indexJ == 0 && node->children[indexI]->songs[indexJ][sortBy] > song[sortBy]) {
                if (indexI == 0) {
                    indexI++;
                }
                indexI--;
            }
            insertHelper(node->children[indexI], song, sortBy);
        }
    }

    // Function to split a leaf node
    void splitLeafNode(BPlusTreeNode* node, int sortBy) {
        // Create a new node for splitting
        BPlusTreeNode* newNode = new BPlusTreeNode();
        int mid = node->songs.size() / 2;

        // Move half of the songs to the new node
        newNode->songs.assign(node->songs.begin() + mid, node->songs.end());
        node->songs.erase(node->songs.begin() + mid, node->songs.end());

        // Update sibling pointers
        newNode->next = node->next;
        node->next = newNode;

        // Insert the middle key to the parent node
        vector<string>& middleSong = newNode->songs.front(); // Assuming keys are sorted
        insertIntoParent(node, middleSong, newNode, sortBy);
    }

    // Function to find the parent of a given node
    BPlusTreeNode* findParent(BPlusTreeNode* current, BPlusTreeNode* child) {

        if (current == nullptr || current->children.empty()) {
            // Base case: current node is null or a leaf node
            return nullptr;
        }

        // Check if any child of the current node is equal to the given child
        for (auto& node : current->children) {
            if (node == child) {
                // Return the current node if one of its children is the given child
                return current;
            }
            // Recursively search in the children nodes
            BPlusTreeNode* parent = findParent(node, child);
            if (parent != nullptr) {
                // If found in a child, return the parent
                return parent;
            }
        }

        // If the given child is not found in any children of the current node
        return nullptr;
    }

    // Function to split an internal node
    void splitInternalNode(BPlusTreeNode* node) {
        // Create a new node for splitting
        BPlusTreeNode* newNode = new BPlusTreeNode();

        // Find the index to split the keys
        int mid = node->songs.size() / 2;

        // Move half of the keys to the new node
        newNode->songs.assign(node->songs.begin() + mid + 1, node->songs.end());
        node->songs.erase(node->songs.begin() + mid, node->songs.end());

        // Move half of the children to the new node
        newNode->children.assign(node->children.begin() + mid + 1, node->children.end());
        //newNode->children.assign(node->children.begin(), node->children.end() - mid);
        node->children.erase(node->children.begin() + mid + 1, node->children.end());

        // Find the parent node
        BPlusTreeNode* parent = findParent(root, node);
        if (parent == nullptr) {
            // If the current node is the root, create a new root
            parent = new BPlusTreeNode();
            root = parent;
        }
        parent->children.push_back(node);
        // Insert the middle key to the parent node
        parent->songs.insert(parent->songs.begin() + distance(parent->children.begin(), find(parent->children.begin(), parent->children.end(), node)), node->songs[mid]);
        // Insert the new node to the parent's children
        parent->children.insert(parent->children.begin() + distance(parent->children.begin(), find(parent->children.begin(), parent->children.end(), node)) + 1, newNode);
        // If the parent node now has too many keys, split it recursively
        if (parent->songs.size() >= MAX_SONGS_PER_NODE) {
            splitInternalNode(parent);
        }
    }

    // Function to insert a key into the parent node
    void insertIntoParent(BPlusTreeNode* leftChild, const vector<string>& song, BPlusTreeNode* rightChild, int sortBy) {
        if (root == leftChild) {
            // Create a new root
            root = new BPlusTreeNode();
            root->songs.push_back(song);
            root->children.push_back(leftChild);
            root->children.push_back(rightChild);
            root->isLeaf = false;
        } else {
            // Find the parent node
            BPlusTreeNode* parent = findParent(root, leftChild);
            parent->songs.push_back(song);
            //using bubble sort for now should probably make more efficient later
            bool swapped;
            for (int i = 0; i < parent->songs.size() - 1; i++) {
                swapped = false;
                for (int j = 0; j < parent->songs.size() - i - 1; j++) {
                    //if (parent->songs[j].title > parent->songs[j + 1].title) {
                    if (strcasecmp(parent->songs[j][sortBy].c_str(), parent->songs[j + 1][sortBy].c_str()) > 0) {
                        swap(parent->songs[j], parent->songs[j + 1]);
                        swapped = true;
                    }
                }

                // If no two elements were swapped
                // by inner loop, then break
                if (swapped == false)
                    break;
            }
            auto iter = parent->songs.begin();
            int i = 0;
            for (; iter != parent->songs.end(); iter++) {
                //if (iter[sortBy] >= song[sortBy]) {
                if (strcasecmp(parent->songs[i][sortBy].c_str(), song[sortBy].c_str()) >= 0) {
                    break;
                }
                i++;
            }

            // Insert the right child pointer
            int index = distance(parent->songs.begin(), iter);
            parent->children.insert(parent->children.begin() + index + 1, rightChild);

            // Split the parent node if necessary
            if (parent->songs.size() >= MAX_SONGS_PER_NODE) {
                splitInternalNode(parent);
            }
        }
    }

    // Method to search for songs based on criteria like title, artist, etc.
    vector<vector<string>> search(const string& key) {
        vector<vector<string>> result;
        if (root != nullptr) {
            searchHelper(root, key, result);
        }
        cout << result.size() << endl;
        return result;
    }

    //I made it able to search for each attribute and for the instance of a title and album being the same
    //the best I was able to do was make it return both of them in the results

    // Helper function for search
    void searchHelper(BPlusTreeNode* node, const string& key, vector<vector<string>>& result) {

        if (node->children.empty()) {
            // Search for the key in the leaf node
            for (const auto& song : node->songs) {

                //if (song.title == key || song.artist == key || song.album == key || song.length == key || song.year == key) {
                if (strcasecmp(song[0].c_str(), key.c_str()) == 0 ||
                    strcasecmp(song[1].c_str(), key.c_str()) == 0 ||
                    strcasecmp(song[2].c_str(), key.c_str()) == 0 ||
                    strcasecmp(song[3].c_str(), key.c_str()) == 0 ||
                    strcasecmp(song[4].c_str(), key.c_str()) == 0) {
                    if (count(result.begin(), result.end(), song) == 0) {
                        result.push_back(song);
                    }
                }
            }
        } else {

            //awful time complexity but this is the only way I could get it to be able to
            //search for each different attribute
            //only other way I could think would require resorting the entire tree
            for (const auto& child : node->children) {
                searchHelper(child, key, result);
            }
        }
    }

    // Method to remove a song from the tree
    void remove(const vector<string>& song) {
        if (root != nullptr) {
            removeHelper(root, song);
        }
    }

    // Helper function for remove
    void removeHelper(BPlusTreeNode* node,const vector<string>& song) {
        if (node->isLeaf) {
            // Remove the song from the leaf node
            auto iter = node->songs.begin();
            int i = 0;
            for (; iter != node->songs.end(); iter++) {
                //if (song.title == iter->title) {
                if (strcasecmp(song[0].c_str(), node->songs[i][0].c_str()) == 0) {
                    return;
                }
                i++;
            }
            //auto it = find(node->songs.begin(), node->songs.end(), song);
            if (iter != node->songs.end()) {
                node->songs.erase(iter);
                // Handle underflow if necessary
                handleUnderflow(node);
            }
        } else {
            // Find the appropriate child node to descend into
            int i = 0;
            //while (i < node->songs.size() && song.title > node->songs[i].title) {
            while (i < node->songs.size() && strcasecmp(song[0].c_str(), node->songs[i][0].c_str()) > 0) {
                i++;
            }
            removeHelper(node->children[i], song);
        }
    }

    // Function to handle underflow after deletion
    void handleUnderflow(BPlusTreeNode* node) {
        if (node->songs.size() < MIN_SONGS_PER_NODE) {
            // Check if borrowing from sibling is possible
            BPlusTreeNode* leftSibling = findLeftSibling(node);
            BPlusTreeNode* rightSibling = findRightSibling(node);

            if (leftSibling && leftSibling->songs.size() > MIN_SONGS_PER_NODE) {
                // Borrow from left sibling
                borrowFromLeftSibling(node, leftSibling);
            } else if (rightSibling && rightSibling->songs.size() > MIN_SONGS_PER_NODE) {
                // Borrow from right sibling
                borrowFromRightSibling(node, rightSibling);
            } else {
                // Merge with a sibling
                if (leftSibling) {
                    mergeNodes(leftSibling, node);
                } else if (rightSibling) {
                    mergeNodes(node, rightSibling);
                }
            }
        }
    }

    // Function to find the left sibling of a node
    BPlusTreeNode* findLeftSibling(BPlusTreeNode* node) {
        // Find the parent node
        BPlusTreeNode* parent = findParent(root, node);
        if (!parent) return nullptr;

        // Find the index of the current node in the parent's children
        auto it = find(parent->children.begin(), parent->children.end(), node);
        if (it == parent->children.begin()) {
            // No left sibling
            return nullptr;
        } else {
            // Return the left sibling
            return *(it - 1);
        }
    }

    // Function to find the right sibling of a node
    BPlusTreeNode* findRightSibling(BPlusTreeNode* node) {
        // Find the parent node
        BPlusTreeNode* parent = findParent(root, node);
        if (!parent) return nullptr;

        // Find the index of the current node in the parent's children
        auto it = find(parent->children.begin(), parent->children.end(), node);
        if (it == parent->children.end() - 1) {
            // No right sibling
            return nullptr;
        } else {
            // Return the right sibling
            return *(it + 1);
        }
    }

    // Function to borrow a song from the left sibling
    void borrowFromLeftSibling(BPlusTreeNode* node, BPlusTreeNode* leftSibling) {
        // Borrow the last song from the left sibling
        vector<string> borrowedSong = leftSibling->songs.back();
        leftSibling->songs.pop_back();

        // Insert the borrowed song into the node
        node->songs.insert(node->songs.begin(), borrowedSong);

        // Update the parent's key
        BPlusTreeNode* parent = findParent(root, node);
        int index = distance(parent->children.begin(), find(parent->children.begin(), parent->children.end(), node));
        parent->songs[index] = borrowedSong;
    }

    // Function to borrow a song from the right sibling
    void borrowFromRightSibling(BPlusTreeNode* node, BPlusTreeNode* rightSibling) {
        // Borrow the first song from the right sibling
        vector<string> borrowedSong = rightSibling->songs.front();
        rightSibling->songs.erase(rightSibling->songs.begin());

        // Insert the borrowed song into the node
        node->songs.push_back(borrowedSong);

        // Update the parent's key
        BPlusTreeNode* parent = findParent(root, node);
        int index = distance(parent->children.begin(), find(parent->children.begin(), parent->children.end(), rightSibling));
        parent->songs[index - 1] = rightSibling->songs.front();
    }

    // Function to merge two nodes
    void mergeNodes(BPlusTreeNode* leftNode, BPlusTreeNode* rightNode) {
        // Move all songs from the right node to the left node
        leftNode->songs.insert(leftNode->songs.end(), rightNode->songs.begin(), rightNode->songs.end());

        // Update sibling pointers if merging leaf nodes
        if (leftNode->isLeaf) {
            leftNode->next = rightNode->next;
        }

        // Remove the right node from the parent's children
        BPlusTreeNode* parent = findParent(root, leftNode);
        auto it = find(parent->children.begin(), parent->children.end(), rightNode);
        parent->children.erase(it);

        // Delete the right node
        delete rightNode;

        // Handle underflow in the parent node
        handleUnderflow(parent);
    }

    // Method to print the tree (for debugging purposes)
    void printTree() {
        if (root != nullptr) {
            printHelper(root, 0);
        }
    }

    // Helper function for printing the tree
    void printHelper(BPlusTreeNode* node, int level) {
        cout << "Level " << level << ": ";
        if (node->children.empty()) {
            cout << "Leaf Node - ";
            for (const auto& song : node->songs) {
                cout << song[0] << " " << song[2] << ", ";
            }
        }

        else if(node == root) {
            cout << "Root Node - ";
            for (const auto& song : node->songs) {
                cout << song[0] << " " << song[2] << ", ";
            }
            cout << endl;
            // Recursively print child nodes
            for (const auto& child : node->children) {
                printHelper(child, level + 1);
            }
        }

        else {
            cout << "Internal Node - ";
            for (const auto& song : node->songs) {
                cout << song[0] << " " << song[2] << ", ";
            }
            cout << endl;
            // Recursively print child nodes
            for (const auto& child : node->children) {
                printHelper(child, level + 1);
            }
        }
        cout << endl;
    }

    vector<vector<string>> leastToGreatest() {
        vector<vector<string>> result;
        if (root != nullptr) {
            leastToGreatestHelper(root, result);
        }
        return result;
    }

    void leastToGreatestHelper(BPlusTreeNode* node, vector<vector<string>>& result) {
        if (node->children.empty()) {
            for (const auto& song : node->songs) {
                result.push_back(song);
            }
        }
        // Recursively print child nodes
        else {
            for (const auto& child : node->children) {
                leastToGreatestHelper(child, result);
            }
        }
    }

    void removeAll() {
        removeAllHelper(root);
    }

    void removeAllHelper(BPlusTreeNode* node) {
        if (node->children.empty()) {
            delete node;
            return;
        }
        // Recursively print child nodes
        else {
            for (const auto& child : node->children) {
                removeAllHelper(child);
            }
        }
        delete node;
    }
};
/*
int main() {
    BPlusTree tree;

    vector<string> song1 = {"Bomb", "Artist1", "A", "2000", "1992"};
    vector<string> song2 = {"Song", "Artist2", "B", "2005", "1995"};
    vector<string> song3 = {"Apple", "Artist3", "C", "1354", "2010"};
    vector<string> song0 = {"Write", "Artist0", "D", "2019", "2011"};
    vector<string> song4 = {"Dongo", "Artist4", "E", "1258", "2010"};
    vector<string> song5 = {"Join", "Artist5", "P", "1995", "2010"};
    vector<string> song6 = {"Us", "Artist6", "F", "2015", "2010"};
    vector<string> song7 = {"Human", "Artist7", "G", "3111510", "2010"};

    vector<string> song8 = {"Sunshine", "Artist8", "H", "180", "2012"};
    vector<string> song9 = {"Moonlight", "Artist9", "I", "240", "2013"};
    vector<string> song10 = {"Stars", "Artist10", "J", "210", "2014"};
    vector<string> song11 = {"Galaxy", "Artist11", "K", "300", "2015"};
    vector<string> song12 = {"Ocean", "Artist2", "L", "2010", "2016"};
    vector<string> song13 = {"Mountain", "Artist13", "M", "320", "2017"};
    vector<string> song14 = {"Forest", "Artist14", "N", "280", "2018"};
    vector<string> song15 = {"Dongo", "Artist15", "O", "340", "2019"};

    tree.insert(song1, 2);
    tree.insert(song2, 2);
    tree.insert(song3, 2);
    tree.insert(song0, 2);
    tree.insert(song4, 2);
    tree.insert(song5, 2);
    tree.insert(song6, 2);
    tree.insert(song7, 2);
    //tree.printTree();
    tree.insert(song8, 2);
    tree.insert(song9, 2);
    tree.insert(song10, 2);
    tree.insert(song11, 2);
    tree.insert(song12, 2);
    tree.insert(song13, 2);
    tree.insert(song14, 2);
    tree.insert(song15, 2);
    tree.printTree();
    cout << "split" << endl;
    //tree.removeAll();
    delete tree;


    tree.printTree();
    cout << "split" << endl;
    tree.insert(song1, 0);
    tree.insert(song2, 0);
    tree.insert(song3, 0);
    tree.insert(song0, 0);
    tree.insert(song4, 0);
    tree.insert(song5, 0);
    tree.insert(song6, 0);
    tree.insert(song7, 0);
    //tree.printTree();
    tree.insert(song8, 0);
    tree.insert(song9, 0);
    tree.insert(song10, 0);
    tree.insert(song11, 0);
    tree.insert(song12, 0);
    tree.insert(song13, 0);
    tree.insert(song14, 0);
    tree.insert(song15, 0);

    tree.printTree();
/*
    vector<vector<string>> songs = {
            {"Testify", "The Battle Of Los Angeles", "Rage Against The Machine", "210133", "1999"},
            {"Guerrilla Radio", "The Battle Of Los Angeles", "Rage Against The Machine", "206200", "1999"},
            {"Calm Like a Bomb", "The Battle Of Los Angeles", "Rage Against The Machine", "298893", "1999"},
            {"Mic Check", "The Battle Of Los Angeles", "Rage Against The Machine", "213640", "1999"},
            {"Sleep Now In the Fire", "The Battle Of Los Angeles", "Rage Against The Machine", "205600", "1999"},
            {"Born of a Broken Man", "The Battle Of Los Angeles", "Rage Against The Machine", "280960", "1999"},
            {"Born As Ghosts", "The Battle Of Los Angeles", "Rage Against The Machine", "202040", "1999"},
            {"Maria", "The Battle Of Los Angeles", "Rage Against The Machine", "228093", "1999"},
            {"Voice of the Voiceless", "The Battle Of Los Angeles", "Rage Against The Machine", "151573", "1999"},
            {"New Millennium Homes", "The Battle Of Los Angeles", "Rage Against The Machine", "224933", "1999"},
            {"Ashes In the Fall", "The Battle Of Los Angeles", "Rage Against The Machine", "277267", "1999"},
            {"War Within a Breath", "The Battle Of Los Angeles", "Rage Against The Machine", "216427", "1999"},
    };

    for (auto song : songs) {
        cout << song[0] << endl;
        tree.insert(song, 0);
    }

    cout << "doesnt make it" << endl;
    vector<vector<string>> result;
    result = tree.leastToGreatest();
    cout << result.size() << endl;
    for (int i = 0; i < result.size(); i++) {
        cout << result[i][0] << " " << result[i][2] << endl;
    }

    return 0;
}
*/