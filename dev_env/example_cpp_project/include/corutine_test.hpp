#ifndef FILE_SERVER_HPP
#define FILE_SERVER_HPP

#include <boost/program_options/variables_map.hpp>

namespace corutine_test {

    auto main_impl(const boost::program_options::variables_map& args) -> void;

}  // namespace corutine_test

#endif  // FILE_SERVER_HPP
