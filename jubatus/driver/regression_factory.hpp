// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2012 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
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

#ifndef JUBATUS_DRIVER_REGRESSION_FACTORY_HPP_
#define JUBATUS_DRIVER_REGRESSION_FACTORY_HPP_

#include <string>

namespace jubatus {
namespace core {

namespace storage {

class storage_base;

}  // namespace storage

namespace common {
namespace jsonconfig {

class config;

}  // namespace jsonconfig
}  // namespace common

namespace regression {

class regression_base;

}  // namespace regression
}  // namespace core

namespace driver {

class regression_factory {
 public:
  static core::regression::regression_base* create_regression(
      const std::string& name,
      const core::common::jsonconfig::config& param,
      core::storage::storage_base* storage);
};

}  // namespace driver
}  // namespace jubatus

#endif  // JUBATUS_DRIVER_REGRESSION_FACTORY_HPP_