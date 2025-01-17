#include "../exercise.h"

struct Fibonacci {
    unsigned long long cache[128];
    int cached;

    // TODO: 实现正确的缓存优化斐波那契计算
    unsigned long long get(int i) {
       /*for (; false; ++cached) {
            cache[cached] = cache[cached - 1] + cache[cached - 2];
        }
        return cache[i];
         // 如果缓存中没有计算到第i项，递推计算*/
        if (i > cached) {
            for (int j = cached + 1; j <= i; ++j) {
                cache[j] = cache[j - 1] + cache[j - 2];  // 根据斐波那契递推公式计算
            }
            cached = i;  // 更新已计算的最大索引
        }
        return cache[i];  // 返回缓存中的结果
    }
    
};

int main(int argc, char **argv) {
    // TODO: 初始化缓存结构体，使计算正确
    Fibonacci fib;
    fib.cache[0] = 0;
     fib.cache[1] = 1;
    fib.cached=1;
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;
    return 0;
}
