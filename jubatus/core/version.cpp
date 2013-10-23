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

#include "version.hpp"

using std::string;

namespace jubatus {
namespace core {

int version::major() {
  return JUBATUS_VERSION_MAJOR;
}

int version::minor() {
  return JUBATUS_VERSION_MINOR;
}

int version::patch() {
  return JUBATUS_VERSION_PATCH;
}

std::string version::string() {
  return JUBATUS_VERSION "(" JUBATUS_REPOSITORY_VERSION ")";
}

string version::built_branch() {
  return JUBATUS_REPOSITORY_BRANCH;
}

string version::plugin_directory() {
  return JUBATUS_PLUGIN_DIR;
}

}  // namespace core
}  // namespace jubatus
