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

#include "storage_factory.hpp"

#include <string>

#include "jubatus/core/common/exception.hpp"
#include "jubatus/core/storage/storage_base.hpp"
#include "jubatus/core/storage/local_storage.hpp"
#include "jubatus/core/storage/local_storage_mixture.hpp"

using jubatus::core::storage::storage_base;
using jubatus::core::storage::local_storage;
using jubatus::core::storage::local_storage_mixture;

namespace jubatus {
namespace driver {

storage_base* storage_factory::create_storage(const std::string& name) {
  if (name == "local") {
    return static_cast<storage_base*>(new local_storage);
  } else if (name == "local_mixture") {
    return static_cast<storage_base*>(new local_storage_mixture);
  }

  // maybe bug or configuration mistake
  throw JUBATUS_EXCEPTION(
      jubatus::core::common::exception::runtime_error(
          std::string("failed to create storage: ") + name));
}

}  // namespace driver
}  // namespace jubatus
