// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2011 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include <map>
#include <string>
#include <gtest/gtest.h>
#include "dynamic_string_filter.hpp"
#include "jubatus/core/fv_converter/exception.hpp"

using jubatus::core::fv_converter::converter_exception;

namespace jubatus {
namespace server {
namespace fv_converter {

TEST(dynamic_string_filter, trivial) {
  std::map<std::string, std::string> params;

  dynamic_string_filter f(LIBFILTER_SAMPLE,
      "create",
      params);
  std::string out;
  f.filter("hoge-hoge", out);
  EXPECT_EQ("hoge hoge", out);
}

TEST(dynamic_string_filter, unknown_file) {
  std::map<std::string, std::string> params;
  EXPECT_THROW(
      dynamic_string_filter f("unkonwn_file.so",
          "create",
          params),
      converter_exception);
}

TEST(dynamic_string_filter, unknown_function) {
  std::map<std::string, std::string> params;
  EXPECT_THROW(
      dynamic_string_filter f(LIBFILTER_SAMPLE,
          "unknown_function",
          params),
      converter_exception);
}

}  // namespace fv_converter
}  // namespace server
}  // namespace jubatus
