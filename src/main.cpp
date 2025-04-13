#include "threadsafe_queue.h"
#include <thread>
#include <iostream>
#include <chrono>

int main()
{
    Queue<int> queue(2); // size = 2

    std::thread writer([&]() {
        queue.Push(1); // Queue: [1]
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        queue.Push(2); // Queue: [2]
        queue.Push(3); // Queue: [2, 3] 
        queue.Push(4); // Queue: [3, 4] (2 dropped)
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        queue.Push(5); // Queue: [5] — will wake the blocked reader
        });

    std::thread reader([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::cout << "Pop: " << queue.Pop() << std::endl; // print 1

        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        std::cout << "Pop: " << queue.Pop() << std::endl; // print 3
        std::cout << "Pop: " << queue.Pop() << std::endl; // print 4

        try {
            std::cout << "PopWithTimeout: " << queue.PopWithTimeout(500) << std::endl; // Blocks until 5
            std::cout << "PopWithTimeout: " << queue.PopWithTimeout(300) << std::endl; // queue is empty and throw an exception
        }
        catch (const std::exception& e) {
            std::cout << "Timeout: " << e.what() << std::endl;
        }
        });

    writer.join();
    reader.join();

    return 0;
}
