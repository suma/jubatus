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

#ifndef JUBATUS_PLUGIN_FV_CONVERTER_MECAB_SPLITTER_HPP_
#define JUBATUS_PLUGIN_FV_CONVERTER_MECAB_SPLITTER_HPP_

#include <map>
#include <string>
#include <utility>
#include <vector>
#include <mecab.h>
#include "jubatus/util/lang/scoped_ptr.h"

#include "jubatus/core/fv_converter/word_splitter.hpp"

namespace jubatus {
namespace plugin {
namespace fv_converter {

class mecab_splitter : public jubatus::core::fv_converter::word_splitter {
 public:
  mecab_splitter();
  explicit mecab_splitter(const char* arg);

  void split(const std::string& string,
             std::vector<std::pair<size_t, size_t> >& ret_boundaries) const;

 private:
  jubatus::util::lang::scoped_ptr<MeCab::Model> model_;
};

}  // namespace fv_converter
}  // namespace plugin
}  // namespace jubatus

extern "C" {
jubatus::plugin::fv_converter::mecab_splitter* create(
    const std::map<std::string, std::string>& params);
std::string version();
}

#endif  // JUBATUS_PLUGIN_FV_CONVERTER_MECAB_SPLITTER_HPP_
