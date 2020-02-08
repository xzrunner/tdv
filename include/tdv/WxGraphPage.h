#pragma once

#include <ee0/WxStagePage.h>

namespace n0 { class CompComplex; }

namespace tdv
{

class Evaluator;

class WxGraphPage : public ee0::WxStagePage
{
public:
	WxGraphPage(wxWindow* parent, const ee0::GameObj& root,
        const ee0::SubjectMgrPtr& preview_sub_mgr);
    virtual ~WxGraphPage();

    virtual void OnNotify(uint32_t msg, const ee0::VariantSet& variants) override;

    virtual void Traverse(std::function<bool(const ee0::GameObj&)> func,
        const ee0::VariantSet& variants = ee0::VariantSet(), bool inverse = false) const override;

    auto& GetEval() const { return m_eval; }

private:
    bool ClearAllSceneObjs();
    bool InsertSceneObj(const ee0::VariantSet& variants);
    bool DeleteSceneObj(const ee0::VariantSet& variants);

    bool AfterInsertNodeConn(const ee0::VariantSet& variants);
    bool BeforeDeleteNodeConn(const ee0::VariantSet& variants);
    bool UpdateNodeProp(const ee0::VariantSet& variants);

    void UpdateAABB(const ee0::GameObj& obj);

    void InsertScenNode(n0::CompComplex& root,
        const n0::SceneNodePtr& node);

private:
    ee0::SubjectMgrPtr m_preview_sub_mgr = nullptr;

    n0::SceneNodePtr m_root = nullptr;

    std::shared_ptr<Evaluator> m_eval = nullptr;

}; // WxGraphPage

}