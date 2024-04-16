//written by Garrett McClay
//haven't tested it much but it compiles and seems
// to at least partially work with the few inputs I tried

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;
int MAX_SONGS_PER_NODE = 5;
int MIN_SONGS_PER_NODE = 3;

// Define your Song struct
struct Song {
    string title;
    string artist;
    string album;
    int year;
    double length; // Assuming length in minutes or seconds
    // You can add more attributes as needed
};

// Define your B+ tree node structure
struct BPlusTreeNode {
    vector<Song> songs; // Vector to store songs
    vector<BPlusTreeNode*> children; // Vector to store child nodes
    BPlusTreeNode* next;
    bool isLeaf; // Flag to check if the node is a leaf
    // You might need additional variables based on your implementation
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
        if (root == nullptr) {
            root = new BPlusTreeNode();
            root->songs.push_back(song);
            root->isLeaf = true;
        } else {
            insertHelper(root, song);
        }
    }

    // Helper function for insertion
    void insertHelper(BPlusTreeNode* node, const Song& song) {
        if (node->isLeaf) {
            // Insert into leaf node
            // Find the appropriate position to maintain order
            auto it = lower_bound(node->songs.begin(), node->songs.end(), song, [](const Song& a, const Song& b) {
                return a.title < b.title; // Example: Insert based on title
            });
            node->songs.insert(it, song);

            // Split the leaf node if necessary
            if (node->songs.size() > MAX_SONGS_PER_NODE) {
                splitLeafNode(node);
            }
        } else {
            // Find the child node to descend into
            int i = 0;
            while (i < node->songs.size() && song.title > node->songs[i].title) {
                i++;
            }
            insertHelper(node->children[i], song);
        }
    }

// Function to split a leaf node
    void splitLeafNode(BPlusTreeNode* node) {
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
        Song middleSong = newNode->songs.front(); // Assuming keys are sorted
        insertIntoParent(node, middleSong, newNode);
    }

    // Function to find the parent of a given node
    BPlusTreeNode* findParent(BPlusTreeNode* current, BPlusTreeNode* child) {
        if (current == nullptr || current->isLeaf) {
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
        node->children.erase(node->children.begin() + mid + 1, node->children.end());

        // Find the parent node
        BPlusTreeNode* parent = findParent(root, node);
        if (parent == nullptr) {
            // If the current node is the root, create a new root
            parent = new BPlusTreeNode();
            root = parent;
        }

        // Insert the middle key to the parent node
        parent->songs.insert(parent->songs.begin() + distance(parent->children.begin(), find(parent->children.begin(), parent->children.end(), node)), node->songs[mid]);
        // Insert the new node to the parent's children
        parent->children.insert(parent->children.begin() + distance(parent->children.begin(), find(parent->children.begin(), parent->children.end(), node)) + 1, newNode);

        // If the parent node now has too many keys, split it recursively
        if (parent->songs.size() > MAX_SONGS_PER_NODE) {
            splitInternalNode(parent);
        }
    }


    // Function to insert a key into the parent node
    void insertIntoParent(BPlusTreeNode* leftChild, const Song& song, BPlusTreeNode* rightChild) {
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
                if (iter->title > song.title) {
                    break;
                }
            }

            // Insert the right child pointer
            int index = distance(parent->songs.begin(), iter);
            parent->children.insert(parent->children.begin() + index + 1, rightChild);

            // Split the parent node if necessary
            if (parent->songs.size() > MAX_SONGS_PER_NODE) {
                splitInternalNode(parent);
            }
        }
    }

    // Method to search for songs based on criteria like title, artist, etc.
    vector<Song> search(const string& key) {
        vector<Song> result;
        if (root != nullptr) {
            searchHelper(root, key, result);
        }
        return result;
    }

    // Helper function for search
    void searchHelper(BPlusTreeNode* node, const string& key, vector<Song>& result) {
        if (node->isLeaf) {
            // Search for the key in the leaf node
            for (const auto& song : node->songs) {
                if (song.title == key || song.artist == key || song.album == key) {
                    result.push_back(song);
                }
            }
        } else {
            // Descend to the appropriate child node
            int i = 0;
            while (i < node->songs.size() && key > node->songs[i].title) {
                i++;
            }
            searchHelper(node->children[i], key, result);
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
        if (node->isLeaf) {
            cout << "Leaf Node - ";
            for (const auto& song : node->songs) {
                cout << song.title << ", ";
            }
        } else {
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
    // Create B+ tree instance
    BPlusTree tree;

    // Populate the tree with songs
    // You can read data from a file or any other source and insert into the tree

    // Example usage:
    Song song1 = {"Title1", "Artist1", "Album1", 2000, 4.5};
    Song song2 = {"Title2", "Artist2", "Album2", 2005, 3.2};
    Song song3 = {"Title3", "Artist3", "Album3", 2010, 5.1};

    tree.insert(song1);
    tree.insert(song2);
    tree.insert(song3);

    // Example search
    vector<Song> searchResult = tree.search("Title2");
    for (const auto& song : searchResult) {
        cout << "Title: " << song.title << ", Artist: " << song.artist << ", Album: " << song.album << ", Year: " << song.year << ", Length: " << song.length << endl;
    }

    return 0;
}
