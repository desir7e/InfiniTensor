#include "../exercise.h"

// READ: Trivial type <https://learn.microsoft.com/zh-cn/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-170>

struct FibonacciCache {
    unsigned long long cache[16];
    int cached;
};

// TODO: 实现正确的缓存优化斐波那契计算
static unsigned long long fibonacci(FibonacciCache &cache, int i) {
    /*for (; false; ++cached) {
        cache[cached] = cache[cached - 1] + cache[cached - 2];
    }
    return cache.cache[i];*/
    // 如果索引小于等于已缓存的数量，直接返回缓存结果
    if(i <= cache.cached) {
        return cache.cache[i - 1];
    }

    // 否则计算新的斐波那契数并填充到缓存中
    for (int n = cache.cached + 1; n <= i; ++n) {
        if (n == 0) {
            cache.cache[n] = 0;
        } else if (n == 1) {
            cache.cache[n] = 1;
        } else{
            cache.cache[n] = cache.cache[n - 1] + cache.cache[n - 2];
        }
        cache.cached = n;
    }

    // 返回计算的结果
    return cache.cache[i];
}

int main(int argc, char **argv) {
    // TODO: 初始化缓存结构体，使计算正确
    // NOTICE: C/C++ 中，读取未初始化的变量（包括结构体变量）是未定义行为
    // READ: 初始化的各种写法 <https://zh.cppreference.com/w/cpp/language/initialization>
    //FibonacciCache fib;
    FibonacciCache fib = { {}, 0 }; // 使用大括号初始化所有元素为0，并设置已缓存数量为0
    ASSERT(fibonacci(fib, 10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fibonacci(fib, 10) << std::endl;
    return 0;
}
