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

#include <string>
#include <algorithm>

#include <pficommon/lang/bind.h>
#include "linear_function_mixer.hpp"

using std::string;
using pfi::lang::bind;
using pfi::lang::_1;
using pfi::lang::_2;

using jubatus::core::storage::val3_t;
using jubatus::core::storage::feature_val3_t;
using jubatus::core::storage::features3_t;

namespace jubatus {
namespace core {
namespace framework {

namespace {

val3_t mix_val3(double w1, double w2, const val3_t& lhs, const val3_t& rhs) {
  return val3_t(
      (w1 * lhs.v1 + w2 * rhs.v1) / (w1 + w2),
      std::min(lhs.v2, rhs.v2),
      (w1 * lhs.v3 + w2 * rhs.v3) / (w1 + w2));
}

feature_val3_t mix_feature(
    double w1,
    double w2,
    const feature_val3_t& lhs,
    const feature_val3_t& rhs) {
  val3_t def(0, 1, 0);
  feature_val3_t ret(lhs);
  storage::detail::binop(ret, rhs, bind(mix_val3, w1, w2, _1, _2), def);
  return ret;
}

struct internal_diff_object : diff_object_raw {
  void convert_binary(msgpack_writer& writer) const {
    msgpack::pack(writer, diff_);
  }

  diffv diff_;
};

}  // namespace

void linear_function_mixer::mix(const diffv& lhs, diffv& mixed) const {
  features3_t l(lhs.v);
  const features3_t& r(mixed.v);
  storage::detail::binop(l, r, bind(mix_feature, lhs.count, mixed.count, _1, _2));
  mixed.v.swap(l);
  mixed.count += lhs.count;
}

void linear_function_mixer::get_diff(diffv& diff) const {
  diff.count = 1;  // TODO(kuenishi) mixer_->get_count();
  get_model()->get_diff(diff.v);
}

void linear_function_mixer::put_diff(const diffv& diff) {
  get_model()->set_average_and_clear_diff(diff.v);
}

diff_object linear_function_mixer::convert_diff_object(const msgpack::object& obj) const {
  internal_diff_object* diff = new internal_diff_object;
  diff_object diff_obj(diff);
  obj.convert(&diff->diff_);
  return diff_obj;
}

void linear_function_mixer::mix(const msgpack::object& obj, diff_object ptr) const {
  diffv diff;
  internal_diff_object* diff_obj = dynamic_cast<internal_diff_object*>(ptr.get());
  if (!diff_obj) {
    throw JUBATUS_EXCEPTION(core::common::exception::runtime_error("bad diff_object"));
  }
  obj.convert(&diff);
  mix(diff, diff_obj->diff_);
}

void linear_function_mixer::get_diff(msgpack_writer& writer) const {
  diffv diff;
  get_diff(diff);
  msgpack::pack(writer, diff);
}

void linear_function_mixer::put_diff(const diff_object& ptr) {
  internal_diff_object* diff_obj = dynamic_cast<internal_diff_object*>(ptr.get());
  if (!diff_obj) {
    throw JUBATUS_EXCEPTION(core::common::exception::runtime_error("bad diff_object"));
  }
  put_diff(diff_obj->diff_);
}

}  // namespace framework
}  // namespace core
}  // namespace jubatus
