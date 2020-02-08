#pragma once

#include <ee0/typedef.h>

#include <cu/cu_macro.h>

#include <memory>

namespace tdv
{

class Evaluator;

class Blackboard
{
public:
    void SetEval(const std::shared_ptr<Evaluator>& eval) {
        m_eval = eval;
    }
    auto GetEval() const { return m_eval; }

    void SetSubMgr(const ee0::SubjectMgrPtr& sub_mgr) {
        m_sub_mgr = sub_mgr;
    }
    auto GetSubMgr() const { return m_sub_mgr; }

private:
    std::shared_ptr<Evaluator> m_eval = nullptr;

    ee0::SubjectMgrPtr m_sub_mgr = nullptr;

    CU_SINGLETON_DECLARATION(Blackboard);

}; // Blackboard

}