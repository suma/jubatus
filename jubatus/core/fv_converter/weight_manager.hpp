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

#ifndef JUBATUS_CORE_FV_CONVERTER_WEIGHT_MANAGER_HPP_
#define JUBATUS_CORE_FV_CONVERTER_WEIGHT_MANAGER_HPP_

#include <istream>
#include <ostream>
#include <string>
#include <msgpack.hpp>
#include <pficommon/data/unordered_map.h>
#include "../common/type.hpp"
#include "../framework/model.hpp"
#include "counter.hpp"
#include "datum.hpp"
#include "keyword_weights.hpp"

namespace jubatus {
namespace core {
namespace fv_converter {

class weight_manager : framework::model {
 public:
  weight_manager();

  void update_weight(const common::sfv_t& fv);
  void get_weight(common::sfv_t& fv) const;

  void add_weight(const std::string& key, float weight);

  void get_diff(keyword_weights& diff) const {
    diff = diff_weights_;
  }

  void put_diff(const keyword_weights& diff) {
    master_weights_.merge(diff);
    diff_weights_.clear();
  }

  void mix(const keyword_weights& lhs,
      keyword_weights& acc) const {
    acc.merge(lhs);
  }

  void clear() {
    diff_weights_.clear();
    master_weights_.clear();
  }

  void save(std::ostream& os) {
    pfi::data::serialization::binary_oarchive oa(os);
    oa << diff_weights_;
    oa << master_weights_;
  }
  void load(std::istream& is) {
    pfi::data::serialization::binary_iarchive ia(is);
    ia >> diff_weights_;
    ia >> master_weights_;
  }

  void save(framework::msgpack_writer& writer) const {
    msgpack::pack(writer, *this);
  }

  void load(msgpack::object& o) {
    o.convert(this);
  }

  template<class Archiver>
  void serialize(Archiver& ar) {
    ar & MEMBER(diff_weights_) & MEMBER(master_weights_);
  }

  MSGPACK_DEFINE(diff_weights_, master_weights_);
 private:
  size_t get_document_count() const {
    return diff_weights_.get_document_count() +
        master_weights_.get_document_count();
  }

  size_t get_document_frequency(const std::string& key) const {
    return diff_weights_.get_document_frequency(key) +
        master_weights_.get_document_frequency(key);
  }

  double get_user_weight(const std::string& key) const {
    return diff_weights_.get_user_weight(key) +
        master_weights_.get_user_weight(key);
  }

  double get_global_weight(const std::string& key) const;

  keyword_weights diff_weights_;
  keyword_weights master_weights_;
};

}  // namespace fv_converter
}  // namespace core
}  // namespace jubatus

#endif  // JUBATUS_CORE_FV_CONVERTER_WEIGHT_MANAGER_HPP_
