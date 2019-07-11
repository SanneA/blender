#pragma once

#include <functional>

#include "BKE_node_tree.hpp"
#include "FN_data_flow_nodes.hpp"

#include "world_state.hpp"
#include "step_description.hpp"
#include "forces.hpp"

namespace BParticles {

using BKE::bSocketList;
using BKE::IndexedNodeTree;
using BKE::SocketWithNode;
using FN::DataFlowNodes::BTreeDataGraph;

class ProcessNodeInterface {
 private:
  bNode *m_bnode;
  IndexedNodeTree &m_indexed_tree;
  BTreeDataGraph &m_data_graph;
  WorldState &m_world_state;
  ModifierStepDescription &m_step_description;

 public:
  ProcessNodeInterface(bNode *bnode,
                       IndexedNodeTree &indexed_tree,
                       BTreeDataGraph &data_graph,
                       WorldState &world_state,
                       ModifierStepDescription &step_description)
      : m_bnode(bnode),
        m_indexed_tree(indexed_tree),
        m_data_graph(data_graph),
        m_world_state(world_state),
        m_step_description(step_description)
  {
  }

  bNode *bnode()
  {
    return m_bnode;
  }

  IndexedNodeTree &indexed_tree()
  {
    return m_indexed_tree;
  }

  FN::DataFlowNodes::BTreeDataGraph &data_graph()
  {
    return m_data_graph;
  }

  WorldState &world_state()
  {
    return m_world_state;
  }

  ModifierStepDescription &step_description()
  {
    return m_step_description;
  }

  ArrayRef<SocketWithNode> linked_with_input(uint index)
  {
    bNodeSocket *socket = this->inputs().get(index);
    return m_indexed_tree.linked(socket);
  }

  ArrayRef<SocketWithNode> linked_with_output(uint index)
  {
    bNodeSocket *socket = this->outputs().get(index);
    return m_indexed_tree.linked(socket);
  }

  bSocketList inputs()
  {
    return bSocketList(m_bnode->inputs);
  }

  bSocketList outputs()
  {
    return bSocketList(m_bnode->outputs);
  }

  PointerRNA node_rna()
  {
    return m_indexed_tree.get_rna(m_bnode);
  }
};

using ProcessNodeFunction = std::function<void(ProcessNodeInterface &interface)>;
using ProcessFunctionsMap = SmallMap<std::string, ProcessNodeFunction>;

ProcessFunctionsMap &get_node_processors();

struct BuildContext {
  IndexedNodeTree &indexed_tree;
  BTreeDataGraph &data_graph;
  ModifierStepDescription &step_description;
};

using ForceFromNodeCallback =
    std::function<std::unique_ptr<Force>(BuildContext &ctx, bNode *bnode)>;
using ForceFromNodeCallbackMap = SmallMap<std::string, ForceFromNodeCallback>;

ForceFromNodeCallbackMap &get_force_builders();

using EventFromNodeCallback =
    std::function<std::unique_ptr<Event>(BuildContext &ctx, bNode *bnode)>;
using EventFromNodeCallbackMap = SmallMap<std::string, EventFromNodeCallback>;

EventFromNodeCallbackMap &get_event_builders();

}  // namespace BParticles
