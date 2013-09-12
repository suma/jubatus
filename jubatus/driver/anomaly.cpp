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

#include "anomaly.hpp"

#include <string>
#include <utility>
#include <vector>

#include <pficommon/lang/shared_ptr.h>

#include "jubatus/core/common/vector_util.hpp"
#include "jubatus/core/fv_converter/datum.hpp"
#include "jubatus/core/fv_converter/datum_to_fv_converter.hpp"
#include "jubatus/core/fv_converter/revert.hpp"
#include "../driver/fv_converter/converter_config.hpp"

using std::string;
using std::vector;
using std::pair;
using jubatus::core::anomaly::anomaly_base;
using jubatus::core::common::sfv_t;
using jubatus::core::fv_converter::datum;
using jubatus::core::fv_converter::weight_manager;
using pfi::lang::shared_ptr;

namespace jubatus {
namespace driver {

anomaly::anomaly(
    jubatus::core::anomaly::anomaly_base* anomaly_method,
    pfi::lang::shared_ptr<core::fv_converter::datum_to_fv_converter> converter)
    : mixable_holder_(new mixable_holder),
      converter_(converter),
      anomaly_(anomaly_method),
      wm_(core::framework::mixable_weight_manager::model_ptr(new weight_manager))
  {

  mixable_holder_->register_mixable(anomaly_->get_linear_mixable());
  mixable_holder_->register_mixable(&wm_);

  (*converter_).set_weight_manager(wm_.get_model());
}

anomaly::~anomaly() {
}

void anomaly::clear_row(const std::string& id) {
  anomaly_->clear_row(id);
}

pair<string, float> anomaly::add(
    const string& id,
    const datum& d) {
  float score = this->update(id, d);
  return make_pair(id, score);
}

float anomaly::update(const string& id, const datum& d) {
  sfv_t v;
  converter_->convert_and_update_weight(d, v);

  anomaly_->update_row(id, v);
  return anomaly_->calc_anomaly_score(id);
}

void anomaly::clear() {
  anomaly_->clear();
}

float anomaly::calc_score(const datum& d) const {
  sfv_t v;
  converter_->convert(d, v);
  return anomaly_->calc_anomaly_score(v);
}

vector<string> anomaly::get_all_rows() const {
  vector<string> ids;
  anomaly_->get_all_row_ids(ids);
  return ids;
}

void anomaly::save(core::framework::msgpack_writer& writer) const {
  msgpack::packer<core::framework::msgpack_writer> pk(writer);
  pk.pack_array(2);
  anomaly_->save(writer);
  wm_.get_model()->save(writer);
}

void anomaly::load(msgpack::object& o) {
  if (o.type != msgpack::type::ARRAY || o.via.array.size != 2) {
    throw msgpack::type_error();
  }

  anomaly_->load(o.via.array.ptr[0]);
  wm_.get_model()->load(o.via.array.ptr[1]);
}

}  // namespace driver
}  // namespace jubatus
