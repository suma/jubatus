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

#ifndef JUBATUS_DRIVER_DRIVER_ANOMALY_HPP_
#define JUBATUS_DRIVER_DRIVER_ANOMALY_HPP_

#include <string>
#include <utility>
#include <vector>
#include <pficommon/lang/shared_ptr.h>
#include "jubatus/core/anomaly/anomaly_base.hpp"
#include "jubatus/core/framework/mixable.hpp"
#include "jubatus/core/framework/mixable_weight_manager.hpp"
#include "jubatus/core/fv_converter/datum_to_fv_converter.hpp"
#include "mixable_holder.hpp"

namespace jubatus {
namespace driver {

class anomaly {
 public:
  anomaly(
      jubatus::core::anomaly::anomaly_base* anomaly_method,
      pfi::lang::shared_ptr<core::fv_converter::datum_to_fv_converter> converter);
  virtual ~anomaly();

  pfi::lang::shared_ptr<mixable_holder> get_mixable_holder() const {
    return mixable_holder_;
  }

  jubatus::core::anomaly::anomaly_base* get_model() const {
    return anomaly_.get();
  }

  void clear_row(const std::string& id);
  std::pair<std::string, float> add(
      const std::string& id,
      const core::fv_converter::datum& d);
  float update(const std::string& id, const core::fv_converter::datum& d);
  void clear();
  float calc_score(const core::fv_converter::datum& d) const;
  std::vector<std::string> get_all_rows() const;

  void save(core::framework::msgpack_writer& writer) const;
  void load(msgpack::object& o);

 private:
  pfi::lang::shared_ptr<mixable_holder> mixable_holder_;

  pfi::lang::shared_ptr<core::fv_converter::datum_to_fv_converter> converter_;
  pfi::lang::shared_ptr<core::anomaly::anomaly_base> anomaly_;
  core::framework::mixable_weight_manager wm_;
};

}  // namespace driver
}  // namespace jubatus

#endif  // JUBATUS_DRIVER_DRIVER_ANOMALY_HPP_
