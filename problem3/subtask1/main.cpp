#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <atomic>
#include <cstdlib>



int main() {
    size_t count(0);

    std::atomic_bool done(false);

    std::queue<int> items;

    std::mutex queue_mutex;

    std::thread producer([&]() {
        for (int i = 0; i < 10000; ++i) {
            // ... some code may be here ... for example, sleep
            std::this_thread::sleep_for(std::chrono::milliseconds(int(float(std::rand()) / RAND_MAX) * 10));

            std::lock_guard<std::mutex> lock(queue_mutex);
            items.push(i);
            count++;
        }
        done = true;
    });

    std::thread consumer([&]() {
        while (true) {
            std::lock_guard<std::mutex> lock(queue_mutex);
            if (!items.empty()) {
                items.pop();

                // ... some code may be here ... for example, sleep
                std::this_thread::sleep_for(std::chrono::milliseconds(int(float(std::rand()) / RAND_MAX) * 10));

                count--;

            } else if (done) {
                break;
            }
        }
    });

    producer.join();
    consumer.join();
    std::cout << "Count: " << count << std::endl;
    std::cout << "Queue size: " << items.size() << std::endl;
}