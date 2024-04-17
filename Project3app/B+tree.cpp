//written by Garrett McClay
// addding pragma once to make sure stuff doesn't break - Andrew
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;
int MAX_SONGS_PER_NODE = 5;
int MIN_SONGS_PER_NODE = 1;

// Define your Song struct
struct Song {
    string title;
    string artist;
    string album;
    string length;
    string year;
    // You can add more attributes as needed
};

// Define your B+ tree node structure
struct BPlusTreeNode {
    vector<Song> songs; // Vector to store songs
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
    void insert(const Song& song) {
        cout << "begin inserting" << endl;
        if (root == nullptr) {
            cout << "root is null?" << endl;
            root = new BPlusTreeNode();
            root->songs.push_back(song);

            root->isLeaf = true;
        }
        else {
            cout << "good so far" << endl;
            insertHelper(root, song);
            cout << "ok done" << endl;
        }
        cout << "should be done now" << endl;
    }

    // Helper function for insertion
    void insertHelper(BPlusTreeNode* node, const Song& song) {
        if (node->children.empty()) {
            cout << "made it in" << endl;
            // Insert into leaf node
            // Find the appropriate position to maintain order
            auto it = lower_bound(node->songs.begin(), node->songs.end(), song, [](const Song& a, const Song& b) {
                return a.title < b.title; // Example: Insert based on title
            });
            node->songs.insert(it, song);

            for (int i = 0; i < node->songs.size(); i++) {
                cout << node->songs[i].title << endl;
            }

            // Split the leaf node if necessary
            if (node->songs.size() >= MAX_SONGS_PER_NODE) {
                cout << "goes in here" << endl;
                splitLeafNode(node);
            }
        }
        else {
            // Find the child node to descend into
            int i = 0;
            int j;
            bool end = false;
            while (i < node->children.size()) {
                j = 0;
                while (j < node->children[i]->songs.size()){
                    if (node->children[i]->songs[j].title > song.title) {
                        cout << "end loops" << endl;
                        cout << node->children[i]->songs[j].title << endl;
                        end = true;
                        break;
                    }
                    j++;
                }
                if (end == true) {
                    break;
                }
                i++;
            }
            if (j == 0 && node->children[i]->songs[j].title > song.title) {
                i--;
            }

            cout << "recursion" << endl;
            insertHelper(node->children[i], song);
            cout << "out" << endl;
        }
    }

    // Function to split a leaf node
    void splitLeafNode(BPlusTreeNode* node) {
        // Create a new node for splitting
        BPlusTreeNode* newNode = new BPlusTreeNode();
        int mid = node->songs.size() / 2;
        cout << "slit leaf node" << endl;


        // Move half of the songs to the new node
        newNode->songs.assign(node->songs.begin() + mid, node->songs.end());
        node->songs.erase(node->songs.begin() + mid, node->songs.end());

        for (int i = 0; i < node->songs.size();i++) {
            //cout << node->songs[i].title << endl;
        }

        // Update sibling pointers
        newNode->next = node->next;
        node->next = newNode;

        for (int i = 0; i < node->next->songs.size();i++) {
            //cout << node->next->songs[i].title << endl;
        }

        // Insert the middle key to the parent node
        Song middleSong = newNode->songs.front(); // Assuming keys are sorted
        cout << middleSong.title << endl;
        insertIntoParent(node, middleSong, newNode);

        cout << "finished splitting leaf node" << endl;
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

        cout << "split internal node" << endl;
        for (int i = 0; i < node->songs.size(); i++) {
            cout << node->songs[i].title << endl;
        }

        // Find the index to split the keys
        int mid = node->songs.size() / 2;
        cout << node->songs[mid].title << endl;

        // Move half of the keys to the new node
        newNode->songs.assign(node->songs.begin() + mid + 1, node->songs.end());
        node->songs.erase(node->songs.begin() + mid, node->songs.end());
        cout << "making new node" << endl;
        for (int i = 0; i < newNode->songs.size(); i++) {
            cout << newNode->songs[i].title << endl;
        }

        // Move half of the children to the new node
        newNode->children.assign(node->children.begin() + mid + 1, node->children.end());
        //newNode->children.assign(node->children.begin(), node->children.end() - mid);
        node->children.erase(node->children.begin() + mid + 1, node->children.end());

        // Find the parent node
        BPlusTreeNode* parent = findParent(root, node);
        cout << "again" << endl;
        if (parent == nullptr) {
            // If the current node is the root, create a new root
            parent = new BPlusTreeNode();
            root = parent;
        }
        cout << newNode->children.size() << endl;
        parent->children.push_back(node);
        // Insert the middle key to the parent node
        parent->songs.insert(parent->songs.begin() + distance(parent->children.begin(), find(parent->children.begin(), parent->children.end(), node)), node->songs[mid]);
        // Insert the new node to the parent's children
        cout << parent->children.size() << endl;
        parent->children.insert(parent->children.begin() + distance(parent->children.begin(), find(parent->children.begin(), parent->children.end(), node)) + 1, newNode);
        cout << "added the children in" << endl;
        // If the parent node now has too many keys, split it recursively
        if (parent->songs.size() >= MAX_SONGS_PER_NODE) {
            splitInternalNode(parent);
        }
    }


