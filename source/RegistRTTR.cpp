#include "tdv/RegistNodes.h"

#include <ee0/ReflectPropTypes.h>

namespace bp
{
extern void regist_sm_rttr();
extern void regist_pt0_rttr();
}

namespace tdv
{

void regist_rttr()
{
    bp::regist_sm_rttr();
    bp::regist_pt0_rttr();

//	prop_types_regist_rttr();
	nodes_regist_rttr();
}

}