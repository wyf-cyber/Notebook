#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

#define MaxCol 10

using namespace std;

struct DataNode			/*定义数据结点类型*/
{
	int data[MaxCol]; 
	DataNode* next;	  /*指向后继数据结点*/
};

struct HNode	/*定义头结点类型*/
{
	int Row, Col;	/*行数和列数*/
	DataNode* next;	/*指向第一个数据结点*/

	HNode() :next(NULL) {}		//构造函数
	HNode(int r, int c)		//重载构造函数 
	{
		Row = r;
		Col = c;
		next = NULL;
	}
};

/*
输入
3 3
1 2 3
2 3 6
4 5 8
3 4
3 6 9 12
5 7 8 6
9 6 5 8
3 2
输出
2 3 6 3 6 9 12
2 3 6 9 6 5 8  
*/

vector<pair<int, int>> f(HNode& matrixOne, HNode& matrixTwo, vector<int> numsOne, vector<int> numsTwo) {
    vector<pair<int, int>> ans;
    for(int i = 0; i < matrixOne.Row; i++) {
        for(int j = 0; j < matrixTwo.Row; j++) {
            if(numsOne[i] == numsTwo[j]) {
                pair<int, int> elm(i, j);
                ans.push_back(elm);
            }
        }
    }
    return ans;
}

int main() {
    ifstream input("in.txt");
    ofstream output("abc.out");
    int row;
    int col;
    // 接收数据创建第一个二维链表
    input >> row >> col;
    HNode matrixOne(row, col);
    DataNode* pre = nullptr;              // 建立一个pre指针指向当前节点的前一个节点 
    for(int i = 0; i < row; i++) {
        // 将本行的链表节点接到上一行   
        DataNode* colNode = new DataNode;
        if(i == 0) {
            // 创建第一行，需要特别处理，因为当前节点的上一个节点是头结点
            matrixOne.next = colNode;
            pre = colNode;
        } else {
            pre->next = colNode;
            pre = colNode;
        }
        // 将本行数据逐个存入数组中
        for(int j = 0; j < col; j++) {
            int temp;
            input >> temp;
            colNode->data[j] = temp;
        }
    }
    // 接收数据创建第二个二维链表
    input >> row >> col;
    HNode matrixTwo(row, col); 
    for(int i = 0; i < row; i++) {
        // 将本行的链表节点接到上一行   
        DataNode* colNode = new DataNode;
        if(i == 0) {
            matrixTwo.next = colNode;
            pre = colNode;
        } else {
            pre->next = colNode;
            pre = colNode;
        }
        for(int j = 0; j < col; j++) {
            int temp;
            input >> temp;
            colNode->data[j] = temp;
        }
    }
    // 读取第一个表R的列序号a1和第二个表S的列序号a2。
    int a1, a2;
    input >> a1 >> a2;
    // 矩阵创建完毕，根据列序号创建两个数组存储两个矩阵指定列的数据信息以便比较
    vector<int> numsOne;
    DataNode* cur = matrixOne.next;         // 创建一个指针指向当前操作的元素
    for(int i = 0; i < matrixOne.Row; i++) {
        numsOne.push_back(cur->data[a1 - 1]);
        cur = cur->next;
    }
    vector<int> numsTwo;
    cur = matrixTwo.next;        
    for(int i = 0; i < matrixTwo.Row; i++) {
        numsTwo.push_back(cur->data[a2 - 1]);
        cur = cur->next;
    }

    vector<pair<int, int>> ans = f(matrixOne,  matrixTwo, numsOne, numsTwo);
    
    // 打印结果
    for(int i = 0; i < ans.size(); i++) {
        // 每一次循环打印结果中的一行
        // 读取矩阵一中对应的行
        cur = matrixOne.next;
        // 先将指针指向所要打印的行
        for(int j = 0; j < ans[i].first; j++) {
            cur = cur->next;
        }
        // 打印该行
        for(int k = 0; k < matrixOne.Col; k++) {
            output << cur->data[k] << " "; 
        }
        cur = matrixTwo.next;
        for(int j = 0; j < ans[i].second; j++) {
            cur = cur->next;
        }
        // 打印该行
        for(int k = 0; k < matrixTwo.Col; k++) {
            output << cur->data[k] << " "; 
        }
        output << endl;
    }
    input.close();
    output.close();
}