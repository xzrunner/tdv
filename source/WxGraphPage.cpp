#include "tdv/WxGraphPage.h"
#include "tdv/Evaluator.h"
#include "tdv/Blackboard.h"

#include <ee0/SubjectMgr.h>
#include <blueprint/MessageID.h>
#include <blueprint/Blueprint.h>
#include <blueprint/CompNode.h>
#include <blueprint/Node.h>

#include <node0/SceneNode.h>
#include <node0/CompComplex.h>
#include <node2/CompBoundingBox.h>

namespace
{

const uint32_t MESSAGES[] =
{
    ee0::MSG_SCENE_NODE_INSERT,
    ee0::MSG_SCENE_NODE_DELETE,
    ee0::MSG_SCENE_NODE_CLEAR,

    bp::MSG_BP_CONN_INSERT,
    bp::MSG_BP_BEFORE_CONN_DELETE,
    bp::MSG_BP_CONN_REBUILD,
    bp::MSG_BP_NODE_PROP_CHANGED,
};

}

namespace tdv
{

WxGraphPage::WxGraphPage(wxWindow* parent, const ee0::GameObj& root,
                         const ee0::SubjectMgrPtr& preview_sub_mgr)
    : ee0::WxStagePage(parent)
    , m_preview_sub_mgr(preview_sub_mgr)
    , m_root(root)
{
    bp::Blueprint::Instance();

    m_eval = std::make_shared<Evaluator>(preview_sub_mgr);
    Blackboard::Instance()->SetEval(m_eval);
    Blackboard::Instance()->SetSubMgr(m_sub_mgr);

    for (auto& msg : MESSAGES) {
        m_sub_mgr->RegisterObserver(msg, this);
    }
}

WxGraphPage::~WxGraphPage()
{
    for (auto& msg : MESSAGES) {
        m_sub_mgr->UnregisterObserver(msg, this);
    }
}

void WxGraphPage::OnNotify(uint32_t msg, const ee0::VariantSet& variants)
{
    ee0::WxStagePage::OnNotify(msg, variants);

	bool dirty = false;
	switch (msg)
	{
	case ee0::MSG_SCENE_NODE_INSERT:
		dirty = InsertSceneObj(variants);
		break;
	case ee0::MSG_SCENE_NODE_DELETE:
		dirty = DeleteSceneObj(variants);
		break;
	case ee0::MSG_SCENE_NODE_CLEAR:
        dirty = ClearAllSceneObjs();
		break;

    case bp::MSG_BP_CONN_INSERT:
        dirty = AfterInsertNodeConn(variants);
        break;
    case bp::MSG_BP_BEFORE_CONN_DELETE:
        dirty = BeforeDeleteNodeConn(variants);
        break;
    case bp::MSG_BP_CONN_REBUILD:
        m_eval->OnRebuildConnection();
        dirty = true;
        break;
    case bp::MSG_BP_NODE_PROP_CHANGED:
        dirty = UpdateNodeProp(variants);
        break;
	}

	if (dirty) {
        m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
	}
}

void WxGraphPage::Traverse(std::function<bool(const ee0::GameObj&)> func,
                           const ee0::VariantSet& variants , bool inverse) const
{
    m_root->GetSharedComp<n0::CompComplex>().Traverse(func, inverse);
}

bool WxGraphPage::ClearAllSceneObjs()
{
    // update scene node
    assert(m_root->HasSharedComp<n0::CompComplex>());
    auto& ccomplex = m_root->GetSharedComp<n0::CompComplex>();
    bool dirty = !ccomplex.GetAllChildren().empty();
    ccomplex.RemoveAllChildren();

    // front eval cb
    assert(m_eval);
    m_eval->OnClearAllNodes();

    return true;
}

bool WxGraphPage::InsertSceneObj(const ee0::VariantSet& variants)
{
    auto var = variants.GetVariant("obj");
    GD_ASSERT(var.m_type == ee0::VT_PVOID, "no var in vars: obj");
    const ee0::GameObj* obj = static_cast<const ee0::GameObj*>(var.m_val.pv);
    GD_ASSERT(obj, "err scene obj");

    // update scene node
    assert(m_root->HasSharedComp<n0::CompComplex>());
    auto& ccomplex = m_root->GetSharedComp<n0::CompComplex>();
    InsertScenNode(ccomplex, *obj);

    return true;
}

bool WxGraphPage::DeleteSceneObj(const ee0::VariantSet& variants)
{
	auto var = variants.GetVariant("obj");
	GD_ASSERT(var.m_type == ee0::VT_PVOID, "no var in vars: obj");
    const ee0::GameObj* obj = static_cast<const ee0::GameObj*>(var.m_val.pv);
	GD_ASSERT(obj, "err scene obj");

    // update scene (*obj)
    assert(m_root->HasSharedComp<n0::CompComplex>());
    auto& ccomplex = m_root->GetSharedComp<n0::CompComplex>();
    bool dirty = ccomplex.RemoveChild(*obj);

    if (dirty && (*obj)->HasUniqueComp<bp::CompNode>()) {
        auto& bp_node = (*obj)->GetUniqueComp<bp::CompNode>().GetNode();
        m_eval->OnRemoveNode(*bp_node);
    }

    return dirty;
}

bool WxGraphPage::AfterInsertNodeConn(const ee0::VariantSet& variants)
{
    auto var = variants.GetVariant("conn");
    GD_ASSERT(var.m_type == ee0::VT_PVOID, "no var in vars: conn");
    const std::shared_ptr<bp::Connecting>* conn = static_cast<const std::shared_ptr<bp::Connecting>*>(var.m_val.pv);
    GD_ASSERT(conn, "err conn");

    m_eval->OnConnected(**conn);

    return true;
}

bool WxGraphPage::BeforeDeleteNodeConn(const ee0::VariantSet& variants)
{
    auto var = variants.GetVariant("conn");
    GD_ASSERT(var.m_type == ee0::VT_PVOID, "no var in vars: conn");
    const std::shared_ptr<bp::Connecting>* conn = static_cast<const std::shared_ptr<bp::Connecting>*>(var.m_val.pv);
    GD_ASSERT(conn, "err conn");

    m_eval->OnDisconnecting(**conn);

    return true;
}

bool WxGraphPage::UpdateNodeProp(const ee0::VariantSet& variants)
{
    auto var = variants.GetVariant("obj");
    if (var.m_type == ee0::VT_EMPTY) {
        return false;
    }

    GD_ASSERT(var.m_type == ee0::VT_PVOID, "no var in vars: obj");
    const ee0::GameObj* obj = static_cast<const ee0::GameObj*>(var.m_val.pv);
    GD_ASSERT(obj, "err scene obj");

    if ((*obj)->HasUniqueComp<bp::CompNode>())
    {
        auto& bp_node = (*obj)->GetUniqueComp<bp::CompNode>().GetNode();
        m_eval->OnNodePropChanged(bp_node);
        UpdateAABB(*obj);

        return true;
    }
    else
    {
        return false;
    }
}

void WxGraphPage::UpdateAABB(const ee0::GameObj& obj)
{
    auto& bp_node = obj->GetUniqueComp<bp::CompNode>().GetNode();

    auto& st = bp_node->GetStyle();
    obj->GetUniqueComp<n2::CompBoundingBox>().SetSize(
        *obj, sm::rect(st.width, st.height)
    );
}

void WxGraphPage::InsertScenNode(n0::CompComplex& root,
                                 const n0::SceneNodePtr& node)
{
    root.AddChild(node);

    if (!node->HasUniqueComp<bp::CompNode>()) {
        return;
    }

    auto& bp_node = node->GetUniqueComp<bp::CompNode>().GetNode();
    m_eval->OnAddNode(*bp_node, node);
    UpdateAABB(node);
}

}