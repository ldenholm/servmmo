#include <iostream>


#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include<chrono>
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

// General use buffer for reads: 1kb.
std::vector<char> vBuffer(1 * 1024);

void ReadSomeData(asio::ip::tcp::socket& socket)
{
    // Use lambda here for the handler arg.
    socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
        [&](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                std::cout << "\n\nRead " << length << " bytes\n\n";

                for (int i = 0; i < length; i++)
                {
                    std::cout << vBuffer[i];
                }

                /* Note: not recursion, simply pushing another asio task
                   to the context. For large payloads it will read over
                   say 5 read ops. Once a reading is complete the task remains
                   in the queue for future read ops.
                */
                ReadSomeData(socket);
            }
        }
    );
}

int main()
{

    asio::error_code ec;
    asio::io_context context;

    asio::executor_work_guard<asio::io_context::executor_type> ctx_work_guard = asio::make_work_guard(context);
    
    // Start up context
    std::thread tContext = std::thread([&]() { context.run(); });

    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1", ec), 80);

    asio::ip::tcp::socket socket(context);

    socket.connect(endpoint, ec);

    if (!ec)
    {
        std::cout << "Successfully connected." << std::endl;
    }
    else {
        std::cout << "Error establishing connection.\n" << ec.message() << std::endl;
    }

    if (socket.is_open())
    {
        // Push read instruction to asio queue.
        ReadSomeData(socket);

        std::string req =
            "GET /index.html HTTP/1.1\r\n"
            "HOST: example.com\r\n"
            "Connection: close\r\n\r\n";

        socket.write_some(asio::buffer(req.data(), req.size()), ec);

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(20000ms);
        //ctx_work_guard.reset();
        context.stop();

        // Await completion of context thread.
        if (tContext.joinable()) tContext.join();
    }

    return 0;
}