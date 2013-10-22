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

#include "jubatus/core/common/vector_util.hpp"
#include "jubatus/core/fv_converter/datum.hpp"
#include "jubatus/core/fv_converter/datum_to_fv_converter.hpp"
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
    const pfi::lang::shared_ptr<core::classifier::multiclass_classifier>& classifier_method,
    const pfi::lang::shared_ptr<core::fv_converter::datum_to_fv_converter>& converter)
    : converter_(converter)
    , classifier_(classifier_method)
    , mixable_classifier_model_(classifier_method->storage())
    , wm_(core::framework::mixable_weight_manager::model_ptr(new weight_manager))
  {

  converter_->set_weight_manager(wm_.get_model());
}

classifier::~classifier() {
}

pfi::lang::shared_ptr<core::storage::storage_base> classifier::get_model() const {
  return classifier_->storage();
}

void classifier::train(const string label, const datum& data) {
  sfv_t v;
  converter_->convert_and_update_weight(data, v);
  core::common::sort_and_merge(v);
  classifier_->train(v, label);
}

core::classifier::classify_result classifier::classify(
    const datum& data) const {
  sfv_t v;
  converter_->convert(data, v);

  core::classifier::classify_result scores;
  classifier_->classify_with_scores(v, scores);
  return scores;
}

void classifier::clear() {
  classifier_->storage()->clear();
}

void classifier::save(core::framework::msgpack_writer& writer) const {
  msgpack::packer<core::framework::msgpack_writer> pk(writer);
  pk.pack_array(2);
  classifier_->storage()->save(writer);
  wm_.get_model()->save(writer);
}

void classifier::load(msgpack::object& o) {
  if (o.type != msgpack::type::ARRAY || o.via.array.size != 2) {
    throw msgpack::type_error();
  }

  classifier_->storage()->load(o.via.array.ptr[0]);
  wm_.get_model()->load(o.via.array.ptr[1]);
}

}  // namespace driver
}  // namespace jubatus
