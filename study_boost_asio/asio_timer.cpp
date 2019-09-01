//
// Created by Administrator on 2019/7/8.
//

#include <iostream>
#include <boost/asio.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <boost/bind.hpp>

void show_current_time(const char *what) {
    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << what << std::put_time(std::localtime(&time), "%F %T") << "\n";
}


//example 1
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

//example 2
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


//example 3
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

//example 4
void print(const boost::system::error_code & /*e*/,
           boost::asio::steady_timer *t, int *count) {
    if (*count < 5) {
        std::cout << *count << std::endl;
        ++(*count);
        t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));
        t->async_wait(boost::bind(print,
                                  boost::asio::placeholders::error, t, count));
    }

    //exit
}

void test_async_wait_always() {
    boost::asio::io_context io;
    int count = 0;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(1));

    t.async_wait(boost::bind(print,
                             boost::asio::placeholders::error, &t, &count));

    io.run();

    std::cout << "Final count is " << count << std::endl;
}


//example 5
class printer {
public:
    explicit printer(boost::asio::io_context &io)
            : timer_(io, boost::asio::chrono::seconds(1)),
              count_(0) {
        timer_.async_wait(boost::bind(&printer::print, this));
    }

    ~printer() {
        std::cout << "Final count is " << count_ << std::endl;
    }

    void print() {
        if (count_ < 5) {
            std::cout << count_ << std::endl;
            ++count_;

            timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(1));
            timer_.async_wait(boost::bind(&printer::print, this));
        }
    }

private:
    boost::asio::steady_timer timer_;
    int count_;
};

void test_async_wait_bind() {
    boost::asio::io_context io;
    printer p(io);
    io.run();
}


int main(int argc, char *argv[]) {
//    test_deadline_timer();
//    test_async_deadline_timer();
//    test_async_deadline_poll();
//    test_async_wait_always();
    test_async_wait_bind();
    return 0;
}