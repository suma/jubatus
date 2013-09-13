// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2012 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
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

#ifndef JUBATUS_CORE_RECOMMENDER_RECOMMENDER_MOCK_HPP_
#define JUBATUS_CORE_RECOMMENDER_RECOMMENDER_MOCK_HPP_

#include <string>
#include <utility>
#include <vector>
#include "recommender_base.hpp"
#include "recommender_mock_storage.hpp"

namespace jubatus {
namespace core {
namespace recommender {

class recommender_mock : public recommender_base {
 public:
  using recommender_base::similar_row;
  using recommender_base::neighbor_row;

  recommender_mock();
  virtual ~recommender_mock();

  void set_similar_relation(
      const common::sfv_t& query,
      const std::vector<std::pair<std::string, float> >& ids);
  void set_similar_relation(
      const std::string& id,
      const std::vector<std::pair<std::string, float> >& ids);
  void set_neighbor_relation(
      const common::sfv_t& query,
      const std::vector<std::pair<std::string, float> >& ids);
  void set_neighbor_relation(
      const std::string& id,
      const std::vector<std::pair<std::string, float> >& ids);

  virtual void similar_row(
      const common::sfv_t& query,
      std::vector<std::pair<std::string, float> >& ids,
      size_t ret_num) const;
  virtual void neighbor_row(
      const common::sfv_t& query,
      std::vector<std::pair<std::string, float> >& ids,
      size_t ret_num) const;
  virtual void clear();
  virtual void clear_row(const std::string& id);
  virtual void update_row(const std::string& id, const sfv_diff_t& diff);
  virtual void get_all_row_ids(std::vector<std::string>& ids) const;

  virtual std::string type() const;

  framework::linear_mixable* get_linear_mixable();
  const framework::linear_mixable* get_const_linear_mixable() const;

  MSGPACK_DEFINE(orig_);
 private:
  virtual void save_impl(framework::msgpack_writer&) const;
  virtual void load_impl(msgpack::object&);

  mixable_recommender_mock_storage storage_;
};

}  // namespace recommender
}  // namespace core
}  // namespace jubatus

#endif  // JUBATUS_CORE_RECOMMENDER_RECOMMENDER_MOCK_HPP_
