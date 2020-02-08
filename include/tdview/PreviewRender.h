#pragma once

#include <tessellation/Painter.h>
#include <painting0/RenderContext.h>

namespace n0 { class SceneNode; }
namespace pt3 { class Viewport; }

namespace tdv
{

class PreviewRender
{
public:
    PreviewRender(const pt3::Viewport& vp, const sm::mat4& cam_mat);

    void DrawNode(const pt0::RenderContext& rc, const n0::SceneNode& node) const;

    auto& GetPainter() const { return m_pt; }

private:
    const pt3::Viewport& m_vp;
    const sm::mat4&      m_cam_mat;

    mutable tess::Painter m_pt;

}; // PreviewRender

}