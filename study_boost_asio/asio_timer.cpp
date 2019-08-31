//
// Created by Administrator on 2019/7/8.
//

#include <iostream>
#include <boost/asio.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>


void show_current_time(const char *what) {
    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << what << std::put_time(std::localtime(&time), "%F %T") << "\n";
}

//sync timer
void test_deadline_timer() {
    boost::asio::io_context io{};

    //构造一个deadline定时器,等到5s
    boost::asio::deadline_timer timer(io);
    timer.expires_from_now(boost::posix_time::seconds(5));
    show_current_time("before wait:");
    timer.wait();
    show_current_time("after wait:");
}

void test_async_deadline_timer() {
    boost::asio::io_context io{};
    boost::asio::deadline_timer timer(io);
    timer.expires_from_now(boost::posix_time::seconds(5));

    show_current_time("before async_wait:");

    timer.async_wait([](const boost::system::error_code &ec) {
        if (!ec)
            show_current_time("wait finished:");
    });

    show_current_time("after async_wait:");
    io.run_one();
}

void test_async_deadline_poll() {
    boost::asio::io_context io{};
    boost::asio::deadline_timer timer(io);
    timer.expires_from_now(boost::posix_time::seconds(5));

    show_current_time("before async_wait:");

    timer.async_wait([](const boost::system::error_code &ec) {
        if (!ec)
            show_current_time("wait finished:");
    });

    show_current_time("after async_wait:");
    auto now = std::chrono::high_resolution_clock::now();
    while (std::chrono::high_resolution_clock::now() < now + std::chrono::seconds(10)) {
        int events = io.poll();
        if (events == 0)continue;
        std::cout << events << std::endl;
        show_current_time("poll:");
    }

    show_current_time("exit:");

}

int main(int argc, char *argv[]) {
//    test_deadline_timer();
//    test_async_deadline_timer();
    test_async_deadline_poll();
    return 0;
}