//
// Created by clem on 5/31/22.
//

#include <include/renderable.hpp>

namespace ts::detail
{
    void forward_render(const Renderable * object, RenderTarget * target)
    {
        object->render(target);
    }
}
