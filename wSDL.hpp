#include <set>
#include <string>
#include <SDL2/SDL.h>
#include <memory>
#include <type_traits>
#include <limits>
#include <cmath>

namespace wsdl{
    // header
    enum class InitFlags : Uint32{
            audio          = SDL_INIT_AUDIO,
            events         = SDL_INIT_EVENTS,
            everything     = SDL_INIT_EVERYTHING,
            gamecontroller = SDL_INIT_GAMECONTROLLER,
            haptic         = SDL_INIT_HAPTIC,
            joystick       = SDL_INIT_JOYSTICK,
            noparachute    = SDL_INIT_NOPARACHUTE,
            video          = SDL_INIT_VIDEO,
            timer          = SDL_INIT_TIMER,
    };

    enum class WindowFlags : Uint32{
        allowHighdpi      = SDL_WINDOW_ALLOW_HIGHDPI,
        alwaysOnTop       = SDL_WINDOW_ALWAYS_ON_TOP,
        borderless        = SDL_WINDOW_BORDERLESS,
        foreign           = SDL_WINDOW_FOREIGN,
        fullscreenDesktop = SDL_WINDOW_FULLSCREEN_DESKTOP,
        hidden            = SDL_WINDOW_HIDDEN,
        inputFocus        = SDL_WINDOW_INPUT_FOCUS,
        inputGrabbed      = SDL_WINDOW_INPUT_GRABBED,
        maximized         = SDL_WINDOW_MAXIMIZED,
        minimized         = SDL_WINDOW_MINIMIZED,
        mouseCapture      = SDL_WINDOW_MOUSE_CAPTURE,
        mouseFocus        = SDL_WINDOW_MOUSE_FOCUS,
        opengl            = SDL_WINDOW_OPENGL,
        popupMenu         = SDL_WINDOW_POPUP_MENU,
        resizable         = SDL_WINDOW_RESIZABLE,
        shown             = SDL_WINDOW_SHOWN,
        skipTaskbar       = SDL_WINDOW_SKIP_TASKBAR,
        tooltip           = SDL_WINDOW_TOOLTIP,
        utility           = SDL_WINDOW_UTILITY,
        vulkan            = SDL_WINDOW_VULKAN,
        fullscreen        = SDL_WINDOW_FULLSCREEN,
    };

    enum class RendererFlags : Uint32{
        software      = SDL_RENDERER_SOFTWARE,
        accelerated   = SDL_RENDERER_ACCELERATED,
        presentvsync  = SDL_RENDERER_PRESENTVSYNC,
        targettexture = SDL_RENDERER_TARGETTEXTURE,
    };

    struct Color{
    private:
        static constexpr auto max = std::numeric_limits<Uint8>::max();

    public:
        Uint8 r, g, b, a;
        Uint8 &red = r, &green = g, &blue = b, &alpha = a;

        operator SDL_Color(){
            return SDL_Color({r, g, b, a});
        }
        Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) : r(r), g(g), b(b), a(a){}
        Color(SDL_Color c) : Color(c.r, c.g, c.b, c.a){}
        Color(void) = default;
        Color(Color&) = default;
        Color(Color&&) = default;

        Color operator+(Color const &c) const {
            auto a = c.a + this->a * (max - c.a) / float(max);
            if (a == 0) return {0, 0, 0, 0};
            Color o;
            o.a = a;
            for(auto &&x : {&Color::r, &Color::g, &Color::b}){
                o.*x = std::round((c.*x * c.a +
                         this->*x * this->a * (max - c.a) / float(max)) / o.a);
            }
            return o;
        }

        Color operator=(Color const &c){
          for (auto &x : {&Color::r, &Color::g, &Color::b, &Color::a})
            this->*x = c.*x;
          return *this;
        }

