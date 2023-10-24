#include <vector>
using namespace std;
class Node {
    public:
        int ID;
        Node* parent; // Pointer to the parent node.
        vector<void*> children;
        bool is_leaf;
        double start_tag;
        double finish_tag;
        double virtual_time;
        int weight;
        void* (*selector)(vector<void*>, bool); // Will schedule one of it's child node
        void (*updater)(Node*,void*, int, bool); // Will update the state of the ancestor path of the tree. Eg:- start_tag, finish_tag, virtual_time.
        int numberOfThreads;

        Node(int id, int Weight, void* (*funcSelector)(vector<void*>, bool), void (*funcUpdater)(Node*, void*, int, bool)) { //  Constructor function of the class Node
            ID = id;
            weight = Weight;
            selector = funcSelector;
            updater = funcUpdater;
            parent = NULL;
            numberOfThreads=0;
            virtual_time = 0;
        }
};

class Thread { // A class for the thread object in the hsfq tree
    public:
        int ID;
        string name;
        int process_time; // remaining time to execute the thread, (in seconds)
        double start_tag;
        double finish_tag;
        int unblockTime;
        int weight;
        bool is_thread;
        vector<pair<int, int>> blockStates; // Blocks the thread according to the remaining process time(pair.first) and time for which to block(pair.second)
                                            // Always in sorted order.
        Node* parent; // a pointer to the parent node
    
        Thread(int id, int processTime, int Weight, vector<pair<int, int>> block_states) { // constructor function for the class Thread
            ID = id;
            process_time = processTime;
            weight = Weight;
            parent = NULL;
            is_thread = 1;
            for(auto x  : block_states){
                blockStates.push_back(x);
            }
        }
};

class Comparator {
    public:
        bool operator()(Thread* &a, Thread* &b) {
            return a->unblockTime < b->unblockTime;
        }
};
extern priority_queue<Thread*, vector<Thread*>, Comparator> blockedQueue;
extern unordered_map<int, vector<int>> threadPositions;
extern int timer;
extern Node* root;


void* scheduler(void*, bool);
void* sfq_selector(std::vector<void*>, bool);
void insert(Thread*, std::vector<int>, Node*, int);
void block(Thread*, bool, int);
void sfq_updater(Node*, void*, int, bool );
void notifyThreadRemoved(Node * node);
void threadWakeup();