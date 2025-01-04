#include <boost/asio/awaitable.hpp>
#include <boost/core/ref.hpp>
#include <boost/fiber/algo/round_robin.hpp>
#include <boost/fiber/algo/shared_work.hpp>
#include <boost/fiber/fiber.hpp>
#include <boost/fiber/operations.hpp>
#include <boost/fiber/unbuffered_channel.hpp>
#include <chrono>
#include <concepts>
#include <ctime>
#include <iostream>

#include <boost/fiber/all.hpp>
#include <memory>
#include <ranges>
#include <ratio>
#include <string>
#include <vector>

#include "corutine_test.hpp"

using namespace corutine_test;

template <typename T>
auto generator(std::vector<T>& container, std::shared_ptr<boost::fibers::unbuffered_channel<std::string>> channel) -> void {
    for (const auto& i : container) {
        channel->push(i);
        boost::this_fiber::sleep_for(std::chrono::milliseconds(1000));
    }
}

auto get_current_time() -> std::string {
    static auto begin = std::chrono::system_clock::now();
    auto chrono_now = std::chrono::system_clock::now();
    return std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(chrono_now - begin).count());
}

auto listener(std::shared_ptr<boost::fibers::unbuffered_channel<std::string>> channel) -> void {
    for (const auto& i : *channel) {
        if (channel->is_closed()) {
            return;
        }
        std::cerr << "value: '"<< i << "', time: " << get_current_time() << "\n";
    }
}

auto corutine_test::main_impl(const boost::program_options::variables_map& /*args*/) -> void {
    std::vector<std::string> v = {"Hi", "dear", "world!!!"};
    auto channel = std::make_shared<boost::fibers::unbuffered_channel<std::string>>();

    std::array fibers = {
        boost::fibers::fiber {[channel, &v] { generator(v, channel); }},
        boost::fibers::fiber {[channel, &v] { generator(v, channel); }},
        boost::fibers::fiber {[channel, &v] { generator(v, channel); }},
        boost::fibers::fiber {[channel, &v] { generator(v, channel); }},
        boost::fibers::fiber {[channel, &v] { generator(v, channel); }},
        boost::fibers::fiber {[channel, &v] { generator(v, channel); }},
        boost::fibers::fiber {[channel, &v] { generator(v, channel); }},
        boost::fibers::fiber {[channel, &v] { generator(v, channel); }},
        boost::fibers::fiber {[channel, &v] { generator(v, channel); }},
        boost::fibers::fiber {[channel, &v] { generator(v, channel); }},
        boost::fibers::fiber {[channel, &v] { generator(v, channel); }},
        boost::fibers::fiber {[channel, &v] { generator(v, channel); }},
        boost::fibers::fiber {[channel] { listener(channel); }}
    };
    for (auto& fiber : fibers) {
        fiber.join();
    }
}
