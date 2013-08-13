// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2013 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
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

#include "mixable_weight_manager.hpp"

#include <pficommon/data/serialization.h>

#include "../fv_converter/weight_manager.hpp"

using pfi::lang::shared_ptr;
using jubatus::core::fv_converter::keyword_weights;
using jubatus::core::fv_converter::weight_manager;

namespace jubatus {
namespace core {
namespace framework {

keyword_weights mixable_weight_manager::get_diff_impl() const {
  return get_model()->get_diff();
}

void mixable_weight_manager::put_diff_impl(
    const fv_converter::keyword_weights& diff) {
  get_model()->put_diff(diff);
}

void mixable_weight_manager::mix_impl(
    const keyword_weights& lhs,
    const keyword_weights& rhs,
    keyword_weights& acc) const {
  acc = rhs;
  acc.merge(lhs);
}

void mixable_weight_manager::clear() {
  get_model()->clear();
}

namespace {

class weight_diff : public diff_object_raw {
 public:
  void convert_binary(msgpack_writer& writer) {
    msgpack::pack(writer, weights_);
  }

  keyword_weights weights_;
};

}  // namespace

linear_mixable_weight_manager::linear_mixable_weight_manager(shared_ptr<weight_manager> wm)
  : weight_manager_(wm) {
}

void linear_mixable_weight_manager::save(msgpack_writer& writer) {
  msgpack::pack(writer, *weight_manager_);
}

void linear_mixable_weight_manager::load(msgpack::object& obj) {
  obj.convert(weight_manager_.get());
}

diff_object linear_mixable_weight_manager::convert_diff_object(const msgpack::object& obj) const {
  weight_diff* diff = new weight_diff;
  obj.convert(&diff->weights_);
  return diff_object(diff);
}

void linear_mixable_weight_manager::mix(const msgpack::object& obj, diff_object out) const {
  keyword_weights lhs;
  weight_diff* diff = dynamic_cast<weight_diff*>(out.get());
  if (!diff) {
    // TODO(suma): jubatus exception? or other error handling?
    throw std::bad_cast();
  }
  obj.convert(&lhs);

  diff->weights_.merge(lhs);
}

void linear_mixable_weight_manager::get_diff(msgpack_writer& writer) {
  msgpack::pack(writer, weight_manager_->get_diff());
}

void linear_mixable_weight_manager::put_diff(const msgpack::object& obj) {
  keyword_weights diff;
  obj.convert(&diff);
  weight_manager_->put_diff(diff);
}

}  // namespace framework
}  // namespace core
}  // namespace jubatus