    // Function to insert a key into the parent node
    void insertIntoParent(BPlusTreeNode* leftChild, const Song& song, BPlusTreeNode* rightChild) {
        cout << "insertintoparent" << endl;
        if (root == leftChild) {
            // Create a new root
            cout << "make a new root" << endl;
            root = new BPlusTreeNode();
            root->songs.push_back(song);
            root->children.push_back(leftChild);
            root->children.push_back(rightChild);
            root->isLeaf = false;
        } else {
            // Find the parent node
            BPlusTreeNode* parent = findParent(root, leftChild);
            cout << song.title << endl;
            parent->songs.push_back(song);
            //using bubble sort for now should probably make more efficient later
            bool swapped;
            for (int i = 0; i < parent->songs.size() - 1; i++) {
                swapped = false;
                for (int j = 0; j < parent->songs.size() - i - 1; j++) {
                    if (parent->songs[j].title > parent->songs[j + 1].title) {
                        swap(parent->songs[j], parent->songs[j + 1]);
                        swapped = true;
                    }
                }

                // If no two elements were swapped
                // by inner loop, then break
                if (swapped == false)
                    break;
            }
            int i = 0;
            auto iter = parent->songs.begin();
            for (; iter != parent->songs.end(); iter++) {
                //cout << iter->title << song.title << endl;
                if (iter->title >= song.title) {
                    break;
                }
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
    vector<Song> search(const string& key) {
        vector<Song> result;
        cout << "search" << endl;
        if (root != nullptr) {
            searchHelper(root, key, result);
        }
        cout << "printing result" << endl;
        return result;
    }

    //I made it able to search for each attribute and for the instance of a title and album being the same
    //the best I was able to do was make it return both of them in the results

    // Helper function for search
    void searchHelper(BPlusTreeNode* node, const string& key, vector<Song>& result) {
        cout << "search helper" << endl;

        if (node->children.empty()) {
            // Search for the key in the leaf node
            for (const auto& song : node->songs) {
                if (song.title == key || song.artist == key || song.album == key || song.length == key || song.year == key) {
                    cout << "found one" << endl;
                    result.push_back(song);
                }
            }
        } else {
            // Descend to the appropriate child node
            /*
            int i = 0;

            while (i < node->songs.size() && key >= node->songs[i].title) {
                i++;
            }


            cout << "recursion" << endl;
            cout << i << endl;
            searchHelper(node->children[i], key, result);
            */

            //awful time complexity but this is the only way I could get it to be able to
            //search for each different attribute
            //only other way I could think would require resorting the entire tree
            for (const auto& child : node->children) {
                searchHelper(child, key, result);
            }
        }


    }

    // Method to remove a song from the tree
    void remove(const Song& song) {
        if (root != nullptr) {
            removeHelper(root, song);
        }
    }

    // Helper function for remove
    void removeHelper(BPlusTreeNode* node,const Song& song) {
        if (node->isLeaf) {
            // Remove the song from the leaf node
            auto iter = node->songs.begin();
            for (; iter != node->songs.end(); iter++) {
                if (song.title == iter->title) {
                    return;
                }
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
            while (i < node->songs.size() && song.title > node->songs[i].title) {
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
        Song borrowedSong = leftSibling->songs.back();
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
        Song borrowedSong = rightSibling->songs.front();
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
                cout << song.title << ", ";
            }
        }

        else if(node == root) {
            cout << "Root Node - ";
            for (const auto& song : node->songs) {
                cout << song.title << ", ";
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
                cout << song.title << ", ";
            }
            cout << endl;
            // Recursively print child nodes
            for (const auto& child : node->children) {
                printHelper(child, level + 1);
            }
        }
        cout << endl;
    }


};

int main() {
    BPlusTree tree;

    Song song1 = {"Bomb", "Artist1", "Album1", "2000", "1992"};
    Song song2 = {"Song", "Artist2", "Album2", "2005", "1995"};
    Song song3 = {"Apple", "Artist3", "Album3", "1354", "2010"};
    Song song0 = {"Write", "Artist0", "Album0", "2019", "2011"};
    Song song4 = {"Dongo", "Artist4", "Album4", "1258", "2010"};
    Song song5 = {"Join", "Artist5", "Album5", "1995", "2010"};
    Song song6 = {"Us", "Artist6", "Album6", "2015", "2010"};
    Song song7 = {"Human", "Artist7", "Album7", "3111510", "2010"};

    Song song8 = {"Sunshine", "Artist8", "Album8", "180", "2012"};
    Song song9 = {"Moonlight", "Artist9", "Album9", "240", "2013"};
    Song song10 = {"Stars", "Artist10", "Album0", "210", "2014"};
    Song song11 = {"Galaxy", "Artist11", "Album11", "300", "2015"};
    Song song12 = {"Ocean", "Artist2", "Album12", "2000", "2016"};
    Song song13 = {"Mountain", "Artist13", "Album13", "320", "2017"};
    Song song14 = {"Forest", "Artist14", "Album14", "280", "2018"};
    Song song15 = {"Dongo", "Artist15", "Album15", "340", "2019"};

    tree.insert(song1);
    tree.insert(song2);
    tree.insert(song3);
    tree.insert(song0);
    tree.insert(song4);
    tree.insert(song5);
    tree.insert(song6);
    tree.insert(song7);
    tree.insert(song8);
    tree.insert(song9);
    tree.insert(song10);
    tree.insert(song11);
    tree.insert(song12);
    tree.insert(song13);
    tree.insert(song14);
    tree.insert(song15);
    //tree.printTree();
    // Example search
    cout << "here we go" << endl;
    vector<Song> searchResult = tree.search("2010");
    for (const auto& song : searchResult) {
        cout << "Title: " << song.title << ", Artist: " << song.artist << ", Album: " << song.album << ", Length: " << song.length << ", Year: " << song.year << endl;
    }

    return 0;
}
