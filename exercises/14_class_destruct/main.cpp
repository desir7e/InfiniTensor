#include "../exercise.h"

// READ: 析构函数 <https://zh.cppreference.com/w/cpp/language/destructor>
// READ: RAII <https://learn.microsoft.com/zh-cn/cpp/cpp/object-lifetime-and-resource-management-modern-cpp?view=msvc-170>

/// @brief 任意缓存容量的斐波那契类型。
/// @details 可以在构造时传入缓存容量，因此需要动态分配缓存空间。
class DynFibonacci {
    size_t *cache;
    int cached;
   int capacity;
public:
    // TODO: 实现动态设置容量的构造器
    //DynFibonacci(int capacity): cache(new ?), cached(?) {}
      
    DynFibonacci(int capacity) : capacity(capacity), cached(2) {
        // 动态分配缓存空间，确保至少可以存储两个斐波那契数
        cache = new size_t[capacity];
        cache[0] = 0;  // 初始值 fib(0) = 0
        cache[1] = 1;  // 初始值 fib(1) = 1
    }

    // TODO: 实现析构器，释放缓存空间
    ~DynFibonacci(){
           
        delete[] cache;  // 释放内存
    };

    // TODO: 实现正确的缓存优化斐波那契计算
    size_t get(int i) {  
    // 如果需要的数值超出了当前缓存，进行计算并更新缓存
        for (int j = cached; j <= i; ++j) {
            // 防止超过缓存容量
            if (j >= capacity) {
                throw std::out_of_range("Requested Fibonacci index exceeds cache capacity.");
            }
            cache[j] = cache[j - 1] + cache[j - 2];
        }

        cached = i + 1;  // 更新缓存已计算的数量
        return cache[i];
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;
    return 0;
}
