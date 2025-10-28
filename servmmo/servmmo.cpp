#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <iostream>
#include <asio.hpp>
#include <functional>

void print(const std::error_code& e,
    asio::steady_timer* t, int* count)
{
    if (*count < 5)
    {
        std::cout << *count << std::endl;
        (*count)++;
        t->expires_at(t->expiry() + asio::chrono::seconds(1));
        t->async_wait(std::bind(print,
            asio::placeholders::error, t, count));
        std::cout << "this is quick" << std::endl;
    }
    std::cout << "Guten Tag, Welt!" << std::endl;
}

int main()
{
    asio::io_context context;
    int count = 0;
    asio::steady_timer timer(context, asio::chrono::seconds(1));
    
    // Blocking wait.
    //timer.wait();
    //std::cout << "Guten Tag, Welt!" << std::endl;

    timer.async_wait(std::bind(print,
        asio::placeholders::error, &timer, &count));
    context.run();
    
    std::cout << "Final count is: " << count << std::endl;
    return 0;
}