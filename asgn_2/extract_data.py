import re

file_data = open("priority_benchmark.txt")
lines = file_data.readlines()

cleaned_data = []
for line in lines:
    if line == '\n':
        continue
    else:
        cleaned_data.append(line.strip())
# print(cleaned_data)

data = {}

prevTime = 0
for line in cleaned_data:
    if 'time' in line:
        prevTime = int(re.findall(r'\d+', line)[0])
    elif 'blocked' in line or 'wakeup' in line:
        job = line.split()[2]
        data[job]["block_wakeup_list"].append(prevTime)
    elif 'Item' in line and '->' in line:
        job = line.split()[1][0]
        if job not in data:
            data[job] = {
                "count": 0,
                "last_occurance": prevTime,
                "block_wakeup_list": []
            }
        data[job]["count"] += 1
        data[job]["last_occurance"] = prevTime

for job in data:
    data[job]["turnAround_time"] = data[job]["last_occurance"]
    for i in range(0, len(data[job]["block_wakeup_list"]), 2):
        data[job]["turnAround_time"] -= data[job]["block_wakeup_list"][i+1] - data[job]["block_wakeup_list"][i]
    
    data[job]["bandwidth"] = data[job]["count"] / data[job]["turnAround_time"]

print(data)