#include "../exercise.h"

// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>

template<class T>
struct Tensor4D {
    unsigned int shape[4];
    T *data;

    Tensor4D(unsigned int const shape_[4], T const *data_) {
        //unsigned int size = 1;
         // 复制形状信息
        std::memcpy(shape, shape_, sizeof(shape));

        // 计算张量的大小
        unsigned int size = shape[0] * shape[1] * shape[2] * shape[3];
        // TODO: 填入正确的 shape 并计算 size
        data = new T[size];
        std::memcpy(data, data_, size * sizeof(T));
    }
    ~Tensor4D() {
        delete[] data;
    }

    // 为了保持简单，禁止复制和移动
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

    // 这个加法需要支持“单向广播”。
    // 具体来说，`others` 可以具有与 `this` 不同的形状，形状不同的维度长度必须为 1。
    // `others` 长度为 1 但 `this` 长度不为 1 的维度将发生广播计算。
    // 例如，`this` 形状为 `[1, 2, 3, 4]`，`others` 形状为 `[1, 2, 1, 4]`，
    // 则 `this` 与 `others` 相加时，3 个形状为 `[1, 2, 1, 4]` 的子张量各自与 `others` 对应项相加。
   /*  Tensor4D &operator+=(Tensor4D const &others) {
        // TODO: 实现单向广播的加法
          // 检查广播兼容性
        for (int i = 0; i < 4; ++i) {
            if (shape[i] != others.shape[i] && others.shape[i] != 1) {
                // 如果维度不匹配且others的对应维度不为1，则不兼容
                throw std::invalid_argument("Shapes are not broadcast-compatible.");
            }
        }

        // 广播相加
        unsigned int size = 1;
        for (int i = 0; i < 4; ++i) {
            size *= shape[i];
        }

        // 执行加法操作
        for (unsigned int i = 0; i < size; ++i) {
            unsigned int indices[4];
            unsigned int temp = i;

            // 计算各维度的索引
            for (int j = 3; j >= 0; --j) {
                indices[j] = temp % shape[j];
                temp /= shape[j];
            }

            // 获取广播值
            T other_value = 0;
            for (int j = 0; j < 4; ++j) {
                if (others.shape[j] == 1 || others.shape[j] == shape[j]) {
                    other_value = others.data[i];  // 由于广播，只有1维会重复
                    break;
                }
            }

            // 执行加法
            data[i] += other_value;
        }

        return *this;
    }
}; */
Tensor4D& operator+=(const Tensor4D& others) {
      // 检查广播兼容性
        for (int i = 0; i < 4; ++i) {
            if (shape[i] != others.shape[i] && others.shape[i] != 1) {
                throw std::invalid_argument("Shapes are not broadcast-compatible.");
            }
        }
        unsigned int size = 1;
        for (int i = 0; i < 4; ++i) {
            size *= shape[i];
        }
        // 计算广播后的索引映射
        auto getBroadcastIndex = [&](unsigned int index, int dim) -> unsigned int {
            unsigned int stride = 1;
            for (int j = 3; j > dim; --j) {
                stride *= shape[j];
            }
            unsigned int otherDimSize = others.shape[dim];
            return (index / stride) % otherDimSize;
        };

        // 执行加法操作
        for (unsigned int i = 0; i < size; ++i) {
            unsigned int other_index = 0;
            unsigned int stride = 1;
            for (int j = 3; j >= 0; --j) {
                unsigned int other_dim_index = getBroadcastIndex(i, j);
                other_index += other_dim_index * stride;
                stride *= others.shape[j];
            }
            data[i] += others.data[other_index];
        }

        return *this;
    }
};
// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    {
        unsigned int shape[]{1, 2, 3, 4};
        // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        auto t0 = Tensor4D(shape, data);
        auto t1 = Tensor4D(shape, data);
        t0 += t1;
        for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,

            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        // clang-format on
        unsigned int s1[]{1, 2, 3, 1};
        // clang-format off
        float d1[]{
            6,
            5,
            4,

            3,
            2,
            1};
        // clang-format on

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == 7.f, "Every element of t0 should be 7 after adding t1 to it.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        unsigned int s1[]{1, 1, 1, 1};
        double d1[]{1};

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
    }
}
