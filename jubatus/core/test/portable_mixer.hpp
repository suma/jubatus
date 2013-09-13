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

#ifndef JUBATUS_CORE_TEST_PORTABLE_MIXER_HPP_
#define JUBATUS_CORE_TEST_PORTABLE_MIXER_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <pficommon/math/random.h>
#include "../common/hash.hpp"
#include "../framework/model.hpp"
#include "../framework/linear_mixable.hpp"

namespace jubatus {
namespace core {
namespace test {

class portable_mixer {
 public:
  portable_mixer() {
  }

  explicit portable_mixer(uint32_t seed)
      : rand_(seed) {
  }

  void clear() {
    mixable_.clear();
  }

  void add(framework::linear_mixable* storage) {
    mixable_.push_back(storage);
  }

  framework::linear_mixable* get_random() {
    return mixable_[rand_(mixable_.size())];
  }

  size_t get_hash(const std::string& id) const {
    return common::hash_util::calc_string_hash(id) % mixable_.size();
  }

  void mix() {
    if (mixable_.empty()) {
      return;
    }

    msgpack::sbuffer sbuf;
    framework::stream_writer<msgpack::sbuffer> writer(sbuf);
    mixable_[0]->get_diff(writer);
    msgpack::unpacked msg;
    msgpack::unpack(&msg, sbuf.data(), sbuf.size());

    framework::diff_object mixed = mixable_[0]->convert_diff_object(msg.get());

    for (size_t i = 1; i < mixable_.size(); ++i) {
      msgpack::sbuffer diff_buf;
      framework::stream_writer<msgpack::sbuffer> diff_writer(diff_buf);
      mixable_[i]->get_diff(diff_writer);
      msgpack::unpacked diff;
      msgpack::unpack(&diff, diff_buf.data(), diff_buf.size());
      mixable_[0]->mix(diff.get(), mixed);
    }

    for (size_t i = 0; i < mixable_.size(); ++i) {
      mixable_[i]->put_diff(mixed);
    }
  }

 private:
  std::vector<framework::linear_mixable*> mixable_;
  pfi::math::random::mtrand rand_;
};

}  // namespace test
}  // namespace core
}  // namespace jubatus

#endif  // JUBATUS_CORE_TEST_PORTABLE_MIXER_HPP_
