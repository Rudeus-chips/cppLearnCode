#include <iostream>
#include <fstream>
#include <thread>
#include <functional>
#include <stdexcept>

// ����ص��������ͣ�����Ϊ��ȡ���ֽ��������ݻ�������nullptr��ʾʧ�ܣ�
using ReadCallback = std::function<void(size_t bytes_read, const char* buffer)>;

/**
 * @brief �첽��ȡ�ļ��ĺ�������ͳ�ص���ʽ��
 * @param filename �ļ���
 * @param callback ��ȡ��ɺ���õĻص�����
 */
void async_read_file_traditional(const std::string& filename, ReadCallback callback) {
    // ����һ�����߳�ִ��ʵ�ʵ��ļ���ȡ������ģ���첽��
    std::thread([filename, callback]() {
        try {
            std::ifstream file(filename, std::ios::binary);
            if (!file.is_open()) {
                // ��ȡʧ��ʱ��ͨ���ص�����0�ֽں�nullptr
                callback(0, nullptr);
                return;
            }

            char buffer[1024]; // ��ȡ���������̶���С��
            file.read(buffer, sizeof(buffer)); // ͬ����ȡ��ʵ���첽Ӧʹ�÷�����API��
            size_t bytes_read = file.gcount(); // ��ȡʵ�ʶ�ȡ���ֽ���

            // ��ȡ��ɺ󣬵��ûص��������ݽ��
            callback(bytes_read, buffer);
        } catch (const std::exception& e) {
            // �쳣ʱͨ���ص����ݴ�����Ϣ���˴���Ϊ0�ֽڣ�
            callback(0, nullptr);
        }
    }).detach(); // �����̣߳���������ǰ�߳�
}

int main() {
    // ���ô�ͳ�첽��ȡ����������ص�������
    async_read_file_traditional("example.txt", 
        [](size_t bytes_read, const char* buffer) {
            if (bytes_read == 0) {
                std::cerr << "Error: Failed to read file or empty file." << std::endl;
                return;
            }
            std::cout << "��ͳ�첽��ȡ�����" << bytes_read << "�ֽڣ�: " << std::endl;
            std::cout.write(buffer, bytes_read); // �����ȡ������
            std::cout << std::endl;
        });

    // ���̼߳���ִ��������������������
    std::cout << "���̼߳�������..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2)); // ģ�����̼߳���ִ��
    std::cout << "���̼߳�������2" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1)); // �ȴ��첽�������

    return 0;
}