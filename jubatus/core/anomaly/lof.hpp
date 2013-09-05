// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2012 Preferred Infrastracture and Nippon Telegraph and Telephone Corporation.
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

#ifndef JUBATUS_CORE_ANOMALY_LOF_HPP_
#define JUBATUS_CORE_ANOMALY_LOF_HPP_

#include <string>
#include <vector>
#include <pficommon/text/json.h>
#include "anomaly_base.hpp"
#include "lof_storage.hpp"

namespace jubatus {
namespace core {
namespace anomaly {

class lof : public anomaly_base {
 public:
  lof();
  explicit lof(
      const lof_storage::config& config,
      core::recommender::recommender_base* nn_engine);
  ~lof();

  // return anomaly score of query
  float calc_anomaly_score(const common::sfv_t& query) const;
  float calc_anomaly_score(const std::string& id) const;

  void clear();
  void clear_row(const std::string& id);
  void update_row(const std::string& id, const sfv_diff_t& diff);
  void get_all_row_ids(std::vector<std::string>& ids) const;

  std::string type() const;

  framework::linear_mixable* get_linear_mixable() = 0;
  const framework::linear_mixable* get_const_linear_mixable() const = 0;

  void save(framework::msgpack_writer&) const;
  void load(msgpack::object&);

  // static float calc_distance(common::sfv_t& q1, common::sfv_t& q2);
  // static float calc_l2norm(common::sfv_t& q1, common::sfv_t& q2);
  //lof_storage lof_index_;
  mixable_lof_storage mixable_storage_;
};

}  //  namespace anomaly
}  //  namespace core
}  //  namespace jubatus

#endif  // JUBATUS_CORE_ANOMALY_LOF_HPP_
