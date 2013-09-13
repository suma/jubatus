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

#ifndef JUBATUS_CORE_STAT_MIXABLE_STAT_HPP_
#define JUBATUS_CORE_STAT_MIXABLE_STAT_HPP_

#include <map>
#include <string>
#include <utility>

#include "stat.hpp"
#include "../framework/mixable.hpp"

namespace jubatus {
namespace core {
namespace stat {

class mixable_stat : public stat_base,
  public framework::linear_mixable_base<
                     mixable_stat, std::pair<double, size_t> > {
 public:
  explicit mixable_stat(size_t);
  virtual ~mixable_stat();

  void push(const std::string& key, double val) {
    stat_.push(key, val);
  }

  double sum(const std::string& key) const {
    return stat_.sum(key);
  }

  double stddev(const std::string& key) const {
    return stat_.stddev(key);
  }

  double max(const std::string& key) const {
    return stat_.max(key);
  }

  double min(const std::string& key) const {
    return stat_.min(key);
  }

  double moment(const std::string& key, int n, double c) const {
    return stat_.moment(key, n, c);
  }

  // entropy = - \sum_{for all keys in this stat} p(key) log p(key)

  // - mixed entropy -
  // p(key) log p(key) = { n(key) log n(key) - n (key) log N } / N
  // E (key) = ( \sum n(key) log n(key) )/N - log N / N \sum n(key)
  //        = ( \sum n(key) log n(key) )/N - log N
  // get_diff : () ->  pair( \sum n(key) log n(key), partial N )
  // put_diff : pair( \sum n(key) log n(key), partial N ) -> ()
  // mix : pair(e1, n1) -> pair(e2, n2) -> pair( e1+e2, n1+n2 )
  // entropy : e -> n -> e/n - log n

  void get_diff(std::pair<double, size_t>& diff) const;
  void put_diff(const std::pair<double, size_t>&);
  void mix(const std::pair<double, size_t>&, std::pair<double, size_t>&) const;

  double entropy() const;

  void clear();

  void save(framework::msgpack_writer&) const;
  void load(msgpack::object&);

  std::string type() const;

  MSGPACK_DEFINE(e_, n_, stat_);
 private:
  stat stat_;
  double e_;
  double n_;
};
}  // namespace stat
}  // namespace core
}  // namespace jubatus

#endif  // JUBATUS_CORE_STAT_MIXABLE_STAT_HPP_
