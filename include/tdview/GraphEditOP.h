#pragma once

#include <ee0/typedef.h>

#include <blueprint/ConnectPinOP.h>

namespace tdv
{

class GraphEditOP : public bp::ConnectPinOP
{
public:
    GraphEditOP(const std::shared_ptr<pt0::Camera>& cam, ee0::WxStagePage& stage,
        const std::vector<bp::NodePtr>& nodes);

    virtual bool OnMouseLeftDown(int x, int y) override;
    virtual bool OnMouseLeftUp(int x, int y) override;
    virtual bool OnMouseDrag(int x, int y) override;
    virtual bool OnMouseWheelRotation(int x, int y, int direction) override;

private:
    ee0::EditOPPtr QuerySelectedOP(const sm::vec2& pos) const;

private:
    ee0::WxStagePage& m_stage;

    ee0::EditOPPtr m_selected_op = nullptr;

}; // GraphEditOP

}