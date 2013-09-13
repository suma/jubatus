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

#ifndef JUBATUS_CORE_RECOMMENDER_RECOMMENDER_MOCK_STORAGE_HPP_
#define JUBATUS_CORE_RECOMMENDER_RECOMMENDER_MOCK_STORAGE_HPP_

#include <map>
#include <string>
#include <utility>
#include <vector>
#include <msgpack.hpp>
#include <pficommon/data/serialization.h>
#include "recommender_type.hpp"
#include "../framework/model.hpp"
#include "../framework/mixable.hpp"

namespace jubatus {
namespace core {
namespace recommender {

class recommender_mock_storage
    : public framework::model {
 public:
  typedef std::map<common::sfv_t, std::vector<std::pair<std::string, float> > >
    relation_type;
  struct diff_type {
    relation_type similar_relation;
    relation_type neighbor_relation;
    MSGPACK_DEFINE(similar_relation, neighbor_relation);
  };

  virtual ~recommender_mock_storage();

  void set_similar_items(
      const common::sfv_t& query,
      const std::vector<std::pair<std::string, float> >& ids);
  void set_neighbor_items(
      const common::sfv_t& query,
      const std::vector<std::pair<std::string, float> >& ids);

  void similar_items_similarity(
      const common::sfv_t& query,
      std::vector<std::pair<std::string, float> >& ids,
      size_t ret_num) const;
  void neighbor_items_distance(
      const common::sfv_t& query,
      std::vector<std::pair<std::string, float> >& ids,
      size_t ret_num) const;

  void update(const common::sfv_t& from, const common::sfv_t& to);
  void remove(const common::sfv_t& query);
  void clear();

  std::string name() const;

  void get_diff(diff_type& diff) const;
  void put_diff(const diff_type& mixed_diff);
  void mix(const diff_type& lhs, diff_type& rhs) const;

  void save(framework::msgpack_writer&) const;
  void load(msgpack::object&);

  MSGPACK_DEFINE(similar_relation_, neighbor_relation_);
 private:
  static void get_relation(
      const common::sfv_t& query,
      const relation_type& relmap,
      size_t ret_num,
      std::vector<std::pair<std::string, float> >& ids);

  static void update_relation_key(
      const common::sfv_t& from,
      const common::sfv_t& to,
      relation_type& relmap);

  static void mix_relation(const relation_type& from, relation_type& to);

  relation_type similar_relation_;
  relation_type neighbor_relation_;
};

typedef framework::linear_mixable_delegation<recommender_mock_storage,
    recommender_mock_storage::diff_type> mixable_recommender_mock_storage;

}  // namespace recommender
}  // namespace core
}  // namespace jubatus

#endif  // JUBATUS_CORE_RECOMMENDER_RECOMMENDER_MOCK_STORAGE_HPP_
