#include "tdview/NodePreview.h"
#include "tdview/Node.h"
#include "tdview/Evaluator.h"
#include "tdview/RegistNodes.h"
#include "tdview/PinType.h"
#include "tdview/Blackboard.h"

#include <ee3/WorldTravelOP.h>
#include <blueprint/NodeHelper.h>

#include <SM_Matrix2D.h>
#include <unirender/Blackboard.h>
#include <unirender/RenderContext.h>
#include <painting0/RenderContext.h>
#include <painting2/Blackboard.h>
#include <painting2/RenderContext.h>
#include <painting2/RenderTarget.h>
#include <painting2/RenderSystem.h>
#include <painting2/RenderTargetCtx.h>
#include <painting2/WindowContext.h>
#include <painting2/WindowCtxRegion.h>
#include <painting2/Shader.h>
#include <painting3/RenderSystem.h>
#include <painting3/MaterialMgr.h>
#include <painting3/PerspCam.h>
#include <node2/RenderSystem.h>
#include <node3/RenderSystem.h>
#include <renderpipeline/RenderMgr.h>
#include <renderpipeline/IRenderer.h>
#include <td/Operator.h>
#include <td/ParamImpl.h>
#include <tessellation/Painter.h>

namespace tdv
{

NodePreview::NodePreview()
{
    m_cam = std::make_shared<pt3::PerspCam>(
        sm::vec3(0, 2, -2), sm::vec3(0, 0, 0), sm::vec3(0, 1, 0)
    );

    m_op = std::make_shared<ee3::WorldTravelOP>(
        m_cam, m_vp, Blackboard::Instance()->GetSubMgr()
    );
}

void NodePreview::Draw(const Evaluator& eval, const bp::Node& node, const n2::RenderParams& rp) const
{
    auto& st = node.GetStyle();
    m_cam->OnSize(st.width, st.width);
    m_vp.SetSize(st.width, st.width);

    auto& rt_mgr = pt2::Blackboard::Instance()->GetRenderContext().GetRTMgr();
    auto rt = rt_mgr.Fetch();

    rt->Bind();
    bool succ = DrawToRT(eval, node);
    rt->Unbind();

    if (succ) {
        DrawFromRT(node, rp, *rt);
    }

    rt_mgr.Return(rt);
}

bool NodePreview::DrawToRT(const Evaluator& eval, const bp::Node& node) const
{
    auto& rt_mgr = pt2::Blackboard::Instance()->GetRenderContext().GetRTMgr();

    auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
    auto renderer = rp::RenderMgr::Instance()->GetRenderer(rp::RenderType::SPRITE);
    auto shader = std::static_pointer_cast<pt2::Shader>(renderer->GetAllShaders()[0]);

    auto& st = node.GetStyle();
    pt2::RenderTargetCtx ctx(ur_rc, shader, st.width, st.width);

    ur_rc.SetClearColor(0xff000000);
    ur_rc.Clear();

    auto type = node.get_type();
    auto scene_node = eval.QuerySceneNode(node);
    if (!scene_node) {
        return false;
    }

    pt0::RenderContext rc;
    rc.AddVar(
        pt3::MaterialMgr::PositionUniforms::light_pos.name,
        pt0::RenderVariant(sm::vec3(0, 2, -4))
    );
    if (m_cam->TypeID() == pt0::GetCamTypeID<pt3::PerspCam>())
    {
        auto persp = std::static_pointer_cast<pt3::PerspCam>(m_cam);
        rc.AddVar(
            pt3::MaterialMgr::PositionUniforms::cam_pos.name,
            pt0::RenderVariant(persp->GetPos())
        );
    }
    rc.AddVar(
        pt3::MaterialMgr::PosTransUniforms::view.name,
        pt0::RenderVariant(m_cam->GetViewMat())
    );
    rc.AddVar(
        pt3::MaterialMgr::PosTransUniforms::projection.name,
        pt0::RenderVariant(m_cam->GetProjectionMat())
    );

    tess::Painter pt;

    auto cam_mat = m_cam->GetProjectionMat() * m_cam->GetViewMat();

    pt3::RenderParams rp;
    rp.painter  = &pt;
    rp.viewport = &m_vp;
    rp.cam_mat  = &cam_mat;

    n3::RenderSystem::Draw(*scene_node, rp, rc);

    pt2::RenderSystem::DrawPainter(pt);

    return true;
}

void NodePreview::DrawFromRT(const bp::Node& node, const n2::RenderParams& rp, const pt2::RenderTarget& rt) const
{
    auto mt4 = sm::mat4(bp::NodeHelper::CalcPreviewMat(node, rp.GetMatrix()));
    const auto scale = mt4.GetScale();
    const float hw = scale.x * 0.5f;
    const float hh = scale.y * 0.5f;
    const auto pos = mt4.GetTranslate();
    const float vertices[] = {
        pos.x - hw, pos.y - hh,
        pos.x + hw, pos.y - hh,
        pos.x + hw, pos.y + hh,
        pos.x - hw, pos.y + hh,
    };

    auto& st = node.GetStyle();
    auto& rt_mgr = pt2::Blackboard::Instance()->GetRenderContext().GetRTMgr();
    auto tw = st.width / rt_mgr.WIDTH;
    auto th = st.width / rt_mgr.HEIGHT;
    const float texcoords[] = {
        0, 0,
        tw, 0,
        tw, th,
        0, th
    };
    pt2::RenderSystem::DrawTexQuad(vertices, texcoords, rt.GetTexID(), 0xffffffff);
}

}