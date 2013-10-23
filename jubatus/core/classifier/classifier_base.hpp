// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2011 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
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

#ifndef JUBATUS_CORE_CLASSIFIER_CLASSIFIER_BASE_HPP_
#define JUBATUS_CORE_CLASSIFIER_CLASSIFIER_BASE_HPP_

#include <stdint.h>

#include <cmath>
#include <map>
#include <string>
#include <vector>

#include <pficommon/lang/shared_ptr.h>
#include "../common/type.hpp"
#include "../storage/storage_base.hpp"
#include "classifier_type.hpp"

namespace jubatus {
namespace core {
namespace classifier {

typedef pfi::lang::shared_ptr<core::storage::storage_base>
  classifier_storage_ptr;

class multiclass_classifier_base {
 public:
  virtual ~multiclass_classifier_base();
  virtual void train(const common::sfv_t& fv, const std::string& label) = 0;
  virtual std::string classify(const common::sfv_t& fv) const = 0;
  virtual void classify_with_scores(const common::sfv_t& fv,
      classify_result& scores) const = 0;
  virtual std::string name() const = 0;
};

// abstract class
class multiclass_classifier : public multiclass_classifier_base {
 public:
  explicit multiclass_classifier(const classifier_storage_ptr&);
  virtual ~multiclass_classifier();
  virtual void train(const common::sfv_t& fv, const std::string& label) = 0;

  std::string classify(const common::sfv_t& fv) const;
  void classify_with_scores(const common::sfv_t& fv,
                            classify_result& scores) const;

  void clear();

  classifier_storage_ptr storage();

  virtual std::string name() const = 0;

 protected:
  void update_weight(
      const common::sfv_t& sfv,
      float step_weigth,
      const std::string& pos_label,
      const std::string& neg_class);
  float calc_margin(
      const common::sfv_t& sfv,
      const std::string& label,
      std::string& incorrect_label) const;
  float calc_margin_and_variance(
      const common::sfv_t& sfv,
      const std::string& label,
      std::string& incorrect_label,
      float& variance) const;
  std::string get_largest_incorrect_label(
      const common::sfv_t& sfv,
      const std::string& label,
      classify_result& scores) const;

  static float squared_norm(const common::sfv_t& sfv);

  classifier_storage_ptr storage_;
  bool use_covars_;
};

}  // namespace classifier
}  // namespace core
}  // namespace jubatus

#endif  // JUBATUS_CORE_CLASSIFIER_CLASSIFIER_BASE_HPP_
