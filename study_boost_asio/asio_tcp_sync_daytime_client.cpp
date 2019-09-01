//
// Created by linwei on 2019-09-01.
//

//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int, char *[]) {
    try {
        std::string host = "localhost";

        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints =
                resolver.resolve(host, "daytime");

        for (const auto &endpoint : endpoints) {
            std::cout << endpoint.host_name() << " " <<
                      endpoint.service_name() << " " <<
                      endpoint.endpoint().address().to_string() << " " <<
                      endpoint.endpoint().port() << std::endl;
        }

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        for (;;) {
            boost::array<char, 128> buf{0};
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);
            std::cout << "received " << len << " bytes" << std::endl;

            if (error == boost::asio::error::eof)
            {
                std::cout << "eof" << std::endl;
                break; // Connection closed cleanly by peer.
            }
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            std::cout.write(buf.data(), len);
        }
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}