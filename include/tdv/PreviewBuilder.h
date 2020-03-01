#pragma once

#include <ee0/typedef.h>

#include <dag/Graph.h>
#include <td/ParamType.h>
#include <node0/typedef.h>

#include <unordered_map>

namespace td { class Operator; }

namespace tdv
{

class PreviewBuilder
{
public:
    PreviewBuilder(const ee0::SubjectMgrPtr& sub_mgr, const dag::Graph<td::ParamType>& eval,
        const std::unordered_map<const dag::Node<td::ParamType>*, n0::SceneNodePtr>& back2node);

    void Build();

private:
    ee0::SubjectMgrPtr m_sub_mgr;

    const dag::Graph<td::ParamType>& m_eval;
    const std::unordered_map<const dag::Node<td::ParamType>*, n0::SceneNodePtr>& m_back2node;

}; // PreviewBuilder

}