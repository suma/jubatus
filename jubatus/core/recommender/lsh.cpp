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

#include "lsh.hpp"

#include <cmath>
#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include "../common/exception.hpp"
#include "../common/hash.hpp"
#include "lsh_util.hpp"

using std::pair;
using std::string;
using std::vector;
using jubatus::core::storage::bit_vector;
using jubatus::core::storage::mixable_bit_index_storage;

namespace jubatus {
namespace core {
namespace recommender {

static const uint64_t DEFAULT_BASE_NUM = 64;  // should be in config

lsh::config::config()
    : bit_num(DEFAULT_BASE_NUM) {
}

lsh::lsh(uint64_t base_num)
    : mixable_storage_(mixable_bit_index_storage::model_ptr(new storage::bit_index_storage))
    , base_num_(base_num) {
  if (base_num == 0) {
    throw JUBATUS_EXCEPTION(common::exception::runtime_error("base_num == 0"));
  }
}

lsh::lsh(const config& config)
    : mixable_storage_(mixable_bit_index_storage::model_ptr(new storage::bit_index_storage))
    , base_num_(config.bit_num) {
}

lsh::lsh()
    : mixable_storage_(mixable_bit_index_storage::model_ptr(new storage::bit_index_storage))
    , base_num_(DEFAULT_BASE_NUM) {
}

lsh::~lsh() {
}

void lsh::similar_row(
    const common::sfv_t& query,
    vector<pair<string, float> >& ids,
    size_t ret_num) const {
  ids.clear();
  if (ret_num == 0) {
    return;
  }

  bit_vector query_bv;
  calc_lsh_values(query, query_bv);
  mixable_storage_.get_model()->similar_row(query_bv, ids, ret_num);
}

void lsh::neighbor_row(
    const common::sfv_t& query,
    vector<pair<string, float> >& ids,
    size_t ret_num) const {
  similar_row(query, ids, ret_num);
  for (size_t i = 0; i < ids.size(); ++i) {
    ids[i].second = 1 - ids[i].second;
  }
}

void lsh::clear() {
  orig_.clear();
  pfi::data::unordered_map<std::string, std::vector<float> >()
    .swap(column2baseval_);
  mixable_storage_.get_model()->clear();
}

void lsh::clear_row(const string& id) {
  orig_.remove_row(id);
  mixable_storage_.get_model()->remove_row(id);
}

void lsh::calc_lsh_values(const common::sfv_t& sfv, bit_vector& bv) const {
  const_cast<lsh*>(this)->generate_column_bases(sfv);

  vector<float> lsh_vals;
  prod_invert_and_vector(column2baseval_, sfv, base_num_, lsh_vals);
  set_bit_vector(lsh_vals, bv);
}

void lsh::generate_column_bases(const common::sfv_t& sfv) {
  for (size_t i = 0; i < sfv.size(); ++i) {
    generate_column_base(sfv[i].first);
  }
}

void lsh::generate_column_base(const string& column) {
  if (column2baseval_.count(column) != 0) {
    return;
  }
  const uint32_t seed = common::hash_util::calc_string_hash(column);
  generate_random_vector(base_num_, seed, column2baseval_[column]);
}

void lsh::update_row(const string& id, const sfv_diff_t& diff) {
  generate_column_bases(diff);
  orig_.set_row(id, diff);
  common::sfv_t row;
  orig_.get_row(id, row);
  bit_vector bv;
  calc_lsh_values(row, bv);
  mixable_storage_.get_model()->set_row(id, bv);
}

void lsh::get_all_row_ids(std::vector<std::string>& ids) const {
  mixable_storage_.get_model()->get_all_row_ids(ids);
}

string lsh::type() const {
  return string("lsh");
}

void lsh::save_impl(framework::msgpack_writer& writer) const {
  msgpack::packer<framework::msgpack_writer> pk(writer);

  pk.pack_array(2);
  pk.pack(column2baseval_);
  pk.pack(*mixable_storage_.get_model().get());
}

void lsh::load_impl(msgpack::object& o) {
  //o.convert(this);
  size_t size = o.via.array.size;
  if (o.type != msgpack::type::ARRAY || size != 2) {
    throw msgpack::type_error();
  }

  o.via.array.ptr[0].convert(&column2baseval_);
  o.via.array.ptr[1].convert(mixable_storage_.get_model().get());
}

framework::linear_mixable* lsh::get_linear_mixable() {
  return &mixable_storage_;
}

const framework::linear_mixable* lsh::get_const_linear_mixable() const {
  return &mixable_storage_;
}

}  // namespace recommender
}  // namespace core
}  // namespace jubatus
