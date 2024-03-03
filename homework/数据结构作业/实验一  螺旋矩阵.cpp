#include <iostream>
#include <vector>

using namespace std;

void printMatrix(int n) {
    int startX = 0;       // 本圈开始坐标的行
    int startY = 0;       // 本圈开始坐标的列
    int count = 1;        // 用于记录每一格中应该放置的数字
    int offset = 1;       // 定义矩阵的右侧偏置量，每一圈增大1
    vector<vector<int>> matrix(n, vector<int>(n, 0));
    for(int i = 0; i < n/2; i++) {
        // 循环[n/2]圈
        // n 是奇数，循环 n/2 - 1 圈
        // n 是偶数，循环 n/2 圈
        
        // 从左向右打印上方的一行
        for(int j = startX; j < n - offset; j++) {
            matrix[i][j] = count;
            count++;
        }
        // 从上向下打印右边的一列
        for(int j = startY; j < n - offset; j++) {
            matrix[j][n - offset] = count;
            count++;
        }
        // 从右往左打印下面的一行
        for(int j = n - offset; j > offset - 1; j--) {
            matrix[n - offset][j] = count;
            count++;
        }
        // 从下向上打印左边的一列
        for(int j = n - offset; j > offset - 1; j--) {
            matrix[j][startY] = count;
            count++;
        }
        startX++;
        startY++;
        offset++;
    }
    // 如果n是奇数，需要额外添加中央的元素
    if(n % 2) {
        matrix[n / 2][n / 2] = count;
    }
    // 打印结果
    for(int row = 0; row < n; row++) {
        for(int col = 0; col < n; col++) {
            cout << matrix[row][col] << " ";
        }
        cout << endl;
    }
}

int main() {
    int n = 1;
    cin >> n;
    printMatrix(n);
}