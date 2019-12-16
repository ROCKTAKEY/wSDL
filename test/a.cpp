#include <memory>
#include <cassert>

#include "wSDL.hpp"

int main(int, char*[]){
    std::unique_ptr<wsdl::SDL> sdl =
        std::make_unique<wsdl::SDL>(wsdl::InitFlags::audio);
    {
        auto w = sdl->createWindow("aaa", 100, 100, 100, 100,
                                   wsdl::WindowFlags::borderless);

        auto r = w->createRenderer(wsdl::RendererFlags::software);
    }
    wsdl::Color c = {10, 10, 10, 255};
    wsdl::Color d = {20, 30, 10, 20};

    auto e = c + d;

    assert(e.a == 255 && e.r == 11 && e.g == 12 && e.b == 10);

    return 0;
}
