#include "tdv/PreviewBuilder.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>
#include <ee0/MsgHelper.h>

#include <td/ParamImpl.h>
#include <td/operator/Line.h>
#include <ns/NodeFactory.h>
#include <node0/SceneNode.h>
#include <node3/CompShape.h>
#include <geoshape/Polyline3D.h>

namespace tdv
{

PreviewBuilder::PreviewBuilder(const ee0::SubjectMgrPtr& sub_mgr, const dag::Graph<td::ParamType>& eval,
                               const std::unordered_map<const dag::Node<td::ParamType>*, n0::SceneNodePtr>& back2node)
    : m_sub_mgr(sub_mgr)
    , m_eval(eval)
    , m_back2node(back2node)
{
}

void PreviewBuilder::Build()
{
    m_sub_mgr->NotifyObservers(ee0::MSG_SCENE_NODE_CLEAR);
    auto& ops = m_eval.GetAllNodes();
    for (auto& pair : ops)
    {
        auto& op = pair.second;

        auto itr = m_back2node.find(op.get());
        assert(itr != m_back2node.end());
        auto scene_node = itr->second;

        auto type = op->get_type();
        if (type == rttr::type::get<td::op::Line>())
        {
            auto param = std::static_pointer_cast<td::Operator>(op)->GetValue(0);
            assert(param && param->Type() == td::ParamType::Geo);
            auto geo_param = std::static_pointer_cast<td::GeoParam>(param);
            auto& geo = geo_param->GetGeo();

            std::vector<std::shared_ptr<gs::Shape3D>> shapes;
            auto& pts = geo.GetPoints();
            for (auto& prim : geo.GetPrimitives())
            {
                std::vector<sm::vec3> vts;
                vts.reserve(prim.points.size());
                for (auto& idx : prim.points) {
                    assert(idx < pts.size());
                    vts.push_back(pts[idx].pos);
                }
                shapes.push_back(std::make_shared<gs::Polyline3D>(vts));
            }
            auto node = ns::NodeFactory::Create3D();
            auto& cshape = node->AddUniqueComp<n3::CompShape>();
            cshape.SetShapes(shapes);
            ee0::MsgHelper::InsertNode(*m_sub_mgr, node);

            auto& cshape2 = scene_node->HasUniqueComp<n3::CompShape>() ?
                scene_node->GetUniqueComp<n3::CompShape>() :
                scene_node->AddUniqueComp<n3::CompShape>();
            cshape2.SetShapes(shapes);
        }
    }
}

}