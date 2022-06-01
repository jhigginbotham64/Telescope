//
// Created by clem on 5/31/22.
//

#include <include/renderable.hpp>

namespace ts::detail
{
    void forward_render(RenderTarget* target, const Renderable* object, Transform transform)
    {
        object->render(target, transform);
    }
}
