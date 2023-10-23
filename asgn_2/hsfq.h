#include <vector>
using namespace std;
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
        void (*updater)(Node*,void*, int, bool); // Will update the state of the ancestor path of the tree. Eg:- start_tag, finish_tag, virtual_time.
        int virtual_time;
        int numberOfThreads; 

        Node(int id, int Weight, void* (*funcSelector)(vector<void*>, bool), void (*funcUpdater)(Node*, void*, int, bool)) { //  Constructor function of the class Node
            ID = id;
            weight = Weight;
            selector = funcSelector;
            updater = funcUpdater;
            parent = NULL;
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


void* scheduler(void*, bool);
void* sfq_selector(std::vector<void*>, bool);
void insert(void*, std::vector<int>, Node*, int);
void block(Thread*);
void sfq_updater(Node*, void*, int, bool );