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

#ifndef JUBATUS_CORE_STORAGE_INVERTED_INDEX_STORAGE_HPP_
#define JUBATUS_CORE_STORAGE_INVERTED_INDEX_STORAGE_HPP_

#include <string>
#include <utility>
#include <vector>
#include <msgpack.hpp>
#include <pficommon/data/unordered_map.h>
#include "storage_type.hpp"
#include "../common/type.hpp"
#include "../common/key_manager.hpp"
#include "sparse_matrix_storage.hpp"
#include "../framework/model.hpp"
#include "../framework/mixable.hpp"

namespace jubatus {
namespace core {
namespace storage {

class inverted_index_storage : public framework::model {
 public:
  struct diff_type {
    sparse_matrix_storage inv;
    map_float_t column2norm;

    MSGPACK_DEFINE(inv, column2norm);
  };

  inverted_index_storage();
  ~inverted_index_storage();

  void set(const std::string& row, const std::string& column, float val);
  float get(const std::string& row, const std::string& column) const;
  void remove(const std::string& row, const std::string& column);
  void clear();
  void get_all_column_ids(std::vector<std::string>& ids) const;

  void calc_scores(
      const common::sfv_t& sfv,
      std::vector<std::pair<std::string, float> >& scores,
      size_t ret_num) const;

  void get_diff(diff_type& diff) const;
  void put_diff(const diff_type& mixed_diff);
  void mix(const diff_type& lhs, diff_type& mixed) const;

  std::string name() const;

  void save(framework::msgpack_writer&) const;
  void load(msgpack::object&);

  MSGPACK_DEFINE(inv_, inv_diff_, column2norm_,
      column2id_, column2norm_diff_, column2id_);
 private:
  static float calc_l2norm(const common::sfv_t& sfv);
  float calc_columnl2norm(uint64_t column_id) const;
  float get_from_tbl(
      const std::string& row,
      uint64_t column_id,
      const tbl_t& tbl,
      bool& exist) const;

  void add_inp_scores(
      const std::string& row,
      float val,
      pfi::data::unordered_map<uint64_t, float>& scores) const;

  tbl_t inv_;
  tbl_t inv_diff_;
  imap_float_t column2norm_;
  imap_float_t column2norm_diff_;
  common::key_manager column2id_;
};

typedef framework::linear_mixable_delegation<
  inverted_index_storage, inverted_index_storage::diff_type>
  mixable_inverted_index_storage;

}  // namespace storage
}  // namespace core
}  // namespace jubatus

#endif  // JUBATUS_CORE_STORAGE_INVERTED_INDEX_STORAGE_HPP_
