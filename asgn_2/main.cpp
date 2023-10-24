#include <bits/stdc++.h>
#include "hsfq.h"
// #include "rapidjson/document.h" 
// #include "rapidjson/filereadstream.h" 

using namespace std;

int MAX_JOBS = 100;

unordered_map<int, int> iterators;

int main() {
    /*
        Generate jobs to be executed on the hspq tree structure.
        1. Construct a hsfq tree. 
        2. Generate jobs.
        2. Execute simple jobs on hsfq.
        3. Each job has to be blocked and woken up.
        3. Measure the throughput and execution time of a job.
        4. Add the data to a json file.
    */

    root = new Node(1, 1, &sfq_selector, &sfq_updater);
    root->is_leaf = 1;
    root->virtual_time = 0;
    
    Thread* t1 = new Thread(2, 20, 1, {{5, 4}});
    Thread* t2 = new Thread(3, 20, 2, {{6, 3}});
    threadPositions[t1->ID]={1};
    threadPositions[t2->ID]={2};
    iterators[t1->ID] = 0;
    iterators[t1->ID] = 0;
    insert(t1,threadPositions[t1->ID],root,1);
    insert(t2,threadPositions[t2->ID],root,1);

    timer = 0;
    while(root->numberOfThreads > 0 or !blockedQueue.empty()) {
        cout<<"time:"<<timer<<endl;
        if(root->numberOfThreads){
            Thread* thread = (Thread *)scheduler(root, 0);
            
            cout<<thread->ID<<"->";
            cout<<thread->start_tag<<endl;
            if(--thread->process_time == 0) {
                cout<<"exitted:"<<thread->ID<<endl;
                block(thread, 0, 0);
            }
            // blocking mechanism
            int i = iterators[thread->ID];
            if(i<thread->blockStates.size() && thread->blockStates[i].first == thread->process_time) {
                block(thread, 1, thread->blockStates[i].second);
                iterators[thread->ID]++;
            }

            Node* Parent = thread->parent;
            Parent->updater(root, thread, 1, 1);
            
        }
        threadWakeup();
        cout<<endl;
        timer++;
    }
    return 0;
}

/*
g++ -c hsfq.cpp
g++ -c main.cpp -I ./
g++ hsfq.o main.o
*/