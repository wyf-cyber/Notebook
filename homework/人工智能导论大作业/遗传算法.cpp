#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

const int populationSize = 50;
const int maxGenerations = 1000;
const double mutationRate = 0.01;

// 随机数生成器
default_random_engine rng(random_device{}());

// 随机初始化棋盘
void randomInit(vector<int>& board) {
    // 设置随机数生成器的种子为当前时间，以确保每次运行都产生不同的随机数
    srand(static_cast<unsigned int>(time(nullptr)));
    int n = board.size();
    for(int i = 0; i < n; i++) {
        // 将皇后的位置随机初始化为分布在0到n-1的整数
        board[i] = rand() % n;
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
        if (board[i] == board[k] || board[k] + abs(k - i) == board[i] || board[k] - abs(k - i) == board[i]) {
            count++;
        }
    }
    return count;
}

int f(vector<int> board) {
    // 适应度函数
    // 适应度越高，状态越接近目标状态
    // 适应度函数的尺度变换采用线性变换  f' = a*f + b
    // 统计当前状态下发生冲突的皇后总数
    int sum = 0;
    int n = board.size();
    int maxSum = n * (n - 1);
    for (int i = 0; i < n; i++) {
        sum += conflictsNum(board, i);
    }
    return maxSum - sum;
}

struct State {
    // 存储状态节点
    vector<int> board;    // 当前状态皇后所在的列数
    int fitness;          // 当前状态的适应度
    // 创建一个构造函数以便初始化状态
    // 注意与类名相同的一定是构造函数，该函数不需要声明返回值，其返回值一定是void
    State(int n) : board(n), fitness(0){
        randomInit(board);
        fitness = f(board);
    }
};


// 交叉操作
State crossover(const State& parent1, const State& parent2, int n) {
    State child(n);
    uniform_int_distribution<int> distribution(0, n - 1);
    int crossoverPoint = distribution(rng);

    for (int i = 0; i < crossoverPoint; i++) {
        child.board[i] = parent1.board[i];
    }

    for (int i = crossoverPoint; i < n; i++) {
        child.board[i] = parent2.board[i];
    }
    // 更新适应度
    child.fitness = f(child.board);
    return child;
}

// 变异操作
void mutate(State& individual, int n) {
    if((rand() / (double)RAND_MAX) > mutationRate) return;
    uniform_int_distribution<int> distribution(0, n - 1);
    int i = distribution(rng);
    int j = distribution(rng);
    swap(individual.board[i], individual.board[j]);
    // 更新适应度
    individual.fitness = f(individual.board);
}

int chooseParent(vector<State>& population) {
    // 计算总的适应度分数
    int totalFitness = 0;
    for (const State& state : population) {
        totalFitness += state.fitness;
    }

    // 生成一个随机数，范围在 [0, 1)
    double randomValue = (rand() / (double)RAND_MAX);

    // 使用指数函数增大返回索引值小的元素的概率
    int selectedIdx = 0;
    double cumulativeProbability = 0.0;
    for (int i = 0; i < population.size(); i++) {
        double fitnessRatio = (double)population[i].fitness / totalFitness;
        cumulativeProbability += fitnessRatio;

        if (randomValue <= cumulativeProbability) {
            selectedIdx = i;
            break;
        }
    }

    return selectedIdx;
}

// 遗传算法
vector<int> geneticAlgorithm(int n) {
    int maxFitness = n * (n - 1);
    // 初始化种群
    vector<State> population;
    for(int i = 0; i < populationSize; i++) {
        State newState(n);
        population.push_back(newState);
    }
    for (int gen = 0; gen < maxGenerations; gen++) {
        // 每次都对新产生的种群排序
        sort(population.begin(), population.end(), [](const State& a, const State& b) {
            return a.fitness > b.fitness;
        });

        // 如果找到满意解，提前结束
        if (population[0].fitness == maxFitness) {
            return population[0].board;
        }

        // 创建新种群
        vector<State> newPopulation;

        for (int i = 0; i < populationSize/2; i++) {
            // 交叉操作
            int parentA = chooseParent(population);
            int parentB = chooseParent(population);
            State child = crossover(population[parentA], population[parentB], n);

            // 变异操作
            mutate(child, n);

            newPopulation.push_back(child);
        }
        // 将细胞记忆库加入种群
        for(int i = 0; i < populationSize/2; i++) {
            newPopulation.push_back(population[i]);
        }
        population = newPopulation;
    }

    return population[0].board;  // 返回最好的个体
}

// 打印n皇后问题的解
void printAns(vector<int> ans) {
    int n = ans.size();
    /*
    if(f(ans) < n*(n-1)) {
        // 未发现可行解
        cout << "No solution for this size." << endl;
        return;
    }
     */
    cout << "One valid solution:" << endl;
    for(int i = 0; i < ans.size(); i++) {
        string line(ans.size(), '#');
        line[ans[i]] = 'Q';
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
        vector<int> ans = geneticAlgorithm(n);
        // 停止计时
        // 打印结果的时间不计入算法运行时间中
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

        printAns(ans);

        cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
        cout << "Please enter the size of the board: ";
    }
}