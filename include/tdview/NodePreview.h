#pragma once

#include <ee0/typedef.h>

#include <painting3/Viewport.h>

namespace n2 { class RenderParams; }
namespace pt0 { class Camera; }
namespace pt2 { class RenderTarget; }
namespace bp { class Node; }

#include <memory>

namespace tdv
{

class Evaluator;

class NodePreview
{
public:
    NodePreview();

    void Draw(const Evaluator& eval, const bp::Node& node, const n2::RenderParams& rp) const;

    auto GetEditOP() const { return m_op; }

private:
    bool DrawToRT(const Evaluator& eval, const bp::Node& node) const;
    void DrawFromRT(const bp::Node& node, const n2::RenderParams& rp, const pt2::RenderTarget& rt) const;

private:
    std::shared_ptr<pt0::Camera> m_cam = nullptr;
    mutable pt3::Viewport m_vp;

    ee0::EditOPPtr m_op = nullptr;

}; // NodePreview

}