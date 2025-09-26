#include <coroutine>   // 协程相关头文件
#include <iostream>    // 输入输出
#include <optional>    // 可选类型（当前代码未直接使用，但协程中可能涉及）

// 自定义生成器类型（C++23 标准库未提供生成器，需手动实现）
// 模板参数 T 表示生成器产生的值类型（如 int、string 等）
template <typename T>
struct Generator {
    // 协程的「承诺类型」（Promise Type），负责管理协程的状态和行为
    // 是协程与生成器交互的核心接口
    struct promise_type {
        T current_value;  // 存储当前生成的值（通过 co_yield 传递）

        // 返回一个 Generator 对象，包装当前协程的句柄
        // get_return_object 是协程必须实现的接口，用于构造返回值
        Generator get_return_object() {
            // 通过协程句柄的 from_promise 方法，从当前 promise 对象创建句柄
            return Generator(handle_type::from_promise(*this));
        }

        // 初始挂起策略：协程启动时是否挂起
        // std::suspend_always 表示协程启动后立即挂起，等待外部调用 resume 恢复
        std::suspend_always initial_suspend() { return {}; }

        // 最终挂起策略：协程结束时是否挂起
        // std::suspend_always 表示协程执行完毕后挂起（避免被销毁）
        std::suspend_always final_suspend() noexcept { return {}; }

        // 处理未捕获的异常：默认行为是终止程序
        void unhandled_exception() { std::terminate(); }

        // 当协程通过 co_yield 产生值时调用此方法
        // 参数 value 是 co_yield 传递的值，保存到 current_value
        // 返回值决定协程是否挂起（std::suspend_always 表示挂起，等待下次 resume）
        std::suspend_always yield_value(T value) {
            current_value = value;  // 保存当前生成的值
            return {};  // 返回挂起策略（此处总是挂起）
        }
    };

    // 协程句柄类型，用于管理协程的生命周期（恢复、销毁等）
    using handle_type = std::coroutine_handle<promise_type>;

    // 构造函数：接受协程句柄并保存
    explicit Generator(handle_type h) : handle(h) {}

    // 析构函数：如果句柄有效，销毁协程（释放资源）
    ~Generator() { if (handle) handle.destroy(); }

    // 调用生成器对象时，恢复协程并返回当前生成的值
    T operator()() {
        handle.resume();  // 恢复协程执行（从上次挂起的位置继续）
        return handle.promise().current_value;  // 返回本次生成的值
    }

private:
    handle_type handle;  // 协程句柄，管理协程的生命周期
};

// 斐波那契数列生成器协程（无限生成斐波那契数）
// 返回类型为自定义的 Generator<int>，表示生成 int 类型的值
Generator<int> fibonacci() {
    int a = 0, b = 1;  // 初始斐波那契数（a=0, b=1）

    // 无限循环生成斐波那契数（协程不会主动终止，除非外部停止调用）
    while (true) {
        co_yield a;  // 生成当前值 a，并挂起协程（等待下次调用 operator() 恢复）

        // 更新 a 和 b 为下一个斐波那契数（a 变为 b，b 变为 a+b）
        std::tie(a, b) = std::make_pair(b, a + b);
    }
}

int main() {
    // 创建斐波那契生成器实例（调用 fibonacci() 协程，返回 Generator 对象）
    auto fib = fibonacci();

    // 循环调用生成器的 operator()，获取前 10 个斐波那契数
    for (int i = 0; i < 10; ++i) {
        std::cout << fib() << " ";  // 输出：0 1 1 2 3 5 8 13 21 34
    }
    return 0;
}