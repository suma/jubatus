// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2013 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
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

#ifndef JUBATUS_CORE_FRAMEWORK_LINEAR_MIXABLE_HPP_
#define JUBATUS_CORE_FRAMEWORK_LINEAR_MIXABLE_HPP_

#include <msgpack.hpp>
#include <pficommon/lang/shared_ptr.h>
#include "../common/exception.hpp"
#include "../common/byte_buffer.hpp"

namespace jubatus {
namespace core {
namespace framework {

class msgpack_writer;

class diff_object_raw {
 public:
  virtual ~diff_object_raw();
  virtual void convert_binary(msgpack_writer&) const = 0;
};

typedef pfi::lang::shared_ptr<diff_object_raw> diff_object;

class linear_mixable {
 public:
  virtual ~linear_mixable();

  virtual diff_object convert_diff_object(const msgpack::object&) const = 0;
  // mix assumes fold left?
  virtual void mix(const msgpack::object& obj, diff_object) const = 0;
  virtual void get_diff(msgpack_writer&) const = 0;
  virtual void put_diff(const diff_object& obj) = 0;
};


}  // namespace framework
}  // namespace core
}  // namespace jubatus

#endif  // JUBATUS_CORE_FRAMEWORK_LINEAR_MIXABLE_HPP_
