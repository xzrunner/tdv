#pragma once

#include <ee0/Observer.h>
#include <ee0/typedef.h>

#include <wx/panel.h>

namespace ur { class Device; }
namespace bp { class WxNodeProperty; }

namespace tdv
{

class WxToolbarPanel : public wxPanel, public ee0::Observer
{
public:
    WxToolbarPanel(const ur::Device& dev, wxWindow* parent,
        const ee0::SubjectMgrPtr& graph_sub_mgr);
    virtual ~WxToolbarPanel();

    virtual void OnNotify(uint32_t msg, const ee0::VariantSet& variants) override;

private:
    void InitLayout(const ur::Device& dev,
        const ee0::SubjectMgrPtr& graph_sub_mgr);

    void OnSelectionInsert(const ee0::VariantSet& variants);
    void OnSelectionClear(const ee0::VariantSet& variants);

private:
    ee0::SubjectMgrPtr m_graph_sub_mgr = nullptr;

    bp::WxNodeProperty* m_node_prop;

}; // WxToolbarPanel

}