#pragma once

#include "tdview/Node.h"

namespace tdv
{

void nodes_regist_rttr();

namespace node
{

#define EXE_FILEPATH "tdview/node_def_gen.h"
#include "tdview/node_regist_cfg.h"
#undef EXE_FILEPATH

}

}