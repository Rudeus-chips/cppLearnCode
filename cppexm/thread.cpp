#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>
#include <future>
#include <iostream>
#include <atomic>

/**
 * @brief 线程池类，用于管理和复用多个线程执行任务
 */
class ThreadPool {
public:
    /**
     * @brief 构造函数，创建指定数量的工作线程
     * @param numThreads 线程数量
     */
    ThreadPool(size_t numThreads) : stop(false) {
        for(size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] { workerThread(); });  // 创建工作线程并启动
        }
    }

    /**
     * @brief 向线程池提交任务
     * @param f 要执行的任务函数
     * @return std::future<decltype(f())> 用于获取任务返回值的future
     */
    template<class F>
    auto enqueue(F&& f) -> std::future<decltype(f())> {
        using ReturnType = decltype(f());
        auto task = std::make_shared<std::packaged_task<ReturnType()>>(  // 打包任务
            std::forward<F>(f)
        );
        
        std::future<ReturnType> res = task->get_future();  // 获取任务的future
        {
            std::unique_lock lock(queueMutex);
            tasks.emplace([task]() { (*task)(); });  // 将任务加入队列
            active_tasks++;  // 增加活跃任务计数
        }
        condition.notify_one();  // 通知一个工作线程有新任务
        return res;
    }

    /**
     * @brief 等待所有任务完成
     */
    void waitAll() {
        std::unique_lock lock(queueMutex);
        completion_cond.wait(lock, [this] {  // 等待直到所有任务完成
            return tasks.empty() && active_tasks == 0;
        });
        
    }

    /**
     * @brief 析构函数，清理线程池资源
     */
    ~ThreadPool() {
        waitAll();  // 等待所有任务完成
        {
            std::lock_guard lock(queueMutex);
            stop = true;  // 设置停止标志
        }
        condition.notify_all();  // 通知所有工作线程
        for (std::thread &worker : workers) {  // 等待所有工作线程结束
            if (worker.joinable()) worker.join();
        }
    }

private:
    /**
     * @brief 工作线程的主函数
     */
    void workerThread() {
        while(true) {
            std::function<void()> task;
            {
                std::unique_lock lock(queueMutex);
                condition.wait(lock, [this] {  // 等待直到有任务或需要停止
                    return stop || !tasks.empty();
                });
                
                if(stop && tasks.empty()) return;  // 如果需要停止且没有任务，则退出
                
                task = std::move(tasks.front());  // 获取任务
                tasks.pop();  // 从队列中移除任务
            }
            
            try {
                task();  // 执行任务
            } catch (const std::exception& e) {  // 捕获并处理异常
                std::cerr << "Task failed: " << e.what() << std::endl;
            } catch (...) {
                std::cerr << "Unknown task failure" << std::endl;
            }
            
            if (--active_tasks == 0) {  // 减少活跃任务计数
                completion_cond.notify_all();  // 如果没有活跃任务，通知等待的线程
            }
        }
    }

    std::vector<std::thread> workers;  // 工作线程容器
    std::queue<std::function<void()>> tasks;  // 任务队列
    
    std::mutex queueMutex;  // 互斥锁，用于保护任务队列
    std::condition_variable condition;  // 条件变量，用于通知有新任务
    std::condition_variable completion_cond;  // 条件变量，用于通知任务完成
    std::atomic<int> active_tasks{0};  // 原子变量，记录活跃任务数
    bool stop;  // 停止标志
};

/**
 * 主函数：演示线程池的使用
 * 创建一个线程池，提交多个任务，并收集结果
 */
int main() {
    // 创建一个包含4个线程的线程池
    ThreadPool pool(4);
    // 用于存储任务返回结果的容器
    std::vector<std::future<int>> results;
    // std::cout<< __cplusplus << std::endl;

    // 提交8个任务到线程池
    for(int i = 0; i < 8; ++i) {
        // 使用emplace_back添加任务，并获取future对象用于获取结果
        // 每个任务打印开始和结束信息，并返回i的平方
        results.emplace_back(
            pool.enqueue([i] {
                std::cout << "Task "<< i << " started\n";
                // 模拟任务执行，休眠1秒
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "Task "<< i << " finished\n";
                // 返回i的平方作为结果
                return i * i;
            })
        );
    }

    // 等待所有任务完成
    pool.waitAll();
    
    // 遍历所有结果并打印
    for (auto& result : results) {
        std::cout << "Result: " << result.get() << std::endl;
    }
    
    return 0;
}