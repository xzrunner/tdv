#pragma once

#include <ee0/typedef.h>

#include <node0/typedef.h>

namespace td { class Evaluator; class Operator; }

#include <unordered_map>

namespace tdv
{

class PreviewBuilder
{
public:
    PreviewBuilder(const ee0::SubjectMgrPtr& sub_mgr, const td::Evaluator& eval,
        const std::unordered_map<const td::Operator*, n0::SceneNodePtr>& back2node);

    void Build();

private:
    ee0::SubjectMgrPtr m_sub_mgr;

    const td::Evaluator& m_eval;
    const std::unordered_map<const td::Operator*, n0::SceneNodePtr>& m_back2node;

}; // PreviewBuilder

}