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

#include <stdexcept>

#include <gtest/gtest.h>
#include <pficommon/lang/scoped_ptr.h>
#include <pficommon/text/json.h>

#include "regression_factory.hpp"
#include "jubatus/core/regression/regression.hpp"
#include "jubatus/core/storage/local_storage.hpp"
#include "jubatus/core/common/exception.hpp"
#include "jubatus/core/common/jsonconfig.hpp"

using jubatus::core::storage::local_storage;
using jubatus::core::regression::regression_base;
using jubatus::core::regression::passive_aggressive;

namespace jubatus {
namespace driver {

TEST(regression_factory, trivial) {
  local_storage s;
  core::common::jsonconfig::config param(pfi::text::json::to_json(
      passive_aggressive::config()));
  pfi::lang::scoped_ptr<regression_base>
    r(regression_factory::create_regression("PA", param, &s));
  EXPECT_EQ(typeid(*r), typeid(passive_aggressive&));
}

TEST(regression_factory, unknown) {
  local_storage s;
  core::common::jsonconfig::config param;
  ASSERT_THROW(regression_factory::create_regression("unknown_regression", param, &s),
               core::common::unsupported_method);
}

}  // namespace driver
}  // namespace jubatus
