#include <memory>
#include "wSDL.hpp"

int main(int, char*[]){
    std::unique_ptr<wsdl::SDL> sdl = std::make_unique<wsdl::SDL>(wsdl::InitFlags::audio);
    {
        auto w = sdl->createWindow("aaa", 100, 100, 100, 100,
                                   wsdl::WindowFlags::borderless);

        auto r = w->createRenderer(wsdl::RendererFlags::software);
    }
    return 0;
}
