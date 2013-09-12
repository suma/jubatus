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

#ifndef JUBATUS_CORE_FRAMEWORK_MIXABLE_HPP_
#define JUBATUS_CORE_FRAMEWORK_MIXABLE_HPP_

#include <iostream>
#include <string>
#include <vector>

#include <msgpack.hpp>
#include <pficommon/lang/shared_ptr.h>
#include "../common/exception.hpp"
#include "../common/byte_buffer.hpp"
#include "model.hpp"
#include "linear_mixable.hpp"

namespace jubatus {
namespace core {
namespace framework {

class mixable0 {
 public:
  virtual ~mixable0() {
  }
  virtual common::byte_buffer get_diff() const {}
  virtual void put_diff(const common::byte_buffer&) {}
  virtual void mix(const common::byte_buffer&,
                   const common::byte_buffer&,
                   common::byte_buffer&) const {}

  virtual void save(std::ostream& ofs) {}
  virtual void load(std::istream& ifs) {}
  virtual void clear() {}
};

#if 0
class mixable0 {
 public:
  mixable0() {
  }

  virtual ~mixable0() {
  }

  virtual common::byte_buffer get_diff() const = 0;
  virtual void put_diff(const common::byte_buffer&) = 0;
  virtual void mix(const common::byte_buffer&,
                   const common::byte_buffer&,
                   common::byte_buffer&) const = 0;

  virtual void save(std::ostream& ofs) = 0;
  virtual void load(std::istream& ifs) = 0;
  virtual void clear() = 0;
};
#endif

// linear_mixable CRTP
template <typename Model, typename Diff>
class linear_mixable_base : public linear_mixable {
 public:
  typedef Model model_type;
  typedef Diff diff_type;
  typedef pfi::lang::shared_ptr<Model> model_ptr;

  diff_object convert_diff_object(const msgpack::object& obj) const {
    internal_diff_object* diff = new internal_diff_object;
    diff_object diff_obj(diff);
    obj.convert(&diff->diff_);
    return diff_obj;
  }

  void mix(const msgpack::object& obj, diff_object ptr) const {
    Diff diff;
    internal_diff_object* diff_obj = dynamic_cast<internal_diff_object*>(ptr.get());
    if (!diff_obj) {
      throw JUBATUS_EXCEPTION(core::common::exception::runtime_error("bad diff_object"));
    }
    obj.convert(&diff);
    static_cast<const Model&>(*this).mix(diff, diff_obj->diff_);
  }

  void get_diff(msgpack_writer& writer) const {
    Diff diff;
    static_cast<const Model&>(*this).get_diff(diff);
    msgpack::pack(writer, diff);
  }

  void put_diff(const diff_object& ptr) {
    internal_diff_object* diff_obj = dynamic_cast<internal_diff_object*>(ptr.get());
    if (!diff_obj) {
      throw JUBATUS_EXCEPTION(core::common::exception::runtime_error("bad diff_object"));
    }
    static_cast<Model&>(*this).put_diff(diff_obj->diff_);
  }

 private:
  struct internal_diff_object : diff_object_raw {
    void convert_binary(msgpack_writer& writer) const {
      msgpack::pack(writer, diff_);
    }

    Diff diff_;
  };
};

// mixable_delegating doesnt inherit the `model`
//  MUST NOT multiple inheritance `model`
template <typename Model, typename Diff>
class linear_mixable_delegation : public linear_mixable {
 public:
  typedef Model model_type;
  typedef Diff diff_type;
  typedef pfi::lang::shared_ptr<Model> model_ptr;

  linear_mixable_delegation(model_ptr model) 
    : model_(model) {
    if (!model) {
      throw JUBATUS_EXCEPTION(common::config_not_set());
    }
  }

  virtual ~linear_mixable_delegation() {
  }

  model_ptr get_model() const {
    return model_;
  }

  diff_object convert_diff_object(const msgpack::object& obj) const {
    internal_diff_object* diff = new internal_diff_object;
    diff_object diff_obj(diff);
    obj.convert(&diff->diff_);
    return diff_obj;
  }

  void mix(const msgpack::object& obj, diff_object ptr) const {
    Diff diff;
    internal_diff_object* diff_obj = dynamic_cast<internal_diff_object*>(ptr.get());
    if (!diff_obj) {
      throw JUBATUS_EXCEPTION(core::common::exception::runtime_error("bad diff_object"));
    }
    obj.convert(&diff);
    model_->mix(diff, diff_obj->diff_);
  }

  void get_diff(msgpack_writer& writer) const {
    Diff diff;
    model_->get_diff(diff);
    msgpack::pack(writer, diff);
  }

  void put_diff(const diff_object& ptr) {
    internal_diff_object* diff_obj = dynamic_cast<internal_diff_object*>(ptr.get());
    if (!diff_obj) {
      throw JUBATUS_EXCEPTION(core::common::exception::runtime_error("bad diff_object"));
    }
    model_->put_diff(diff_obj->diff_);
  }

#if 0
  void put_diff(const msgpack::object& obj) {
    Diff diff;
    obj.convert(&diff);
    model_->put_diff(diff);
  }
#endif

 private:
  struct internal_diff_object : diff_object_raw {
    void convert_binary(msgpack_writer& writer) const {
      msgpack::pack(writer, diff_);
    }

    Diff diff_;
  };

  model_ptr model_;
};

#if 1
template<typename Model, typename Diff>
class mixable : public mixable0 {
 public:
  typedef Model model_type;
  typedef Diff diff_type;
  typedef pfi::lang::shared_ptr<Model> model_ptr;

  virtual ~mixable() {
  }

  virtual void clear() = 0;

  virtual Diff get_diff_impl() const = 0;
  virtual void put_diff_impl(const Diff&) = 0;
  virtual void mix_impl(const Diff&, const Diff&, Diff&) const = 0;

  void set_model(model_ptr m) {
    model_ = m;
  }

  common::byte_buffer get_diff() const {
    if (model_) {
      common::byte_buffer buf;
      pack_(get_diff_impl(), buf);
      return buf;
    } else {
      throw JUBATUS_EXCEPTION(common::config_not_set());
    }
  }

  void put_diff(const common::byte_buffer& d) {
    if (model_) {
      Diff diff;
      unpack_(d, diff);
      put_diff_impl(diff);
    } else {
      throw JUBATUS_EXCEPTION(common::config_not_set());
    }
  }

  void mix(
      const common::byte_buffer& lhs,
      const common::byte_buffer& rhs,
      common::byte_buffer& mixed_buf) const {
    Diff left, right, mixed;
    unpack_(lhs, left);
    unpack_(rhs, right);
    mix_impl(left, right, mixed);
    pack_(mixed, mixed_buf);
  }

  void save(std::ostream& os) {
    model_->save(os);
  }

  void load(std::istream& is) {
    model_->load(is);
  }

  model_ptr get_model() const {
    return model_;
  }

 private:
  void unpack_(const common::byte_buffer& buf, Diff& d) const {
    msgpack::unpacked msg;
    msgpack::unpack(&msg, buf.ptr(), buf.size());
    msg.get().convert(&d);
  }

  void pack_(const Diff& d, common::byte_buffer& buf) const {
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, d);
    buf.assign(sbuf.data(), sbuf.size());
  }

  model_ptr model_;
};

#endif

}  // namespace framework
}  // namespace core
}  // namespace jubatus

#endif  // JUBATUS_CORE_FRAMEWORK_MIXABLE_HPP_
