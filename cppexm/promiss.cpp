#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <atomic>  // ������ԭ�ӱ���������ֹ��־

std::mutex mtx;
std::condition_variable cv;
std::queue<int> data_queue;  // �������
std::atomic<bool> stop(false);  // ��ֹ��־��ԭ�Ӳ�����֤�̰߳�ȫ��

// �������̣߳��ȴ����зǿղ��������ݣ�֧��������ֹ��
void consumer() {
    while (!stop) {  // ѭ��������δ�յ���ֹ�ź�
        std::unique_lock<std::mutex> lock(mtx);
        // �ȴ����������зǿ� �� �յ���ֹ�źţ�������ٻ��ѣ�
        cv.wait(lock, []{ 
            return !data_queue.empty() || stop; 
        });  

        // ���յ���ֹ�ź��Ҷ���Ϊ�գ��˳�ѭ��
        if (stop && data_queue.empty()) {
            break;
        }

        // �������ݣ��������зǿ�ʱ��
        if (!data_queue.empty()) {
            int data = data_queue.front();
            data_queue.pop();
            std::cout << "�������������ݣ�" << data << std::endl;
        }

        // lock.unlock();  // ��ǰ�ͷ�������ѡ������������Զ��ͷţ�
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // ģ�����Ѻ�ʱ
    }
    std::cout << "�������߳��˳�" << std::endl;
}

// �������̣߳��������������ݣ��̶����� 5 �κ������
void producer(int id) {
    for (int i = 0; i < 5; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));  // ģ��������ʱ
        int data = id * 100 + i;

        std::lock_guard<std::mutex> lock(mtx);
        data_queue.push(data);
        std::cout << "������ " << id << " �������ݣ�" << data << std::endl;
        cv.notify_one();  // �ؼ��Ľ���������֪ͨ�����ߣ��������������޵ȴ���
    }
    std::cout << "������ " << id << " ��������" << std::endl;
    
}

int main() {
    // �����������߳�
    std::thread consumer_thread(consumer);

    // ���� 2 ���������߳�
    std::thread producer1(producer, 1);
    std::thread producer2(producer, 2);

    // �ȴ������߽���
    producer1.join();
    producer2.join();

    // �����߽�����֪ͨ������ֹͣ���ȴ����˳�
    stop = true;  // ������ֹ��־
    cv.notify_one();  // ���������ߣ����������� wait ����

    // �ȴ��������߳������˳������ detach()��
    consumer_thread.join();

    std::cout << "�����߳̽����������˳�" << std::endl;
    return 0;
}