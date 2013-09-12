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

#ifndef JUBATUS_DRIVER_DRIVER_GRAPH_HPP_
#define JUBATUS_DRIVER_DRIVER_GRAPH_HPP_

#include <string>
#include <utility>
#include <vector>
#include <pficommon/lang/shared_ptr.h>
#include "jubatus/core/graph/graph_base.hpp"
#include "jubatus/core/graph/graph_type.hpp"
#include "jubatus/core/graph/graph_wo_index.hpp"
#include "jubatus/core/framework/mixable.hpp"
#include "mixable_holder.hpp"

namespace jubatus {
namespace driver {

class graph {
 public:
  explicit graph(jubatus::core::graph::graph_wo_index* graph_method);
  virtual ~graph();

  pfi::lang::shared_ptr<mixable_holder> get_mixable_holder() const {
    return mixable_holder_;
  }

  jubatus::core::graph::graph_base* get_model() const {
    return graph_.get_model().get();
  }

  void create_node(jubatus::core::graph::node_id_t id);
  void update_node(
      jubatus::core::graph::node_id_t id,
      const jubatus::core::graph::property& p);
  void remove_node(jubatus::core::graph::node_id_t id);
  void create_edge(
      jubatus::core::graph::edge_id_t eid,
      jubatus::core::graph::node_id_t src,
      jubatus::core::graph::node_id_t target,
      const jubatus::core::graph::property& p);
  void update_edge(
      jubatus::core::graph::edge_id_t eid,
      const jubatus::core::graph::property& p);
  void remove_edge(jubatus::core::graph::edge_id_t eid);
  double get_centrality(
      jubatus::core::graph::node_id_t nid,
      jubatus::core::graph::centrality_type ct,
      const jubatus::core::graph::preset_query& q) const;
  std::vector<jubatus::core::graph::node_id_t> get_shortest_path(
      jubatus::core::graph::node_id_t src,
      jubatus::core::graph::node_id_t target,
      uint64_t max_hop,
      const jubatus::core::graph::preset_query &q) const;
  void add_centrality_query(const jubatus::core::graph::preset_query& q);
  void add_shortest_path_query(const jubatus::core::graph::preset_query& q);
  void remove_centrality_query(const jubatus::core::graph::preset_query& q);
  void remove_shortest_path_query(const jubatus::core::graph::preset_query& q);
  jubatus::core::graph::node_info get_node(
      jubatus::core::graph::node_id_t nid) const;
  jubatus::core::graph::edge_info get_edge(
      jubatus::core::graph::edge_id_t eid) const;
  void update_index();
  void clear();

  // internal apis used between servers
  void create_node_here(jubatus::core::graph::node_id_t id);
  void remove_global_node(jubatus::core::graph::node_id_t nid);
  void create_edge_here(
      jubatus::core::graph::edge_id_t eid,
      jubatus::core::graph::node_id_t src,
      jubatus::core::graph::node_id_t target,
      const jubatus::core::graph::property& p);

  void save(core::framework::msgpack_writer&) const;
  void load(msgpack::object&);

 private:
  pfi::lang::shared_ptr<mixable_holder> mixable_holder_;

  core::graph::mixable_graph_wo_index graph_;
};

}  // namespace driver
}  // namespace jubatus

#endif  // JUBATUS_DRIVER_DRIVER_GRAPH_HPP_
