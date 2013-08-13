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

#ifndef JUBATUS_CORE_FRAMEWORK_MIXABLE_WEIGHT_MANAGER_HPP_
#define JUBATUS_CORE_FRAMEWORK_MIXABLE_WEIGHT_MANAGER_HPP_

#include "model.hpp"
#include "mixable.hpp"
#include "linear_mixable.hpp"
#include "../fv_converter/weight_manager.hpp"

namespace jubatus {
namespace core {
namespace framework {

class mixable_weight_manager : public core::framework::mixable<
    fv_converter::weight_manager,
    fv_converter::keyword_weights> {
 public:
  fv_converter::keyword_weights get_diff_impl() const;

  void put_diff_impl(const fv_converter::keyword_weights& diff);

  void mix_impl(
      const fv_converter::keyword_weights& lhs,
      const fv_converter::keyword_weights& rhs,
      fv_converter::keyword_weights& acc) const;
  void clear();
};

class linear_mixable_weight_manager : public model, public linear_mixable {
 public:
  linear_mixable_weight_manager(pfi::lang::shared_ptr<fv_converter::weight_manager> wm);

  void save(msgpack_writer&);
  void load(msgpack::object&);

  diff_object convert_diff_object(const msgpack::object&) const;
  void mix(const msgpack::object& obj, diff_object) const;
  void get_diff(msgpack_writer&);
  void put_diff(const msgpack::object& obj);

 private:
  pfi::lang::shared_ptr<fv_converter::weight_manager> weight_manager_;
};

}  // namespace framework
}  // namespace core
}  // namespace jubatus

#endif  // JUBATUS_CORE_FRAMEWORK_MIXABLE_WEIGHT_MANAGER_HPP_
