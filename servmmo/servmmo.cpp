#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <iostream>
#include <asio.hpp>

void print(const std::error_code& e)
{
    std::cout << "Guten Tag, Welt!" << std::endl;
}

int main()
{

    asio::error_code ec;
    asio::io_context context;
    asio::steady_timer timer(context, asio::chrono::seconds(5));
    
    // Blocking wait.
    //timer.wait();
    //std::cout << "Guten Tag, Welt!" << std::endl;

    timer.async_wait(&print);
    context.run();
    return 0;
}