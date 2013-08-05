// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2011,2012,2013 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
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

#ifndef JUBATUS_DRIVER_MIXABLE_HOLDER_HPP_
#define JUBATUS_DRIVER_MIXABLE_HOLDER_HPP_

#include <vector>
#include <pficommon/concurrent/rwmutex.h>

#include "jubatus/core/framework/mixable.hpp"

namespace jubatus {
namespace driver {

class mixable_holder {
 public:
  typedef std::vector<core::framework::mixable0*> mixable_list;

  mixable_holder() {
  }

  virtual ~mixable_holder() {
  }

  void register_mixable(core::framework::mixable0* m) {
    mixables_.push_back(m);
  }

  mixable_list get_mixables() const {
    return mixables_;
  }

  pfi::concurrent::rw_mutex& rw_mutex() {
    return rw_mutex_;
  }

 protected:
  pfi::concurrent::rw_mutex rw_mutex_;
  std::vector<core::framework::mixable0*> mixables_;
};

}  // namespace driver
}  // namespace jubatus

#endif
