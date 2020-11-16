#pragma once

namespace sacred
{
    namespace gui
    {
        enum MyNodeTypes
        {
            MNT_UNARY_NODE = 0,
            MNT_BINARY_NODE = 1,
            MNT_GENERATOR_NODE = 2,
            MNT_RENDER_NODE = 3,
            MNT_COUNT
        };

        static const char *MyNodeTypeNames[] = {
            "Render",
            0,
            0
        };
    }
}
