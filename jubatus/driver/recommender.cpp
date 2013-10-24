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

#include "recommender.hpp"

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
using jubatus::core::common::sfv_t;
using jubatus::core::fv_converter::datum;
using jubatus::core::fv_converter::weight_manager;
using pfi::lang::shared_ptr;

namespace jubatus {
namespace driver {

recommender::recommender(
    const pfi::lang::shared_ptr<core::recommender::recommender_base>& method,
    const pfi::lang::shared_ptr<core::fv_converter::datum_to_fv_converter>& converter)
    : converter_(converter),
      recommender_(method),
      wm_(core::framework::mixable_weight_manager::model_ptr(
            new weight_manager)) {
  converter_->set_weight_manager(wm_.get_model());
}

recommender::~recommender() {
}

void recommender::clear_row(const std::string& id) {
  recommender_->clear_row(id);
}

void recommender::update_row(
    const std::string& id,
    const datum& dat) {
  core::recommender::sfv_diff_t v;
  converter_->convert_and_update_weight(dat, v);
  recommender_->update_row(id, v);
}

void recommender::clear() {
  recommender_->clear();
  wm_.get_model()->clear();
}

datum recommender::complete_row_from_id(const std::string& id) {
  sfv_t v;
  recommender_->complete_row(id, v);

  datum ret;
  core::fv_converter::revert_feature(v, ret);
  return ret;
}

datum recommender::complete_row_from_datum(
    const datum& dat) {
  sfv_t u, v;
  converter_->convert(dat, u);
  recommender_->complete_row(u, v);

  datum ret;
  core::fv_converter::revert_feature(v, ret);
  return ret;
}

std::vector<std::pair<std::string, float> > recommender::similar_row_from_id(
    const std::string& id,
    size_t ret_num) {
  std::vector<std::pair<std::string, float> > ret;
  recommender_->similar_row(id, ret, ret_num);
  return ret;
}

std::vector<std::pair<std::string, float> >
recommender::similar_row_from_datum(
    const datum& data,
    size_t size) {
  sfv_t v;
  converter_->convert(data, v);

  std::vector<std::pair<std::string, float> > ret;
  recommender_->similar_row(v, ret, size);
  return ret;
}

float recommender::calc_similality(
    const datum& l,
    const datum& r) {
  sfv_t v0, v1;
  converter_->convert(l, v0);
  converter_->convert(r, v1);
  return jubatus::core::recommender::recommender_base::calc_similality(v0, v1);
}

float recommender::calc_l2norm(const datum& q) {
  sfv_t v0;
  converter_->convert(q, v0);
  return jubatus::core::recommender::recommender_base::calc_l2norm(v0);
}

datum recommender::decode_row(const std::string& id) {
  sfv_t v;
  recommender_->decode_row(id, v);

  datum ret;
  core::fv_converter::revert_feature(v, ret);
  return ret;
}

std::vector<std::string> recommender::get_all_rows() {
  std::vector<std::string> ret;
  recommender_->get_all_row_ids(ret);
  return ret;
}

void recommender::save(core::framework::msgpack_writer& writer) const {
  msgpack::packer<core::framework::msgpack_writer> pk(writer);
  pk.pack_array(2);
  recommender_->save(writer);
  wm_.get_model()->save(writer);
}

void recommender::load(msgpack::object& o) {
  if (o.type != msgpack::type::ARRAY || o.via.array.size != 2) {
    throw msgpack::type_error();
  }

  recommender_->load(o.via.array.ptr[0]);
  wm_.get_model()->load(o.via.array.ptr[1]);
}

}  // namespace driver
}  // namespace jubatus
