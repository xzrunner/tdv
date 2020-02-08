#pragma once

#include "tdv/Node.h"

namespace tdv
{

void nodes_regist_rttr();

namespace node
{

#define EXE_FILEPATH "tdv/node_def_gen.h"
#include "tdv/node_regist_cfg.h"
#undef EXE_FILEPATH

}

}