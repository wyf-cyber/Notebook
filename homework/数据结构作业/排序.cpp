#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int divide(vector<int>& nums, int left, int right) {
    int base = nums[left];
    while(left < right) {
        while(left < right && base >= nums[right]) right--;
        if(left < right) {
            nums[left] = nums[right];
            left++;
        }
        while(left < right && base <= nums[left]) left++;
        if(left < right) {
            nums[right] = nums[left];
            right--;
        }
    }
    nums[left] = base;
    return left;
}

void quickSort(vector<int>& nums, int left, int right) {
    if(left >= right) return;
    int mid = divide(nums, left, right);
    quickSort(nums, left,  mid-1);
    quickSort(nums, mid+1, right);
}

int main() {
    int i = 0;
    vector<int> nums;
    ifstream input("in.txt");
    ofstream output("out.txt");
    while(input >> i) {
        nums.push_back(i);
    }
    quickSort(nums, 0, nums.size() - 1);
    for(int j = 0; j < 10; j++) {
        output << nums[j] << " ";
    }
}