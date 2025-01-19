#include <stdio.h>

/*
1、保留每个元素最多三次重复。
2、删除同一个元素超过三次的重复部分
3、时间复杂度为O(n)
4、空间复杂度为O(1)
*/
size_t func(int *nums, int argsNum);

int main(int argc, char *argv[])
{

    int nums[] = {-1,-1,1,1,1,1,1,1,3,3,3,3,3,3,3,3};
    size_t alter_len = func(nums, 16);
    printf("alter_len = %ld \n", alter_len);
    return 0;
}

size_t func(int *nums, int argsNum)
{
    int *pre = nums;
    int args_num = argsNum;
    int *p;
    int count = 0;
    for (int i = 0; i < argsNum; i++)
    {
        count += 1;
        p = &nums[i];
        if(pre == p){
            continue;
        }
        if (*pre != *p)
        {
            pre = p;
            count = 1;
            continue;
        }else{ // *pre == *p
            
            if (count <= 3)
            {

            }else{ // count > 3
                args_num -= 1;
                if(*pre != *(p+1)){
                    pre = p+1; 
                    count = 0;
                }
            }
        }
    }
    return args_num;
}
