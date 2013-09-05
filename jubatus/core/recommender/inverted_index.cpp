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

#include "inverted_index.hpp"

#include <algorithm>
#include <cmath>
#include <string>
#include <utility>
#include <vector>
#include "../common/exception.hpp"
#include "../common/vector_util.hpp"

using std::pair;
using std::string;
using std::vector;
using jubatus::core::storage::inverted_index_storage;
using jubatus::core::storage::mixable_inverted_index_storage;

namespace jubatus {
namespace core {
namespace recommender {

inverted_index::inverted_index()
  : mixable_storage_(mixable_inverted_index_storage::model_ptr(
        new inverted_index_storage)) {
}

inverted_index::~inverted_index() {
}

void inverted_index::similar_row(
    const common::sfv_t& query,
    std::vector<std::pair<std::string, float> >& ids,
    size_t ret_num) const {
  ids.clear();
  if (ret_num == 0) {
    return;
  }
  mixable_storage_.get_model()->calc_scores(query, ids, ret_num);
}

void inverted_index::neighbor_row(
    const common::sfv_t& query,
    vector<pair<string, float> >& ids,
    size_t ret_num) const {
  similar_row(query, ids, ret_num);
  for (size_t i = 0; i < ids.size(); ++i) {
    ids[i].second = 1 - ids[i].second;
  }
}

void inverted_index::clear() {
  orig_.clear();
  mixable_storage_.get_model()->clear();
}

void inverted_index::clear_row(const std::string& id) {
  vector<pair<string, float> > columns;
  orig_.get_row(id, columns);
  for (size_t i = 0; i < columns.size(); ++i) {
    mixable_storage_.get_model()->remove(columns[i].first, id);
  }
  orig_.remove_row(id);
}

void inverted_index::update_row(const std::string& id, const sfv_diff_t& diff) {
  orig_.set_row(id, diff);
  for (size_t i = 0; i < diff.size(); ++i) {
    mixable_storage_.get_model()->set(diff[i].first, id, diff[i].second);
  }
}

void inverted_index::get_all_row_ids(std::vector<std::string>& ids) const {
  mixable_storage_.get_model()->get_all_column_ids(ids);  // inv_.column = row
}

string inverted_index::type() const {
  return string("inverted_index");
}

void inverted_index::save_impl(framework::msgpack_writer& writer) const {
  msgpack::pack(writer, *mixable_storage_.get_model().get());
}

void inverted_index::load_impl(msgpack::object& o) {
  o.convert(mixable_storage_.get_model().get());
}

framework::linear_mixable* inverted_index::get_linear_mixable() {
  return &mixable_storage_;
}

const framework::linear_mixable* inverted_index::get_const_linear_mixable()
  const {
  return &mixable_storage_;
}

}  // namespace recommender
}  // namespace core
}  // namespace jubatus
