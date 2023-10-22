#include <vector>

class Node {
    public:
        int ID;
        Node* parent;
        std::vector<void*> children;
        bool is_leaf;
        int start_tag;
        int finish_tag;
        int weight;
        void* (*selector)(std::vector<void*>, bool);
        void (*updater)(Node*, int, Node*);
        int virtual_time;
        
        Node(int, int, void* (*funcSelector)(std::vector<void*>, bool), void (*funcUpdater)(Node*, int, Node*));
};

class Thread {
    public:
        int ID;
        int process_time;
        int start_tag;
        int finish_tag;
        int weight;
        bool is_thread;
        Node* parent;

        Thread(int, int, int, int);
};


void* scheduler(void*, bool);
void* sfq_selector(std::vector<void*>, bool);
void insert(void*, std::vector<int>, Node*, int);
void block(Thread*);
void sfq_updater(Node*, int, Node*);