        Color operator+=(Color const &c){
            return *this = *this + c;
        }
    };


    // main
    class SDL{
    public:
        class Window;

    private:
        int initializationCode;
        std::set<Window *> windows;

    public:
        // Constructor/Destructor
        SDL(InitFlags flags)
            : initializationCode(SDL_Init(static_cast<Uint32>(flags))){}
        ~SDL(void){
            while (!windows.empty()) delete *windows.begin();
            SDL_Quit();
        }

        // Getter
        int getInitializationCode(void){ return initializationCode; }
        operator int(void){ return getInitializationCode(); }

        // around Window
        class Window{
            friend class SDL;
        public:
            class Renderer;

        private:
            SDL_Window *window;
            SDL *sdl;
            std::set<Renderer *> renderers;

            Window(SDL_Window *window, SDL *sdl) : window(window), sdl(sdl){}
            Window(Window const &w) : window(w.window), sdl(w.sdl){}
            Window &operator=(Window const &w) = default;
            Window(void) : window(nullptr){}
        public:
            // Constructor/Destructor/asignment operator
            Window(Window &&w) = default;
            Window &operator=(Window &&w) {
                this->window = w.window;
                this->sdl = w.sdl;
                w.window = nullptr;
                return *this;
            };

            ~Window(void){
             while (!renderers.empty()) delete *renderers.begin();

                if (window){
                    auto x = sdl->windows.find(this);
                    if (x != sdl->windows.end()) {
                        sdl->windows.erase(x);
                    }
                    SDL_DestroyWindow(window);
                }
            }

            operator bool(void){ return window; }

            bool operator==(Window const &w) const {
                return this->window == w.window;
            }
            bool operator!=(Window const &w) const{
                return !(this->window == w.window);
            }
            bool operator<(Window const &w) const {
                return this->window < w.window;
            }

            // around Renderer
            class Renderer{
                friend class Window;
            private:
                SDL_Renderer *renderer;
                Window *window;

                Renderer(SDL_Renderer *renderer, Window *window)
                    : renderer(renderer), window(window){}
                Renderer(Renderer const &w)
                    : renderer(w.renderer), window(w.window){}
                Renderer &operator=(Renderer const &w) = default;

            public:
                // Constructor/Destructor/asignment operator
                Renderer(void) : renderer(nullptr){}
                Renderer(Renderer &&w) = default;
                Renderer &operator=(Renderer &&w) {
                    this->renderer = w.renderer;
                    this->window = w.window;
                    w.renderer = nullptr;
                    return *this;
                };

                ~Renderer(void){
                    if (renderer){
                      auto x = window->renderers.find(this);
                      if (x != window->renderers.end()) {
                        window->renderers.erase(x);
                      }
                      SDL_DestroyRenderer(renderer);
                    }
                }

                operator bool(void){ return renderer; }

                bool operator==(Renderer const &w) const {
                    return this->renderer == w.renderer;
                }
                bool operator!=(Renderer const &w) const{
                    return !(this->renderer == w.renderer);
                }
                bool operator<(Renderer const &w) const {
                    return this->renderer < w.renderer;
                }

                void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
                    SDL_SetRenderDrawColor(renderer, r, g, b, a);
                }

                void setDrawColor(Color c){
                    setDrawColor(c.r, c.g, c.b, c.a);
                }

            };

            std::shared_ptr<Renderer> const createRenderer(
                RendererFlags flags, int index = -1
                ){
                return std::shared_ptr<Renderer> (
                    *renderers.insert(
                        new Renderer(
                            SDL_CreateRenderer(
                                window,
                                index,
                                static_cast<Uint32>(flags)
                                ),
                            this
                            )).first);
            }
        };

        std::shared_ptr<Window> const createWindow(
            std::string title, int x, int y,
            int width, int height,
            WindowFlags flags){
            // Don't use emplace because this constructor is private.
            return std::shared_ptr<Window>(
                *windows.insert(
                    new Window(
                        SDL_CreateWindow(title.c_str(), x, y, width, height,
                                         static_cast<Uint32>(flags)),
                        this)
                    ).first);
        }
    };

    template<typename T>
    struct isSDLenum : std::false_type{};

    template<>
    struct isSDLenum<InitFlags> : std::true_type{};
    template<>
    struct isSDLenum<WindowFlags> : std::true_type{};
    template<>
    struct isSDLenum<RendererFlags> : std::true_type{};

    template<typename T>
    using SDL_enum_t =
        std::enable_if_t<wsdl::isSDLenum<T>::value, std::nullptr_t>;

}

template<typename T, wsdl::SDL_enum_t<T> = nullptr>
constexpr T operator|(T l, T r){
    return static_cast<T>(static_cast<Uint32>(l) | static_cast<Uint32>(r));
}
template<typename T, wsdl::SDL_enum_t<T> = nullptr>
constexpr T operator&(T l, T r){
    return static_cast<T>(static_cast<Uint32>(l) & static_cast<Uint32>(r));
}
template<typename T, wsdl::SDL_enum_t<T> = nullptr>
constexpr T operator^(T l, T r){
    return static_cast<T>(static_cast<Uint32>(l) ^ static_cast<Uint32>(r));
}
template<typename T, wsdl::SDL_enum_t<T> = nullptr>
constexpr T operator~(T i){
    return static_cast<T>(~(static_cast<Uint32>(i)));
}
template<typename T, wsdl::SDL_enum_t<T> = nullptr>
constexpr T operator|=(T &l, T r){
    return l = l | r;
}
template<typename T, wsdl::SDL_enum_t<T> = nullptr>
constexpr T operator&=(T &l, T r){
    return l = l & r;
}
template<typename T, wsdl::SDL_enum_t<T> = nullptr>
constexpr T operator^=(T &l, T r){
    return l = l ^ r;
}
