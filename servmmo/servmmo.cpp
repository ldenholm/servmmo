#include <iostream>


#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

int main()
{

    asio::error_code ec;
    asio::io_context context;
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("192.250.237.69", ec), 443);

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
        std::string req =
            "GET /index.html HTTP/1.1\r\n"
            "HOST: darkswordminiatures.com\r\n"
            "Connection: close\r\n\r\n";

        socket.write_some(asio::buffer(req.data(), req.size()), ec);
    }

    return 0;
}