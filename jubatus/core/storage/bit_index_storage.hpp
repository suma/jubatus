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

#ifndef JUBATUS_CORE_STORAGE_BIT_INDEX_STORAGE_HPP_
#define JUBATUS_CORE_STORAGE_BIT_INDEX_STORAGE_HPP_

#include <string>
#include <utility>
#include <vector>
#include <msgpack.hpp>
#include <pficommon/data/unordered_map.h>
#include "../common/key_manager.hpp"
#include "storage_type.hpp"
#include "sparse_matrix_storage.hpp"
#include "bit_vector.hpp"
#include "../framework/model.hpp"
#include "../framework/mixable.hpp"

namespace jubatus {
namespace core {
namespace storage {

class bit_index_storage : public framework::model {
 public:
  bit_index_storage();
  ~bit_index_storage();

  void set_row(const std::string& row, const bit_vector& bv);
  void get_row(const std::string& row, bit_vector& bv) const;
  void remove_row(const std::string& row);
  void clear();
  void get_all_row_ids(std::vector<std::string>& ids) const;

  void similar_row(
      const bit_vector& bv,
      std::vector<std::pair<std::string, float> >& ids,
      uint64_t ret_num) const;
  std::string name() const;

  void save(framework::msgpack_writer&) const;
  void load(msgpack::object&);

  void get_diff(bit_table_t& diff) const;
  void put_diff(const bit_table_t& mixed_diff);
  void mix(const bit_table_t& lhs, bit_table_t& rhs) const;

  MSGPACK_DEFINE(bitvals_, bitvals_diff_);

 private:

  bit_table_t bitvals_;
  bit_table_t bitvals_diff_;
};

typedef framework::linear_mixable_delegation<bit_index_storage, bit_table_t>
  mixable_bit_index_storage;

}  // namespace storage
}  // namespace core
}  // namespace jubatus

#endif  // JUBATUS_CORE_STORAGE_BIT_INDEX_STORAGE_HPP_
