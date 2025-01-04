#include <iostream>

#include "boost/program_options/options_description.hpp"
#include "boost/program_options/parsers.hpp"
#include "boost/program_options/value_semantic.hpp"
#include "boost/program_options/variables_map.hpp"

#include "corutine_test.hpp"

namespace po = boost::program_options;

auto parse_programm_options(int argc, const char* argv[]) -> po::variables_map {
    auto desc = po::options_description("Simple c++ file exchange server");
    desc.add_options()("help,h", "Show help")("port,p", po::value<int>());
    auto parsed = po::command_line_parser(argc, argv).options(desc).run();

    auto result = po::variables_map();
    po::store(parsed, result);
    po::notify(result);

    if (!result["help"].empty()) {
        std::cout << desc << std::endl;
    }

    return result;
}

auto main(int argc, const char* argv[]) -> int {
    auto args = parse_programm_options(argc, argv);
    if (!args["help"].empty()) {
        return 0;
    }

    corutine_test::main_impl(args);
    return 0;
}
