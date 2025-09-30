#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <atomic>  // 新增：原子变量用于终止标志

std::mutex mtx;
std::condition_variable cv;
std::queue<int> data_queue;  // 共享队列
std::atomic<bool> stop(false);  // 终止标志（原子操作保证线程安全）

// 消费者线程：等待队列非空并消费数据（支持优雅终止）
void consumer() {
    while (!stop) {  // 循环条件：未收到终止信号
        std::unique_lock<std::mutex> lock(mtx);
        // 等待条件：队列非空 或 收到终止信号（避免虚假唤醒）
        cv.wait(lock, []{ 
            return !data_queue.empty() || stop; 
        });  

        // 若收到终止信号且队列为空，退出循环
        if (stop && data_queue.empty()) {
            break;
        }

        // 消费数据（仅当队列非空时）
        if (!data_queue.empty()) {
            int data = data_queue.front();
            data_queue.pop();
            std::cout << "消费者消费数据：" << data << std::endl;
        }

        // lock.unlock();  // 提前释放锁（可选，作用域结束自动释放）
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // 模拟消费耗时
    }
    std::cout << "消费者线程退出" << std::endl;
}

// 生产者线程：向队列中添加数据（固定生产 5 次后结束）
void producer(int id) {
    for (int i = 0; i < 5; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));  // 模拟生产耗时
        int data = id * 100 + i;

        std::lock_guard<std::mutex> lock(mtx);
        data_queue.push(data);
        std::cout << "生产者 " << id << " 生产数据：" << data << std::endl;
        cv.notify_one();  // 关键改进：生产后通知消费者（避免消费者无限等待）
    }
    std::cout << "生产者 " << id << " 结束生产" << std::endl;
    
}

int main() {
    // 启动消费者线程
    std::thread consumer_thread(consumer);

    // 启动 2 个生产者线程
    std::thread producer1(producer, 1);
    std::thread producer2(producer, 2);

    // 等待生产者结束
    producer1.join();
    producer2.join();

    // 生产者结束后，通知消费者停止并等待其退出
    stop = true;  // 设置终止标志
    cv.notify_one();  // 唤醒消费者（可能阻塞在 wait 处）

    // 等待消费者线程优雅退出（替代 detach()）
    consumer_thread.join();

    std::cout << "所有线程结束，程序退出" << std::endl;
    return 0;
}