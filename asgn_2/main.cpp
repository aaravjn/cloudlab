#include <bits/stdc++.h>
#include "hsfq.h"
#include <fstream>
#include <cstdlib>
#include "rapidjson/filewritestream.h"
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>


using namespace std;

int MAX_JOBS = 100;
int uniqueID = 2;
unordered_map<int, int> iterators;

rapidjson::Document doc {};
void inputThreads(int timer){
    for(int i=0;i<doc[timer].Size();i++) {
        auto &tdata = doc[timer][i];
        Thread * thread = new Thread(uniqueID++,tdata["process_time"].GetInt(),tdata["weight"].GetInt());
        thread->name = tdata["name"].GetString();
        auto &blockedStates = tdata["blocked_states"];
        
        for (rapidjson::SizeType j = 0; j < blockedStates.Size(); j++) {
            const rapidjson::Value& blockedState = blockedStates[j];
            int startTime = blockedState["start_time"].GetInt();
            int blockDuration = blockedState["block_duration"].GetInt();
            thread->blockStates.push_back(make_pair(startTime,blockDuration));
        }

        iterators[thread->ID] = 0;
        const rapidjson::Value& positionArray = tdata["position"];
        for (rapidjson::SizeType k = 0; k < positionArray.Size(); k++) {
            int value = positionArray[k].GetInt();
            threadPositions[thread->ID].push_back(value);
        }
        insert(thread,threadPositions[thread->ID],root,1);
    }
}

int main() {
    // Output JSON document
    rapidjson::Document d;
    d.SetObject();
    rapidjson::Value threadsList(rapidjson::kArrayType);
    ifstream ifs { R"(unequal_benchmark.json)" };
    rapidjson::IStreamWrapper isw { ifs };
    doc.ParseStream( isw );

    root = new Node(1, 1, &sfq_selector, &sfq_updater);
    root->is_leaf = 1;
    root->virtual_time = 0;

    timer = 0;
    while(root->numberOfThreads > 0 or !blockedQueue.empty() or timer<doc.Size()) {
        
        // Output JSON Code begins
        rapidjson::Value thread_data(rapidjson::kObjectType);
        thread_data.GetObject();
        thread_data.AddMember("time", timer, d.GetAllocator());
        // Output JSON Code ends

        if(timer<doc.Size())
            inputThreads(timer);
        if(root->numberOfThreads){
            Thread* thread = (Thread *)scheduler(root, 0);
            
            // Output JSON Code begins
            thread_data.AddMember("thread",rapidjson::StringRef(thread->name.c_str()), d.GetAllocator());
            thread_data.AddMember("weight", thread->weight, d.GetAllocator());
            // Output JSON Code ends
            
            if(--thread->process_time == 0) {

                // Output JSON Code
                thread_data.AddMember("exitedThread",rapidjson::StringRef(thread->name.c_str()), d.GetAllocator());
                // Output JSON Code ends
                
                block(thread, 0, 0);
            }

            // blocking mechanism
            int i = iterators[thread->ID];
            if(i<thread->blockStates.size() && thread->blockStates[i].first == thread->process_time) {
                
                // Output JSON Code begins
                thread_data.AddMember("blockedThread",rapidjson::StringRef(thread->name.c_str()), d.GetAllocator());
                // Output JSON Code ends
                block(thread, 1, thread->blockStates[i].second);
                iterators[thread->ID]++;
            }

            Node* Parent = thread->parent;
            Parent->updater(root, thread, 1, 1);
        }
        threadsList.PushBack(thread_data,d.GetAllocator());
        threadWakeup();
        cout<<endl;
        timer++;
    }

    // Writing to output file begins
    d.AddMember("Threads", threadsList, d.GetAllocator());
    FILE *fp = fopen("execution_data.json","wb"); // non-Windows use "w"

    char writeBuffer[65536];

    // Writing the changes to execution_data.json file
    rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
    d.Accept(writer);
    fclose(fp);
    // Writing to output file ends
    return 0;
}