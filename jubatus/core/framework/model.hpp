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

#ifndef JUBATUS_CORE_FRAMEWORK_MODEL_HPP_
#define JUBATUS_CORE_FRAMEWORK_MODEL_HPP_

#include <msgpack.hpp>

namespace jubatus {
namespace core {
namespace framework {

class msgpack_writer {
 public:
  virtual ~msgpack_writer();
  virtual void write(const char* buf, unsigned int len) = 0;
};

template <class T>
class stream_writer : public msgpack_writer {
 public:
  explicit stream_writer(T& stream)
    : stream_(stream) {
  }

  void write(const char*buf, unsigned int len) {
    stream_.write(buf, len);
  }

  T& stream() {
    return stream_;
  }
 private:
  T& stream_;
};

class model {
 public:
  virtual ~model();

  // by msgpack
  virtual void save(msgpack_writer&) const = 0;
  virtual void load(msgpack::object&) = 0;
  // You can calculate only serialized size by msgpack_writer.
  // TODO: virtual void check(msgpack::unpacker&) const = 0;

  // these can be empty, do nothing by default
  virtual void compact();
  virtual void clear();
  //virtual memory_allocator* allocator();
};

/*
 * class memory_allocator {
 *  public:
 *   void reserve();
 *   void do_compact();
 *   size_t heap_used() const;
 *   size_t heap_free_num();
 *    // and so on..
 */


}  // namespace framework
}  // namespace core
}  // namespace jubatus

#endif  // JUBATUS_CORE_FRAMEWORK_MODEL_HPP_
