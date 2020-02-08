#include "tdview/PinCallback.h"
#include "tdview/PinType.h"

#include <painting0/Color.h>
#include <blueprint/Pin.h>

#include <string>

#include <assert.h>

namespace
{

const pt0::Color COL_DEFAULT = pt0::Color(255, 255, 255);

const pt0::Color COL_GEO  = pt0::Color(132, 228, 231);

std::string get_desc_func(const std::string& name, int type)
{
    std::string ret = name;
    switch (type)
    {
    case tdv::PIN_GEO:
        ret += "(G)";
        break;

    default:
        assert(0);
    }
    return ret;
}

const pt0::Color& get_color_func(int type)
{
    switch (type)
    {
    case tdv::PIN_GEO:
        return COL_GEO;

    default:
        return COL_DEFAULT;
    }
}

bool can_type_cast_func(int type_from, int type_to)
{
    if (type_from == type_to) {
        return true;
    }
    if (type_from == bp::PIN_ANY_VAR ||
        type_to == bp::PIN_ANY_VAR) {
        return true;
    }

    return false;
}

}

namespace tdv
{

void InitPinCallback()
{
    bp::Pin::SetExtendFuncs({
        get_desc_func,
        get_color_func,
        can_type_cast_func
    });
}

}