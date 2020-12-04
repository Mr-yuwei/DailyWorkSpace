//
//  main.c
//  剑指 Offer 03(数组中重复的数字)
//
//  Created by Yee on 2020/12/4.
//


#include <stdio.h>
#define maxCount  100000
int findRepeatNumber(int* nums, int numsSize){
    int a[maxCount] = { 0};
    for (int i = 0; i < numsSize; i ++) {
        int num = nums[i];//取下标值
        a[num]++;
        if (a[num] > 1) {//大于1说明重复了
            return num;
            break;
        }
    }
    return -1;
    
    
    
    
    
}
int main(int argc, const char * argv[]) {
    // insert code here...
    
    return 0;
}
