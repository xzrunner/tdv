#pragma once

#include "tdv/PreviewBuilder.h"

#include <ee0/typedef.h>
#include <blueprint/typedef.h>

#include <td/Evaluator.h>
#include <td/typedef.h>
#include <node0/typedef.h>

#include <boost/noncopyable.hpp>

#include <vector>
#include <unordered_map>

namespace bp { class Connecting; }

namespace tdv
{

class Evaluator : boost::noncopyable
{
public:
    Evaluator(const ee0::SubjectMgrPtr& sub_mgr);

    // update nodes
    void OnAddNode(const bp::Node& node, const n0::SceneNodePtr& snode,
        bool need_update = true);
    void OnRemoveNode(const bp::Node& node);
    void OnClearAllNodes();

    // update node prop
    void OnNodePropChanged(const bp::NodePtr& node);

    // update node conn
    void OnConnected(const bp::Connecting& conn);
    void OnDisconnecting(const bp::Connecting& conn);
    void OnRebuildConnection();

    auto& GetEval() const { return m_eval; }

    td::OpPtr QueryBackNode(const bp::Node& front_node) const;
    n0::SceneNodePtr QuerySceneNode(const bp::Node& front_node) const;

private:
    void Update();

private:
    td::Evaluator m_eval;

    PreviewBuilder m_preview_builder;

    std::unordered_map<const bp::Node*, td::OpPtr> m_front2back;
    std::unordered_map<const td::Operator*, n0::SceneNodePtr> m_back2node;

}; // Evaluator

}