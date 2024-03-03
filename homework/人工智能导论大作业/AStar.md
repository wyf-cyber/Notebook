~~~cpp
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <chrono>

using namespace std;

struct State {
    vector<int> board;
    int evaluation;      // h(n), 估计代价，用当前状态下皇后之间的冲突数表示
    int cost;            // g(n), 实际成本，用移动的次数表示
};

struct CompareState {
    bool operator()(const State& a, const State& b) {
        // 在这里指定优先队列如何比较两个状态的优先级
        // 使用状态的 cost 和 evaluation 的和作为比价标准
        // f(n) = futureCost(n) + h(n)
        if (a.cost + a.evaluation != b.cost + b.evaluation) {
            return a.cost + a.evaluation > b.cost + b.evaluation;
        }
        // 如果 cost + evaluation 相等，根据首项元素的大小决定
        return a.board[0] < b.board[0];
    }
};

// 随机初始化棋盘
void randomInit(vector<int>& board) {
    // 设置随机数生成器的种子为当前时间，以确保每次运行都产生不同的随机数
    srand(static_cast<unsigned int>(time(nullptr)));
    int n = board.size();
    // 利用洗牌算法生成不含重复元素的随机棋盘
    vector<int> discard(n);
    for (int i = 0; i < n; i++) {
        discard[i] = i;
    }
    // 洗牌过程
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(discard[i], discard[j]);
    }
    // 将洗牌后生成的随机序列分配给棋盘
    for (int i = 0; i < n; i++) {
        board[i] = discard[i];
    }
}

int conflictsNum(vector<int> board, int i) {
    // 统计Q[i]与棋盘上其他皇后之间的冲突数
    int count = 0;
    for (int k = 0; k < board.size(); k++) {
        if (k == i || board[k] == -1) {
            // 跳过Q[i]与Q[i]的检验
            continue;
        }
        // 因为一定不在同一行（列）所以不用检查
        if (board[k] + abs(k - i) == board[i] || board[k] - abs(k - i) == board[i]) {
            count++;
        }
    }
    return count;
}

int futureCost(vector<int> board) {
    // 用于估计未来的代价
    // 统计当前状态下发生冲突的皇后总数
    int sum = 0;
    for (int i = 0; i < board.size(); i++) {
        sum += conflictsNum(board, i);
    }
    return sum;
}

void move(vector<int>& board, int i, int j) {
    // 进行一次状态改变
    // 将棋盘上的两列互换位置
    int temp = board[i];
    board[i] = board[j];
    board[j] = temp;
}

// A*算法
vector<int> aStar(int n) {
    // 初始化起始状态
    vector<int> initState(n, -1);
    randomInit(initState);
    State start;
    start.board = initState;
    start.cost = 0;
    start.evaluation = futureCost(start.board);

    // 创建优先队列作为开表存储将要搜索的状态
    priority_queue<State, vector<State>,CompareState> open;
    open.push(start);

    while (!open.empty()) {
        // 从队列中取出当前状态
        // 每一次都要优先取代价最小的状态
        State current = open.top();
        open.pop();

        if (current.evaluation == 0) {
            // 找到解
            return current.board;
        }
        // 因为指针i,j已经做了去重操作，所以不需要创建闭表
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                vector<int> newState = current.board;
                move(newState, i, j);
                State nextState;
                nextState.board = newState;
                nextState.evaluation = futureCost(nextState.board);
                nextState.cost = current.cost + 1;
                open.push(nextState);
            }
        }
    }

    // 未找到解的情况
    return vector<int>();
}

void printAns(vector<int> board) {
    if (board.empty()) {
        // 没有可行解
        cout << "No solution for this size." << endl;
        return;
    }
    for (int i = 0; i < board.size(); i++) {
        string line(board.size(), '#');
        line[board[i]] = 'Q';
        cout << line << endl;
    }
}

int main() {
    cout << "Please enter the size of the board: ";
    int n;
    while (cin >> n) {
        if(n < 4) {
            // n == 1 或 n == 2 或 n = 3 时一定没有符合条件的解
            cout << "No solution for this size." << endl;
            cout << "Please enter the size of the board: ";
            continue;
        }
        // 开始为本次算法运行计时
        auto start = chrono::high_resolution_clock::now();
        vector<int> ans = aStar(n);
        // 停止计时
        // 打印结果的时间不计入算法运行时间中
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

        printAns(ans);

        cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
        cout << "Please enter the size of the board: ";
    }
}
~~~