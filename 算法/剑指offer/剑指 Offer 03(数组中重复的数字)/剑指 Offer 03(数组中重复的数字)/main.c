//
//  main.c
//  剑指 Offer 03(数组中重复的数字)
//
//  Created by Yee on 2020/12/4.
//

#include <stdio.h>
#include <stdlib.h>

/*
#define max_count 100000
int findRepeatNumber(int* nums, int numsSize){
    if (!nums || numsSize <= 0) return -1;
    int tem[max_count] = {0}; //初始化为0
    for (int i = 0; i < numsSize; i++) {
        tem[nums[i]] ++;
        if (tem[nums[i]] > 1) {
            return nums[i];
        }
    }
    return -1;
}
 */


int findRepeatNumber(int* nums, int numsSize){
    if (!nums || numsSize <= 0) return -1;
    for (int i = 0; i < numsSize; i++) {
        if (nums[i] != i) {//起始的位置要特别注意
            if (nums[i] == nums[nums[i]]) {
                return nums[i];
            }else{
                int tem = nums[nums[i]];
                nums[nums[i]] = nums[i];
                nums[i] = tem;
            }
        }
    }
    return -1;
}
int main(int argc, const char * argv[]) {
    // insert code here..
    int a[] = {2,3,1,0,2,5,3};
    int m = findRepeatNumber(a, 7);
    printf("%d",m);
    
    return 0;
}
