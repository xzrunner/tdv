#include "tdv/GraphEditOP.h"
#include "tdv/Node.h"

#include <ee0/CameraHelper.h>
#include <ee0/WxStagePage.h>
#include <blueprint/CompNode.h>

#include <node0/SceneNode.h>
#include <SM_Calc.h>

namespace tdv
{

GraphEditOP::GraphEditOP(const std::shared_ptr<pt0::Camera>& cam, ee0::WxStagePage& stage,
                         const std::vector<bp::NodePtr>& nodes)
	: bp::ConnectPinOP(cam, stage, nodes)
    , m_stage(stage)
{
}

bool GraphEditOP::OnMouseLeftDown(int x, int y)
{
    auto pos = ee0::CameraHelper::TransPosScreenToProject(*m_camera, x, y);
    m_selected_op = QuerySelectedOP(pos);
    if (m_selected_op) {
        return m_selected_op->OnMouseLeftDown(x, y);
    }

    return bp::ConnectPinOP::OnMouseLeftDown(x, y);
}

bool GraphEditOP::OnMouseLeftUp(int x, int y)
{
    if (m_selected_op) {
        bool ret = m_selected_op->OnMouseLeftUp(x, y);
        m_selected_op.reset();
        return ret;
    }

    return bp::ConnectPinOP::OnMouseLeftUp(x, y);
}

bool GraphEditOP::OnMouseDrag(int x, int y)
{
    if (m_selected_op) {
        return m_selected_op->OnMouseDrag(x, y);
    }

    return bp::ConnectPinOP::OnMouseDrag(x, y);
}

bool GraphEditOP::OnMouseWheelRotation(int x, int y, int direction)
{
    auto pos = ee0::CameraHelper::TransPosScreenToProject(*m_camera, x, y);
    auto op = QuerySelectedOP(pos);
    if (op) {
        return op->OnMouseWheelRotation(x, y, direction);
    }

    return bp::ConnectPinOP::OnMouseWheelRotation(x, y, direction);
}

ee0::EditOPPtr GraphEditOP::QuerySelectedOP(const sm::vec2& pos) const
{
    ee0::EditOPPtr selected = nullptr;

 	m_stage.Traverse([&](const ee0::GameObj& obj)->bool
	{
		if (!obj->HasUniqueComp<bp::CompNode>()) {
			return true;
		}
		auto& cnode = obj->GetUniqueComp<bp::CompNode>();
		auto bp_node = cnode.GetNode();
		if (!bp_node || !bp_node->get_type().is_derived_from<Node>()) {
			return true;
		}

        auto& st = bp_node->GetStyle();

        sm::rect r;
        r.xmin = -st.width * 0.5f;  r.xmax = r.xmin + st.width;
        r.ymax = -st.height * 0.5f; r.ymin = r.ymax - st.width;
        r.Translate(bp_node->GetPos());

        if (sm::is_point_in_rect(pos, r)) {
            auto op = std::static_pointer_cast<Node>(bp_node)->GetPreview().GetEditOP();
            if (op) {
                selected = op;
            }
            return false;
        } else {
            return true;
        }

		return true;
	});

    return selected;
}

}