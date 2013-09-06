// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2011,2012 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
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

#include "classifier.hpp"

#include <string>
#include <utility>
#include <vector>

#include "jubatus/core/classifier/classifier_factory.hpp"
#include "jubatus/core/common/vector_util.hpp"
#include "jubatus/core/fv_converter/datum.hpp"
#include "jubatus/core/fv_converter/datum_to_fv_converter.hpp"
#include "jubatus/core/storage/storage_factory.hpp"
#include "jubatus/driver/fv_converter/converter_config.hpp"
#include "mixable_holder.hpp"

using std::string;
using std::vector;
using std::pair;
using jubatus::core::common::sfv_t;
using jubatus::core::fv_converter::datum;
using jubatus::core::fv_converter::weight_manager;

namespace jubatus {
namespace driver {

classifier::classifier(
    jubatus::core::storage::storage_base* model_storage,
    jubatus::core::classifier::multiclass_classifier* classifier_method,
    pfi::lang::shared_ptr<core::fv_converter::datum_to_fv_converter> converter)
    : mixable_holder_(new mixable_holder)
    , converter_(converter)
    , classifier_(classifier_method)
    , mixable_classifier_model_(
          core::framework::linear_function_mixer::model_ptr(model_storage))
    , wm_(core::framework::mixable_weight_manager::model_ptr(new weight_manager))
  {

  // TODO: set models
  //       set mixables
  //mixable_holder_->register_mixable(&mixable_classifier_model_);
  //mixable_holder_->register_mixable(&wm_);

  (*converter_).set_weight_manager(wm_.get_model());
}

classifier::~classifier() {
}

void classifier::train(const pair<string, datum>& data) {
  sfv_t v;
  converter_->convert_and_update_weight(data.second, v);
  core::common::sort_and_merge(v);
  classifier_->train(v, data.first);
}

jubatus::core::classifier::classify_result classifier::classify(
    const datum& data) const {
  sfv_t v;
  converter_->convert(data, v);

  jubatus::core::classifier::classify_result scores;
  classifier_->classify_with_scores(v, scores);
  return scores;
}

void classifier::clear() {
  get_model()->clear();
}

}  // namespace driver
}  // namespace jubatus
