#include <iostream>
#include <fstream>
#include <string>
#include <queue>

#define SUB_QUEUE_MAX 1000           // 子队列中所能存储的最多的元素个数
#define TEAM_NUMBER_MAX 1000
#define MAX_ID 1000000               // 个人ID的最大取值

using namespace std;

queue<int> team[SUB_QUEUE_MAX];      // 创建一个组队列（存储元素为队列的数组）
queue<int> mainQueue;                // 主队列，存储各个小组之间的位置关系
int people[MAX_ID];                  // 存储人的编号对应的小组编号，以便查询
bool teamIsInMain[TEAM_NUMBER_MAX];  // 查询组号是否在主队列中，初始状态为false，表示不在
int teamNum;                         // 存储当前子队列的数目

int main() {
    ifstream inputFile("in.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Cannot open input file." << endl;
        return 1;
    }

    // 处理并记录输入的子队列信息
    int peopleID;                    // 存储个人的编号
    int teamSize;                    // 存储当前小组的总人数
    inputFile >> teamNum;
    for(int teamIndex = 0; teamIndex < teamNum; teamIndex++) {
        inputFile >> teamSize;
        while(teamSize--) {
            inputFile >> peopleID;
            // 把每个人的ID（成员编号）与其值（组号）建立映射
            people[peopleID] = teamIndex;
        }
    }


   // 读取入队和出队指令，执行操作
    int teamID;                      // 存储某人所属小队列的编号
    string command;
    while(inputFile >> command && command != "STOP") {
        if(command == "ENQUEUE") {
            // 进队操作
            inputFile >> peopleID;
            teamID = people[peopleID];
            if(!teamIsInMain[teamID]) {
                // 如果不在，创建一个新的子队列
                // 新队列位于主队列的末尾
                teamIsInMain[teamID] = true;
                mainQueue.push(teamID);
            }
            // 将该元素加入所属子队列中
            team[teamID].push(peopleID);
        }else {
            // 出队操作
            if(!mainQueue.empty()) {
                // 若主队列为空，说明不可执行出队操作
                teamID = mainQueue.front();    // 查询队头元素属于哪一组
                cout << team[teamID].front() << endl;
                team[teamID].pop();
                // 检查出队以后该组是否为空
                if(team[teamID].empty()) {
                    // 将该组从大队列中删除
                    mainQueue.pop();
                    teamIsInMain[teamID] = false;
                }
            }
        }
    }
    inputFile.close();
}
