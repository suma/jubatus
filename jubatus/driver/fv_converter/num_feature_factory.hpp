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

#ifndef JUBATUS_CORE_FV_CONVERTER_NUM_FEATURE_FACTORY_HPP_
#define JUBATUS_CORE_FV_CONVERTER_NUM_FEATURE_FACTORY_HPP_

#include <string>
#include <map>
#include "jubatus/core/fv_converter/num_feature.hpp"

namespace jubatus {
namespace driver {
namespace fv_converter {

class num_feature_factory {
 public:
  typedef std::map<std::string, std::string> param_t;
  jubatus::core::fv_converter::num_feature* create(const std::string& name, const param_t& params) const;
};

}  // namespace fv_converter
}  // namespace driver
}  // namespace jubatus

#endif  // JUBATUS_CORE_FV_CONVERTER_NUM_FEATURE_FACTORY_HPP_
