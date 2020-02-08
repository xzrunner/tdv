#pragma once

#include <td/typedef.h>
#include <td/ParamType.h>

namespace td { class Operator; }
namespace bp { class Node; class Pin; }

namespace tdv
{

class Evaluator;

class TdAdapter
{
public:
    static void UpdatePropBackFromFront(const bp::Node& front,
        td::Operator& back, const Evaluator& eval);

    static td::OpPtr CreateBackFromFront(const bp::Node& front);

    static int TypeBackToFront(td::ParamType type);

}; // SOP

}