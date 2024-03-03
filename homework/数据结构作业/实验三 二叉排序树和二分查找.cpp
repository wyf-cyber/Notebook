#include <iostream>
#include <vector>
using namespace std;

struct BTNode {
    int data;
    BTNode* left;
    BTNode* right;
};

// 根据数组构建二叉查找树
BTNode* building(const vector<int>& nums) {
    if(nums.size() == 0) return nullptr;
    BTNode* root = new BTNode();
    root->data = nums[0];
    root->left = nullptr;
    root->right = nullptr;
    for(int i = 1; i < nums.size(); i++) {
        // 插入值为nums[i]的节点
        BTNode* cur = root;
        while(cur) {
            if(cur->data > nums[i]) {
                // turn left
                if(cur->left) {
                    cur = cur->left;
                } else {
                    BTNode* lnode = new BTNode();
                    lnode->data = nums[i];
                    cur->left = lnode;
                    break;
                }
            } else {
                // turn right
                if(cur->right) {
                    cur = cur->right;
                } else {
                    BTNode* rnode = new BTNode();
                    rnode->data =  nums[i];
                    cur->right = rnode;
                    break;
                }
            }
        }
    }
    return root;
}

void InOrderTraversal(vector<int>& path, BTNode* root) {
    if(root == nullptr) return;
    InOrderTraversal(path, root->left);
    cout << root->data << " ";
    path.push_back(root->data);
    InOrderTraversal(path, root->right);
}

int searchInTree(BTNode* root, int target) {
    int count = 0;
    BTNode* cur = root;
    while(cur) {
        count++;
        if(cur->data > target) {
            cur = cur->left;
        } else if(cur->data == target) {
            // 找到目标节点
            break;
        } else {
            cur = cur->right;
        }
    }
    return count;
}

int binarySearch(vector<int> nums, int target) {
    int count = 1;   // 因为循环条件是查找失败，所以初始化为1
    int left = 0;
    int right = nums.size() - 1;
    int middle = (left+right)/2;
    while(nums[middle] != target) {
        count++;
        if(middle == left) {
            middle = right;
            continue;
        } else if(middle == right) {
            break;
        }
        // 折半查找
        if(nums[middle] > target) {
            right = middle - 1;
            middle = (left + right)/2;
        } else {
            left = middle + 1;
            middle = (left + right)/2;
        }
    }
    return count;
}

int main() {
    int n;
    cin >> n;
    vector<int> nums(n);
    for(int i = 0; i < n; i++) {
        cin >> nums[i];
    }
    int target;
    cin >> target;
    // 建立二叉排序树
    BTNode* root = building(nums);
    cout << searchInTree(root, target) << endl;
    vector<int> path;
    InOrderTraversal(path, root);
    cout << endl;
    cout << binarySearch(path, target);
}