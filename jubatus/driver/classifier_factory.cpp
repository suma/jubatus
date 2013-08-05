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

#include "classifier_factory.hpp"

#include <string>

#include "jubatus/core/classifier/classifier.hpp"
#include "jubatus/core/common/exception.hpp"
#include "jubatus/core/common/jsonconfig.hpp"
#include "jubatus/core/storage/storage_base.hpp"

using jubatus::core::common::jsonconfig::config_cast_check;
using jubatus::core::classifier::multiclass_classifier;
using jubatus::core::classifier::classifier_config;
using jubatus::core::classifier::perceptron;
using jubatus::core::classifier::passive_aggressive;
using jubatus::core::classifier::passive_aggressive_1;
using jubatus::core::classifier::passive_aggressive_2;
using jubatus::core::classifier::confidence_weighted;
using jubatus::core::classifier::arow;
using jubatus::core::classifier::normal_herd;
using jubatus::core::storage::storage_base;

namespace jubatus {
namespace driver {

multiclass_classifier* classifier_factory::create_classifier(
    const std::string& name,
    const core::common::jsonconfig::config& param,
    storage_base* storage) {
  if (name == "perceptron") {
    // perceptron doesn't have parameter
    return new perceptron(storage);
  } else if (name == "PA" || name == "passive_aggressive") {
    // passive_aggressive doesn't have parameter
    return new passive_aggressive(storage);
  } else if (name == "PA1" || name == "passive_aggressive_1") {
    return new passive_aggressive_1(
        config_cast_check<classifier_config>(param), storage);
  } else if (name == "PA2" || name == "passive_aggressive_2") {
    return new passive_aggressive_2(
        config_cast_check<classifier_config>(param), storage);
  } else if (name == "CW" || name == "confidence_weighted") {
    return new confidence_weighted(
        config_cast_check<classifier_config>(param), storage);
  } else if (name == "AROW" || name == "arow") {
    return new arow(config_cast_check<classifier_config>(param), storage);
  } else if (name == "NHERD" || name == "normal_herd") {
    return new normal_herd(
        config_cast_check<classifier_config>(param), storage);
  } else {
    throw JUBATUS_EXCEPTION(core::common::unsupported_method(name));
  }
}

}  // namespace driver
}  // namespace jubatus
