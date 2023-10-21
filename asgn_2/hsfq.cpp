#include <iostream>
#include <vector>

using namespace std;

class Thread {
    public:
        int process_time;
        int start_tag;
        int finish_tag;
        int weight;
        bool is_thread = 1;
        Node* parent;
};
class Node {
    public:
        Node* parent;
        vector<void*> children;
        bool is_leaf;
        int start_tag;
        int finish_tag;
        int weight;
        void* (*selector)(vector<void*>, bool);
        int (*updater)();
        int virtual_time;
};


void* scheduler(void* root, bool is_thread) {
    if(is_thread) {
        return root;
    }
    else {
        void* child = ((Node*)root)->selector(((Node*)root)->children, ((Node*)root)->is_leaf);
        return scheduler(child, ((Node*)root)->is_leaf);
    }
}


void* sfq_selector(vector<void*> children, bool is_leaf) {
    int min_start_tag = 1e9;
    void* finalChild = NULL;

    if(is_leaf) {
        for(void* thread : children) {
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
}

