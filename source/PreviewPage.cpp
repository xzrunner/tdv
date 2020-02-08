#include "tdview/PreviewPage.h"

#include <ee0/WxStagePage.h>
#include <ee3/WxStageCanvas.h>
#include <ee3/CameraDriveOP.h>

namespace tdv
{

PreviewPage::PreviewPage(ee0::WxStagePage& stage_page)
    : m_stage_page(stage_page)
{
}

PreviewPage::~PreviewPage()
{
}

void PreviewPage::OnNotify(uint32_t msg, const ee0::VariantSet& variants)
{
}

void PreviewPage::InitEditOP()
{
    auto canvas = m_stage_page.GetImpl().GetCanvas();
    assert(canvas);
    auto sub_mgr = m_stage_page.GetSubjectMgr();
    assert(sub_mgr);

    auto& vp = std::static_pointer_cast<ee3::WxStageCanvas>(canvas)->GetViewport();
    auto edit_op = std::make_shared<ee3::CameraDriveOP>(
        canvas->GetCamera(), vp, sub_mgr
    );

    m_stage_page.GetImpl().SetEditOP(edit_op);
}

}