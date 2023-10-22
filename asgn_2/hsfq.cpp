/*
    A Simulation of the Hierarchial SFQ algorithm
*/
#include <iostream>
#include <vector>

using namespace std;


/*
    This will form the basis of the tree structure of the hsfq algorithm.
    This Tree will be predefined before pushing any jobs to it.
    The tree will represent the hierarchy of the SFQ algorithm.
*/

class Node {
    public:
        int ID;
        Node* parent; // Pointer to the parent node.
        vector<void*> children;
        bool is_leaf;
        int start_tag;
        int finish_tag;
        int weight;
        void* (*selector)(vector<void*>, bool); // Will schedule one of it's child node
        void (*updater)(Node*, int, Node*); // Will update the state of the ancestor path of the tree. Eg:- start_tag, finish_tag, virtual_time.
        int virtual_time;

        Node(int id, int Weight, void* (*funcSelector)(vector<void*>, bool), void (*funcUpdater)(Node*, int, Node*)) { //  Constructor function of the class Node
            ID = id;
            weight = Weight;
            selector = funcSelector;
            updater = funcUpdater;
        }
};

class Thread { // A class for the thread object in the hsfq tree
    public:
        int ID;
        int process_time; // remaining time to execute the thread, (in seconds)
        int start_tag;
        int finish_tag;
        int weight;
        bool is_thread;
        Node* parent; // a pointer to the parent node
    
        Thread(int id, int processTime, int startTag, int Weight) { // constructor function for the class Thread
            ID = id;
            process_time = processTime;
            start_tag = startTag;
            weight = Weight;
            parent = NULL;
            is_thread = 1;
        }
};

void* scheduler(void* root, bool is_thread) { // The main scheduler function of the algorithm.
    if(is_thread) {
        return root;
    }
    else {
        void* child = ((Node*)root)->selector(((Node*)root)->children, ((Node*)root)->is_leaf);
        return scheduler(child, ((Node*)root)->is_leaf);
    }
}


void* sfq_selector(vector<void*> children, bool is_leaf) { // Selector function which is specific to each node.
    int min_start_tag = 1e9;
    void* finalChild = NULL;

    /*
        Will check if the current node is leaf node or not.
        If so, will select the thread to be executed. If not, it will select one of the children
        node to be analyzed further.
    */

    if(is_leaf) {
        for(void* thread : children) { // Searching through the entire children array of the node.
            if(((Thread *)thread)->start_tag < min_start_tag) {
                min_start_tag = ((Thread *)thread)->start_tag;
                finalChild = thread;
            }
        }
        ((Thread *)finalChild)->parent->virtual_time = min_start_tag;
    }
    else {
        for(void* child : children) {
            if(((Node *)child)->start_tag < min_start_tag) {
                min_start_tag = ((Node *)child)->start_tag;
                finalChild = child;
            }
        }
        ((Node *)finalChild)->parent->virtual_time = min_start_tag;
    }

    return finalChild;
}


void insert(void* newNode, vector<int> position, Node* root, int i = 0) { // Used to insert any new node or thread.
    if(i == position.size()) {
        root->children.push_back(newNode);
    }
    else {
        for(void* child : root->children) {
            if(((Node *)child)->ID == position[i]) {
                insert(newNode, position, (Node*)child, i+1);
                return ;
            }
        }
    }
}


void sfq_updater(Node* node, int lengthQuantum, Node* root) { // It will update the state of the tree like changing it's virtual_time or start_tag and finish_tags of the nodes.
    if(node == root) {
        return ;
    }
    
    node->finish_tag = node->start_tag + lengthQuantum / node->weight;
    node->start_tag = max(node->virtual_time, node->finish_tag);

    sfq_updater(node->parent, lengthQuantum, root);
}


void block(Thread* thread) { // Blocks a thread by removing it from the tree structure. It will later on be pushed again in the tree based on its position.
    Node* parentNode = thread->parent;
    auto start_itr = parentNode->children.begin();

    for(int i = 0;i < parentNode->children.size();i++) {
        if((Thread *)parentNode->children[i] == thread) {
            (parentNode->children).erase(i + start_itr);
            break;
        }
    }
}