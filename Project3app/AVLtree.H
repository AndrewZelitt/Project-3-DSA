#ifndef AvlTree_h
#define AvlTree_h

#include <iostream>
#include <string>
#include <string.h>

using namespace std;


struct TreeNode {
    vector<const char*>* song;
    int position_in_vector;
    TreeNode* left;
    TreeNode* right;
    int height;
    vector<TreeNode*> dupes;
    TreeNode(int pos) {
        position_in_vector = pos;
        left = nullptr;
        right = nullptr;
        height = 0;
    }
};
class AVLtree {
    //Just point to the database to get each value.
   
   
    int count = 0;
    int maxid = 1;
    int found = 0;
    
    TreeNode* deleting;

public:
    int num_items = 0;
    vector<vector<const char*>>* raw_database;
    TreeNode* root;
    vector<int> output_for_search;
    TreeNode* insert(TreeNode* root, int pos, int arg, int arg1);
    TreeNode* right_balance(TreeNode* root);
    TreeNode* left_balance(TreeNode* root);
    TreeNode* right_left_balance(TreeNode* root);
    TreeNode* left_right_balance(TreeNode* root);
    //TreeNode* removeID(TreeNode* root, string id);
    void findNthnode(TreeNode* root, int num);
    //void removeInorder(TreeNode* root, int num);
    int max_id(TreeNode* root);
    //probably dont need different searches, maybe just 2, a helper and an output;
    void searchID(TreeNode* root, string input, int arg, int arg1);
    void search(TreeNode* root, string input, int arg, int arg1);
    void searchn(TreeNode* root, string name, int arg);
    int find_height(TreeNode* root);
    void inorder(TreeNode* root);
    void preorder(TreeNode* root);
    void postorder(TreeNode* root);
    void inorder(TreeNode* root, bool& first);
    void preorder(TreeNode* root, bool& first);
    void postorder(TreeNode* root, bool& first);
    void printLevelCount(void);
    void deleteAll(TreeNode* root);
    //void search_IDInsert(TreeNode* root, string id);
    AVLtree() {}
};

#endif