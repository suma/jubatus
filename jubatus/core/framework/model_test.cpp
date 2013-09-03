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

#include "model.hpp"
#include <gtest/gtest.h>

namespace jubatus {
namespace core {
namespace framework {

struct int_model {
  int_model()
      : value(0) {
  }

  int value;
  MSGPACK_DEFINE(value);

  void save(msgpack_writer& buf) const {
    msgpack::pack(buf, *this);
  }

  void load(msgpack::object& o) {
    o.convert(this);
  }
};

TEST(mixable, save_load) {
  int_model m;
  m.value = 30;
  ASSERT_EQ(30, m.value);

  // save model (value = 30)
  msgpack::sbuffer sbuf;
  stream_writer<msgpack::sbuffer> swriter(sbuf);
  m.save(swriter);
  ASSERT_LT(0, sbuf.size());

  // change model value
  m.value = 100;

  msgpack::unpacked msg;
  msgpack::unpack(&msg, sbuf.data(), sbuf.size());

  // load model (value = 30)
  int_model load;
  load.load(msg.get());
  ASSERT_EQ(30, load.value);
}

}  // namespace framework
}  // namespace core
}  // namespace jubatus

