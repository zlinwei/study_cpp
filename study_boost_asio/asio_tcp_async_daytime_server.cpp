//
// Created by Administrator on 2019/7/8.
//

//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string() {
    using namespace std; // For time_t, time and ctime;
    time_t now = time(nullptr);
    return ctime(&now);
}

class tcp_connection
        : public boost::enable_shared_from_this<tcp_connection> {
public:
    typedef boost::shared_ptr<tcp_connection> sptr;

    static sptr create(boost::asio::io_context &io_context) {
        return sptr(new tcp_connection(io_context));
    }

    tcp::socket &socket() {
        return socket_;
    }

    void start() {
        message_ = make_daytime_string();
        std::cout << "8" << std::endl;
        boost::asio::async_write(socket_, boost::asio::buffer(message_),
                                 boost::bind(&tcp_connection::handle_write, shared_from_this(),
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));
        std::cout << "9" << std::endl;
    }

    ~tcp_connection() {
        std::cout << "~connection()\n" << std::flush;
    }

private:
    explicit tcp_connection(boost::asio::io_context &io_context)
            : socket_(io_context) {
    }

    void handle_write(const boost::system::error_code & /*error*/,
                      size_t /*bytes_transferred*/) {
        std::cout << "10" << std::endl;
    }

    tcp::socket socket_;
    std::string message_;
};

class tcp_server {
public:
    explicit tcp_server(boost::asio::io_context &io_context)
            : io_context_(io_context),
              acceptor_(io_context, tcp::endpoint(tcp::v4(), 13)) {
        std::cout << "1" << std::endl;
        start_accept();
        std::cout << "2" << std::endl;
    }

private:
    void start_accept() {
        tcp_connection::sptr new_connection =
                tcp_connection::create(io_context_);
        std::cout << "3" << std::endl;
        acceptor_.async_accept(new_connection->socket(),
                               boost::bind(&tcp_server::handle_accept, this, new_connection,
                                           boost::asio::placeholders::error));
        std::cout << "4" << std::endl;
    }

    void handle_accept(tcp_connection::sptr new_connection,
                       const boost::system::error_code &error) {
        std::cout << "5" << std::endl;
        if (!error) {
            new_connection->start();
        }

        std::cout << "6" << std::endl;
        start_accept();
        std::cout << "7" << std::endl;
    }

    boost::asio::io_context &io_context_;
    tcp::acceptor acceptor_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        tcp_server server(io_context);
        io_context.run();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}