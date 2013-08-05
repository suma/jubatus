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

#ifndef JUBATUS_DRIVER_DRIVER_REGRESSION_HPP_
#define JUBATUS_DRIVER_DRIVER_REGRESSION_HPP_

#include <string>
#include <utility>

#include <pficommon/lang/shared_ptr.h>
#include "jubatus/core/regression/regression_base.hpp"
#include "jubatus/core/framework/mixable.hpp"
#include "jubatus/core/fv_converter/datum_to_fv_converter.hpp"
#include "jubatus/core/framework/linear_function_mixer.hpp"
#include "jubatus/core/framework/mixable_weight_manager.hpp"
#include "mixable_holder.hpp"

namespace jubatus {
namespace driver {

class regression {
 public:
  regression(
      core::storage::storage_base* model_storage,
      jubatus::core::regression::regression_base* regression_method,
      pfi::lang::shared_ptr<core::fv_converter::datum_to_fv_converter> converter);
  virtual ~regression();

  pfi::lang::shared_ptr<mixable_holder> get_mixable_holder() const {
    return mixable_holder_;
  }

  core::storage::storage_base* get_model() const {
    return mixable_regression_model_.get_model().get();
  }

  void train(const std::pair<float, core::fv_converter::datum>& data);
  float estimate(const core::fv_converter::datum& data) const;

 private:
  pfi::lang::shared_ptr<mixable_holder> mixable_holder_;

  pfi::lang::shared_ptr<core::fv_converter::datum_to_fv_converter> converter_;
  pfi::lang::shared_ptr<jubatus::core::regression::regression_base> regression_;
  core::framework::linear_function_mixer mixable_regression_model_;
  core::framework::mixable_weight_manager wm_;
};

}  // namespace driver
}  // namespace jubatus

#endif  // JUBATUS_DRIVER_DRIVER_REGRESSION_HPP_
