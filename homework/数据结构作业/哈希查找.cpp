#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#define MAP_SIZE 200
using namespace std;

vector<int> createHashMap(const vector<int>& nums) {
    // 将所给的无序数组存入一个空的容量为200的哈希表中
    vector<int> map(MAP_SIZE, 0);   
    for(int i = 0; i < nums.size(); i++) {
        int key = nums[i];
        int index = key % 199;
        while(map[index] != 0) {
            // 进行线性探测直到找到合适空位置
            index = (index + 1) % 200; 
        }
        // 将数据存入map数组
        map[index] = key;
    }
    return map;
}

vector<int> search(const vector<int>& map, int target, bool& res) {
    int index = target % 199;
    int begin = index; // 记录起始位置
    vector<int> path;  // 记录查找路径，若成功找到目标元素，则路径中包含该元素自身
    while(map[index] != target) {
        if(map[index] == 0 || ((index == begin)&&(!path.empty()))) {
            // 查找失败
            res = false;
            break;
        }
        // 记录经过的节点，更新index        
        path.push_back(map[index]);
        index = (index + 1) % 200;
    }
    if(res) path.push_back(target);
    return path;
}

int main() {
    ifstream input("in.txt");
    ofstream output("out.txt");
    int target;
    input >> target;
    input.ignore();
    vector<int> nums;
    int temp;
    while(input >> temp) {
        nums.push_back(temp);
    }
    vector<int> path;
    bool res = true;
    path = search(createHashMap(nums), target, res);
    // 打印结果
    if(res) output << "True" << endl;
    else output << "False" << endl;
    
    for(int i = 0; i < path.size(); i++) {
        output << path[i] << " ";
    }
}