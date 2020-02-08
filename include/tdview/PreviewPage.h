#pragma once

#include <ee0/Observer.h>

#include <vector>

namespace ee0 { class WxStagePage; }

namespace tdv
{

class PreviewPage : public ee0::Observer
{
public:
    PreviewPage(ee0::WxStagePage& stage_page);
    virtual ~PreviewPage();

    virtual void OnNotify(uint32_t msg, const ee0::VariantSet& variants) override;

    void InitEditOP();

private:
    ee0::WxStagePage& m_stage_page;

    std::vector<uint32_t> m_messages;

}; // PreviewPage

}