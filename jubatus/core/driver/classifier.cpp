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

#include "../classifier/classifier_factory.hpp"
#include "../common/vector_util.hpp"
#include "../fv_converter/datum.hpp"
#include "../fv_converter/datum_to_fv_converter.hpp"
#include "../fv_converter/converter_config.hpp"
#include "../storage/storage_factory.hpp"

using std::string;
using std::vector;
using std::pair;
using jubatus::core::framework::mixable_holder;
using jubatus::core::fv_converter::weight_manager;

namespace jubatus {
namespace core {
namespace driver {

classifier::classifier(
    storage::storage_base* model_storage,
    jubatus::core::classifier::multiclass_classifier* classifier_method,
    pfi::lang::shared_ptr<fv_converter::datum_to_fv_converter> converter)
    : mixable_holder_(new mixable_holder),
      converter_(converter),
      classifier_(classifier_method) {
  mixable_classifier_model_.set_model(
      linear_function_mixer::model_ptr(model_storage));
  wm_.set_model(mixable_weight_manager::model_ptr(new weight_manager));

  mixable_holder_->register_mixable(&mixable_classifier_model_);
  mixable_holder_->register_mixable(&wm_);

  (*converter_).set_weight_manager(wm_.get_model());
}

classifier::~classifier() {
}

void classifier::train(const pair<string, fv_converter::datum>& data) {
  common::sfv_t v;
  converter_->convert_and_update_weight(data.second, v);
  common::sort_and_merge(v);
  classifier_->train(v, data.first);
}

jubatus::core::classifier::classify_result classifier::classify(
    const fv_converter::datum& data) const {
  common::sfv_t v;
  converter_->convert(data, v);

  jubatus::core::classifier::classify_result scores;
  classifier_->classify_with_scores(v, scores);
  return scores;
}

void classifier::clear() {
  get_model()->clear();
  wm_.clear();
}

}  // namespace driver
}  // namespace core
}  // namespace jubatus
