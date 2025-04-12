#include "threadsafe_queue.h"
#include <thread>
#include <iostream>

int main() 
{
    Queue<int> queue(2);

    std::thread writer([&]() {
        queue.Push(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        queue.Push(2);
        queue.Push(3);
        queue.Push(4); // Drops 2
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        queue.Push(5);
        });

    std::thread reader([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(150)); // Delay reader
        std::cout << queue.Pop() << std::endl; // 1
        std::cout << queue.Pop() << std::endl; // 3
        std::cout << queue.Pop() << std::endl; // 4
        try 
        {
            std::cout << queue.PopWithTimeout(500) << std::endl; // 5
        }
        catch (const std::exception& e) 
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
        });

    writer.join();
    reader.join();
    return 0;
}
