#include "tdview/RegistNodes.h"

#include <ee0/ReflectPropTypes.h>

#include <js/RTTR.h>

RTTR_REGISTRATION
{

// base

rttr::registration::class_<tdv::Node>("tdv::node")
.property("name", &tdv::Node::GetName, &tdv::Node::SetName)
(
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Name"))
)
;

#define EXE_FILEPATH "tdview/node_rttr_gen.h"
#include "tdview/node_regist_cfg.h"
#undef EXE_FILEPATH

}

namespace tdv
{

void nodes_regist_rttr()
{
}

}