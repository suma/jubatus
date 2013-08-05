// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2012 Preferred Infrastracture and Nippon Telegraph and Telephone Corporation.
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

#include "anomaly_factory.hpp"

#include <string>
#include <pficommon/text/json.h>

#include "jubatus/core/anomaly/anomaly.hpp"
#include "jubatus/core/common/exception.hpp"
#include "jubatus/core/common/jsonconfig.hpp"
#include "recommender_factory.hpp"

using jubatus::core::anomaly::anomaly_base;
using jubatus::core::anomaly::lof;
using jubatus::core::storage::lof_storage;
using jubatus::core::common::jsonconfig::config;
using jubatus::core::common::jsonconfig::config_cast_check;
using pfi::text::json::json;
using std::string;

namespace jubatus {
namespace driver {

namespace {
struct anomaly_config {
  std::string method;  // nest engine name
  jubatus::core::common::jsonconfig::config parameter;

  template<typename Ar>
  void serialize(Ar& ar) {
    ar & MEMBER(method) & MEMBER(parameter);
  }
};
}

anomaly_base* anomaly_factory::create_anomaly(
    const string& name,
    const config& param) {
  if (name == "lof") {
    anomaly_config conf = config_cast_check<anomaly_config>(param);
    lof_storage::config config =
        config_cast_check<lof_storage::config>(param);
    return new lof(
        config,
        recommender_factory::create_recommender(
            conf.method,
            conf.parameter));
  } else {
    throw JUBATUS_EXCEPTION(jubatus::core::common::unsupported_method(name));
  }
};

}  // namespace driver
}  // namespace jubatus

