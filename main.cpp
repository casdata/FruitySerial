#define STB_IMAGE_IMPLEMENTATION
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "UIDisplays/Hud.h"
#include "DTypes.h"
#include "imgui_internal.h"
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_thread.h>


#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#include <iomanip>

#elif defined __linux__
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "serial/serial.h"
#include "SerialManager.h"
#include "ClockTime.h"
//#include "SDL_syswm.h"

SDL_Window* gSplashWindow;
SDL_Window* gWindow;
SDL_GLContext gl_context;

SDL_Surface* iconSurface = nullptr;
SDL_Surface* gSplashSurface = nullptr;
SDL_Surface* pngSplashSurface = nullptr;

bool    splashEnable = true;
double  splashCurrentTime = 0;

Hud *hud;
SerialManager *serialManager;
ClockTime *clockTime;

bool initSDL();
void initApp(AppData* appData, MenuData* menuData);
void setTheme(AppData* appData);
void setStyleColorAppDark();
void setStyleColorAppLight();
void enableWindowTitleBar();
void disableWindowTitleBar(HitTestData &hitTestData);
SDL_HitTestResult SDLCALL hitTest(SDL_Window *window, const SDL_Point *pt, void *data);
void keyboardPostProcess(IOData *ioDat);

// Main code
int main(int, char**)
{

    Uint64 NOW = 0;
    Uint64 LAST = 0;
    double deltaTime = 0;


    MenuData menuData = {false, NORMAL, false,
                         TB_2ENABLE};
    AppData appData = {false,
                       1,
                       0,
                       LIGHT,
                       0, 0,
                       0, 0,
                       false,
                       IDLE,
                       false,
                       false,
                       nullptr};
    IOData ioData = {OFF, OFF, OFF, OFF,
                     OFF, OFF, OFF, OFF, OFF, OFF,
                     OFF, OFF, OFF, OFF, OFF, OFF, OFF,
                     OFF, OFF, OFF, OFF, OFF, OFF, OFF, "", ""};
    HitTestData hitTestData = {OFF, 0, 0};

    UI_Theme myUITheme = LIGHT;

    if(!initSDL())
        return -1;

    initApp(&appData, &menuData);

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
#endif



    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    setTheme(&appData);

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    style.WindowMenuButtonPosition = ImGuiDir_None;

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(gWindow, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("../imgui/misc/fonts/ProggyClean.ttf", FunctionTools::norm2HeightFloat(14.0f));
    //io.Fonts->AddFontFromFileTTF("../imgui/misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../imgui/misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../imgui/misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* monoFont = io.Fonts->AddFontFromFileTTF("../imgui/misc/fonts/JetBrainsMono-Regular.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //io.Fonts->AddFontFromFileTTF("../imgui/misc/fonts/Osaka Regular-Mono.otf", 16.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    appData.monoFont = io.Fonts->AddFontFromFileTTF("../imgui/misc/fonts/JetBrainsMono-Regular.ttf", FunctionTools::norm2HeightFloat(16.0f), NULL, io.Fonts->GetGlyphRangesJapanese());

    IM_ASSERT(appData.monoFont != NULL);

    // Our state
    bool show_demo_window = true;
    ImVec4 clear_color = ImVec4(0.55f, 0.55f, 0.90f, 1.00f);

    hud = new Hud();
    //hud->addWindow("It0", appData.windowCount, appData.tabCount, true);
    //hud->addWindow("It1", appData.windowCount, appData.tabCount, true);
    //hud->addWindow("It2", appData.windowCount, appData.tabCount, true);
    clockTime = new ClockTime();
    serialManager = new SerialManager(clockTime);

    NOW = SDL_GetPerformanceCounter();

    // Main loop
    bool runMainLoop = true;
    while (runMainLoop) {

        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() );                               //mS

        if(myUITheme != appData.uiTheme){
            myUITheme = appData.uiTheme;

            setTheme(&appData);
        }

        if(appData.cursorOverSubWinBorder && !appData.cursorOverInputTextBar)
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
        else if(!appData.cursorOverSubWinBorder && appData.cursorOverInputTextBar)
            ImGui::SetMouseCursor(ImGuiMouseCursor_TextInput);

        if(menuData.titleBar == TB_2DISABLE){
            disableWindowTitleBar(hitTestData);
            menuData.titleBar = TB_DISABLE;
        }
        else if(menuData.titleBar == TB_2ENABLE){
            enableWindowTitleBar();
            menuData.titleBar= TB_ENABLE;
        }

        SDL_GetWindowPosition(gWindow, &appData.windowPosX, &appData.windowPosY);
        SDL_GetWindowSize(gWindow, &appData.windowSizeW, &appData.windowSizeH);

        if(ioData.mouseBtnLeft == DOWN)
            ioData.mouseBtnLeft = ON;
        else if(ioData.mouseBtnLeft == UP)
            ioData.mouseBtnLeft = OFF;

        if(ioData.mouseBtnRight == DOWN)
            ioData.mouseBtnRight = ON;
        else if(ioData.mouseBtnRight == UP)
            ioData.mouseBtnRight = OFF;


        ioData.mouseCursorPositionRaw = ImGui::GetMousePos();
        ioData.mouseCursorPosition = FunctionTools::getMouseRealPos(appData.windowPosX, appData.windowPosY);
        hitTestData.mouseBtnLeft = ioData.mouseBtnLeft;
        hitTestData.windowSizeW = appData.windowSizeW;
        hitTestData.windowSizeH = appData.windowSizeH;


        Uint32 winFlags = SDL_GetWindowFlags(gWindow);
        if((winFlags & SDL_WINDOW_MINIMIZED) == SDL_WINDOW_MINIMIZED)
            appData.winNotMinimized = false;
        else
            appData.winNotMinimized = true;

        keyboardPostProcess(&ioData);

        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                menuData.exitApp = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(gWindow))
                menuData.exitApp = true;
            if(event.type == SDL_MOUSEBUTTONDOWN){
                if(event.button.button == SDL_BUTTON_LEFT)
                    ioData.mouseBtnLeft = DOWN;
                else if(event.button.button == SDL_BUTTON_RIGHT)
                    ioData.mouseBtnRight = DOWN;
            }
            if(event.type == SDL_MOUSEBUTTONUP){
                if(event.button.button == SDL_BUTTON_LEFT)
                    ioData.mouseBtnLeft = UP;
                else if(event.button.button == SDL_BUTTON_RIGHT)
                    ioData.mouseBtnRight = UP;
            }
            if(event.type == SDL_TEXTINPUT) {
                ioData.charBuffer.append(event.text.text);
                //std::cout<<"-> "<<event.text.text<<std::endl;
            }


            if(event.type == SDL_KEYDOWN){
                switch(event.key.keysym.sym){
                    case SDLK_c:
                        if(SDL_GetModState() & KMOD_CTRL)
                            ioData.keyCopy = DOWN;
                        break;
                    case SDLK_v:
                        if(SDL_GetModState() & KMOD_CTRL) {
                            ioData.keyPaste = DOWN;
                            ioData.charBuffer.append(SDL_GetClipboardText());
                        }
                        break;
                    case SDLK_x:
                        if(SDL_GetModState() & KMOD_CTRL){
                            std::cout<<"CUT <->"<<std::endl;
                        }
                        break;
                    case SDLK_z:
                        if(SDL_GetModState() & KMOD_SHIFT){
                            if(SDL_GetModState() & KMOD_CTRL)
                                ioData.keyRedo = DOWN;

                        }
                        else{
                            if(SDL_GetModState() & KMOD_CTRL)
                                ioData.keyUndo= DOWN;
                        }
                        break;
                    case SDLK_LEFT:
                        ioData.keyLeft = DOWN;
                        break;
                    case SDLK_RIGHT:
                        ioData.keyRight = DOWN;
                        break;
                    case SDLK_UP:
                        ioData.keyUp = DOWN;
                        break;
                    case SDLK_DOWN:
                        ioData.keyDown = DOWN;
                        break;
                    case SDLK_LCTRL:
                        ioData.keyL_Ctrl = DOWN;
                        break;
                    case SDLK_RCTRL:
                        ioData.keyR_Ctrl = DOWN;
                        break;
                    case SDLK_LSHIFT:
                        ioData.keyL_Shift = DOWN;
                        break;
                    case SDLK_RSHIFT:
                        ioData.keyR_Shift = DOWN;
                        break;
                    case SDLK_LALT:
                        ioData.keyL_Alt = DOWN;
                        break;
                    case SDLK_RALT:
                        ioData.keyR_Alt = DOWN;
                        break;
                    case SDLK_TAB:
                        ioData.keyTab = DOWN;
                        break;
                    case SDLK_BACKSPACE:
                        ioData.keyBackspace = DOWN;
                        break;
                    case SDLK_DELETE:
                        ioData.keyDel = DOWN;
                        break;
                    case SDLK_RETURN:
                        ioData.keyEnter = DOWN;
                        break;
                    case SDLK_ESCAPE:
                        ioData.keyEsc = DOWN;
                        break;
                    case SDLK_HOME:
                        ioData.keyHome = DOWN;
                        break;
                    case SDLK_END:
                        ioData.keyEnd = DOWN;
                        break;
                }
            }

            if(event.type == SDL_KEYUP){
                switch(event.key.keysym.sym){
                    case SDLK_LEFT:
                        ioData.keyLeft = UP;
                        break;
                    case SDLK_RIGHT:
                        ioData.keyRight = UP;
                        break;
                    case SDLK_UP:
                        ioData.keyUp = UP;
                        break;
                    case SDLK_DOWN:
                        ioData.keyDown = UP;
                        break;
                    case SDLK_LCTRL:
                        ioData.keyL_Ctrl = UP;
                        break;
                    case SDLK_RCTRL:
                        ioData.keyR_Ctrl = UP;
                        break;
                    case SDLK_LSHIFT:
                        ioData.keyL_Shift = UP;
                        break;
                    case SDLK_RSHIFT:
                        ioData.keyR_Shift = UP;
                        break;
                    case SDLK_LALT:
                        ioData.keyL_Alt = UP;
                        break;
                    case SDLK_RALT:
                        ioData.keyR_Alt = UP;
                        break;
                    case SDLK_TAB:
                        ioData.keyTab = UP;
                        break;
                    case SDLK_BACKSPACE:
                        ioData.keyBackspace = UP;
                        break;
                    case SDLK_DELETE:
                        ioData.keyDel = UP;
                        break;
                    case SDLK_RETURN:
                        ioData.keyEnter = UP;
                        break;
                    case SDLK_ESCAPE:
                        ioData.keyEsc = UP;
                        break;
                    case SDLK_HOME:
                        ioData.keyHome = UP;
                        break;
                    case SDLK_END:
                        ioData.keyEnd = UP;
                        break;

                }
            }





        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();


        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiWindowFlags_NoDocking
                                                    | ImGuiDockNodeFlags_NoTabBar;
        //| ImGuiDockNodeFlags_HiddenTabBar;
        ImGuiWindowFlags window_flags =
                ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;// | ImGuiWindowFlags_NoResize;
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        float framePadding = static_cast<float>(FunctionTools::norm2Height(10));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(framePadding, framePadding));
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
            window_flags |= ImGuiWindowFlags_NoBackground;
        }


        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        //ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
        ImGui::Begin("DockSpace", nullptr, window_flags);
        //ImGui::PopStyleVar();
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");


        hud->menuBar(menuData, appData, ioData);

        //std::cout<<"-> "<<appData.appState<<std::endl;

        //ImGui::ShowDemoWindow(&show_demo_window);

        //ImGuiID dockspace_id = ImGui::GetMainViewport()->ID;
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        static auto first_time = true;
        if (first_time) {
            first_time = false;
            ImGui::DockBuilderRemoveNode(dockspace_id);
            ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

            auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.10f, nullptr, &dockspace_id);    //0.07f

            ImGui::DockBuilderDockWindow("Main Window", dockspace_id);
            ImGui::DockBuilderDockWindow("InputBar", dock_id_down);

            ImGui::DockBuilderFinish(dockspace_id);
        }


        ImGui::End();


        if(splashEnable){
            splashCurrentTime += deltaTime;
            if(splashCurrentTime > 1600 ){
                splashEnable = false;

                SDL_ShowWindow(gWindow);

                SDL_FreeSurface(pngSplashSurface);
                pngSplashSurface = nullptr;

                SDL_DestroyWindow(gSplashWindow);
                gSplashWindow = nullptr;

                IMG_Quit();

            }
        }


        hud->drawMainWin(deltaTime, appData, serialManager);

        //ImGui::ShowDemoWindow();  //5tat

        clockTime->update();
        serialManager->update();

        hud->update(deltaTime, menuData, appData, ioData, serialManager);

        hud->statusBar();

        if (menuData.exitApp) {
            menuData.exitApp = false;
            if (appData.disableExitMessage)
                runMainLoop = false;
            else
                ImGui::OpenPopup("Comfirm Close App");
        }

        if(menuData.maximize == SET_2MAXIMIZE){
            menuData.maximize = MAXIMIZE;

            SDL_MaximizeWindow(gWindow);
        }
        else if(menuData.maximize == SET_2NORMAL){
            menuData.maximize = NORMAL;

            SDL_RestoreWindow(gWindow);
        }

        if (hud->closeAppDialog(appData))
            runMainLoop = false;


        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int) io.DisplaySize.x, (int) io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
                     clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            SDL_Window *backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);

            if(splashEnable)
                SDL_UpdateWindowSurface(gSplashWindow);
        }

        SDL_GL_SwapWindow(gWindow);
    }

    // Cleanup
    delete serialManager;
    delete clockTime;
    delete hud;


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(gWindow);
    SDL_FreeSurface(iconSurface);

    SDL_Quit();

    return 0;
}

bool initSDL(){

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        std::cout<<"Error: "<<SDL_GetError()<<std::endl;
        return false;
    }

#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);


    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);// | SDL_WINDOW_BORDERLESS);

    gSplashWindow = SDL_CreateWindow("Loading", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 470, 626, window_flags | SDL_WINDOW_BORDERLESS);
    gl_context = SDL_GL_CreateContext(gSplashWindow);

    //SDL_SetWindowOpacity(gSplashWindow, 0.9f);
    /*
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(gSplashWindow, &wmInfo);
    HWND hWnd = wmInfo.info.win.window;

    SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE | WS_EX_LAYERED));

    SetLayeredWindowAttributes(hWnd,RGB(255, 255, 255),0, LWA_COLORKEY );
    */

    gWindow = SDL_CreateWindow("Fruity Serial Terminal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, window_flags | SDL_WINDOW_HIDDEN );  //1280, 980,
    gl_context = SDL_GL_CreateContext(gWindow);
    SDL_GL_MakeCurrent(gWindow, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    int imgFlags = IMG_INIT_PNG;

    SDL_Surface* loadedSurface = IMG_Load("../Assets/icon.png");
    if(loadedSurface != nullptr){
        iconSurface = SDL_ConvertSurface(loadedSurface, loadedSurface->format, 0);

        if(iconSurface == nullptr)
            std::cout << "ERROR: can't create png icon" << std::endl;
        else
            SDL_SetWindowIcon(gWindow, loadedSurface);



        SDL_FreeSurface(loadedSurface);
    }

    loadedSurface = IMG_Load("../Assets/splash.png");
    if(loadedSurface != nullptr){
        pngSplashSurface = SDL_ConvertSurface(loadedSurface, loadedSurface->format, 0);

        if(pngSplashSurface == nullptr)
            std::cout << "ERROR: can't create png splash" << std::endl;
        else{

            if(!(IMG_Init(imgFlags) & imgFlags))
                std::cout<<"SDL_image could not initialize!"<<std::endl;
            else{
                gSplashSurface = SDL_GetWindowSurface(gSplashWindow);
                SDL_BlitSurface(pngSplashSurface, nullptr, gSplashSurface, nullptr);
            }

        }


        SDL_FreeSurface(loadedSurface);
    }


    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    std::cout<<"DisplayMode: "<<dm.w<<" "<<dm.h<<std::endl;

    int testWidth = dm.w;
    int testHeight = dm.h;

    //testWidth = 1280;
    //testHeight = 720;

    FunctionTools::norm2Width(0, true, testWidth);
    FunctionTools::norm2Height(0, true, testHeight);
    FunctionTools::norm2HeightFloat(0, true, testHeight);


    SDL_SetWindowMinimumSize(gWindow, 400, 300);
    //SDL_SetWindowMaximumSize(gWindow, dm.w, dm.h);

    /*
    float ddpi = 0;
    float hdpi = 0;
    float vdpi = 0;

    SDL_GetDisplayDPI(0, &ddpi, &hdpi, &vdpi);

    std::cout<<"Display 0: "<<ddpi<<" "<<hdpi<<" "<<vdpi<<std::endl;

    SDL_GetDisplayDPI(1, &ddpi, &hdpi, &vdpi);

    std::cout<<"Display 1: "<<ddpi<<" "<<hdpi<<" "<<vdpi<<std::endl;
    */


    return true;
}

void initApp(AppData* appData, MenuData* menuData) {

#ifdef _WIN32
    if(mkdir("/")){

    }
#elif defined __linux__

    mkdir("Profiles", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);


#endif

                                                                                                                        //data.bin file

    std::ifstream readFile(dataFileName);

    if(readFile.is_open())
        readFile.close();
    else{
        std::ofstream createFile(dataFileName);

        if(createFile.is_open()){
            createFile.put((char)0);
            createFile.put((char)0);
            createFile.put((char)1);
            createFile.put('~');
            createFile.close();
        }
    }

    std::fstream  startAppData(dataFileName, std::ios::out | std::ios::in);

    if(startAppData.is_open()){

        //get length of file
        startAppData.seekg(0, startAppData.end);
        int length = startAppData.tellg();
        startAppData.seekg(0, startAppData.beg);

        char* buffer = new char [length];

        startAppData.read(buffer, length);

        if(buffer[3] == '~'){
            if(buffer[0] == (char)1)
                appData->disableExitMessage = true;

            if(buffer[1] == (char)1)
                appData->uiTheme = DARK;

            if(buffer[2] == (char)0)
                menuData->titleBar = TB_2DISABLE;

        }

        delete[] buffer;

        startAppData.close();
    }

}

void setTheme(AppData* appData){

    switch(appData->uiTheme){
        case DARK:
            ImGui::StyleColorsDark();
            setStyleColorAppDark();
            break;
        case CLASSIC:
            ImGui::StyleColorsClassic();
            break;
        case LIGHT:
            ImGui::StyleColorsLight();
            setStyleColorAppLight();
            break;
    }

}

void setStyleColorAppDark() {
    ImGuiStyle *style = &ImGui::GetStyle();
    ImVec4 *colors = style->Colors;

    colors[ImGuiCol_MenuBarBg] = ImVec4(0.1294f, 0.1450f, 0.1686f, 1.00f);
}

void setStyleColorAppLight(){
    ImGuiStyle *style = &ImGui::GetStyle();
    ImVec4 *colors = style->Colors;

    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.9490f, 0.9490f, 0.9490f, 1.00f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

}

void enableWindowTitleBar(){
    if(SDL_SetWindowHitTest(gWindow, nullptr, nullptr) == -1)
        SDL_Log("Hit test failed!\n");

    SDL_SetWindowBordered(gWindow, SDL_TRUE);
}

void disableWindowTitleBar(HitTestData &hitTestData){
    if(SDL_SetWindowHitTest(gWindow, hitTest, &hitTestData) == -1)
        SDL_Log("Hit test failed!\n");

    SDL_SetWindowBordered(gWindow, SDL_FALSE);
}

SDL_HitTestResult SDLCALL hitTest(SDL_Window *window, const SDL_Point *pt, void *data){

    //std::cout<<"-> "<<pt->x<<" "<<pt->y<<std::endl;
    HitTestData *hitTestData = reinterpret_cast<HitTestData*>(data);                                                    //(*(HitTestData*)data)


                                                                                                                        //TEMP DELETE after resize code implemented;
    int minXPoint = hitTestData->windowSizeW * 0.1582f;
    int maxXPoint = hitTestData->windowSizeW * 0.8222f;

    if(pt->x > minXPoint && pt->x < maxXPoint){
        if(pt->y > 0 && pt->y < MENU_BAR_HIGHT)
            return SDL_HITTEST_DRAGGABLE;
    }


    if(pt->x < RESIZE_OFFSET && pt->y < RESIZE_OFFSET)
        return SDL_HITTEST_RESIZE_TOPLEFT;
    else if (pt->x > RESIZE_OFFSET && pt->x < hitTestData->windowSizeW - RESIZE_OFFSET && pt->y < RESIZE_OFFSET)
        return SDL_HITTEST_RESIZE_TOP;
    else if (pt->x > hitTestData->windowSizeW - RESIZE_OFFSET && pt->y < RESIZE_OFFSET)
        return SDL_HITTEST_RESIZE_TOPRIGHT;
    else if (pt->x > hitTestData->windowSizeW - RESIZE_OFFSET && pt->y > RESIZE_OFFSET && pt->y < hitTestData->windowSizeH - RESIZE_OFFSET)
        return SDL_HITTEST_RESIZE_RIGHT;
    else if (pt->x > hitTestData->windowSizeW - RESIZE_OFFSET && pt->y > hitTestData->windowSizeH - RESIZE_OFFSET)
        return SDL_HITTEST_RESIZE_BOTTOMRIGHT;
    else if (pt->x < hitTestData->windowSizeW - RESIZE_OFFSET && pt->x > RESIZE_OFFSET && pt->y > hitTestData->windowSizeH - RESIZE_OFFSET)
        return SDL_HITTEST_RESIZE_BOTTOM;
    else if (pt->x < RESIZE_OFFSET && pt->y > hitTestData->windowSizeH - RESIZE_OFFSET)
        return SDL_HITTEST_RESIZE_BOTTOMLEFT;
    else if (pt->x < RESIZE_OFFSET && pt->y < hitTestData->windowSizeH - RESIZE_OFFSET && pt->y > RESIZE_OFFSET)
        return SDL_HITTEST_RESIZE_LEFT;

    return SDL_HITTEST_NORMAL;
}

void keyboardPostProcess(IOData *ioDat){

    if(!ioDat->charBuffer.empty())
        ioDat->charBuffer.clear();

    if(ioDat->keyLeft == DOWN)
        ioDat->keyLeft = ON;
    else if(ioDat->keyLeft == UP)
        ioDat->keyLeft = OFF;

    if(ioDat->keyRight == DOWN)
        ioDat->keyRight = ON;
    else if(ioDat->keyRight == UP)
        ioDat->keyRight = OFF;

    if(ioDat->keyUp == DOWN)
        ioDat->keyUp = ON;
    else if(ioDat->keyUp == UP)
        ioDat->keyUp = OFF;

    if(ioDat->keyDown == DOWN)
        ioDat->keyDown = ON;
    else if(ioDat->keyDown == UP)
        ioDat->keyDown = OFF;

    if(ioDat->keyL_Ctrl == DOWN)
        ioDat->keyL_Ctrl = ON;
    else if(ioDat->keyL_Ctrl == UP)
        ioDat->keyL_Ctrl = OFF;

    if(ioDat->keyR_Ctrl == DOWN)
        ioDat->keyR_Ctrl = ON;
    else if(ioDat->keyR_Ctrl == UP)
        ioDat->keyR_Ctrl = OFF;

    if(ioDat->keyL_Shift == DOWN)
        ioDat->keyL_Shift = ON;
    else if(ioDat->keyL_Shift == UP)
        ioDat->keyL_Shift = OFF;

    if(ioDat->keyR_Shift == DOWN)
        ioDat->keyR_Shift = ON;
    else if(ioDat->keyR_Shift == UP)
        ioDat->keyR_Shift = OFF;

    if(ioDat->keyL_Alt == DOWN)
        ioDat->keyL_Alt = ON;
    else if(ioDat->keyL_Alt == UP)
        ioDat->keyL_Alt = OFF;

    if(ioDat->keyR_Alt == DOWN)
        ioDat->keyR_Alt = ON;
    else if(ioDat->keyR_Alt == UP)
        ioDat->keyR_Alt = OFF;

    if(ioDat->keyTab == DOWN)
        ioDat->keyTab = ON;
    else if(ioDat->keyTab == UP)
        ioDat->keyTab = OFF;

    if(ioDat->keyBackspace == DOWN)
        ioDat->keyBackspace = ON;
    else if(ioDat->keyBackspace == UP)
        ioDat->keyBackspace = OFF;

    if(ioDat->keyDel == DOWN)
        ioDat->keyDel = ON;
    else if(ioDat->keyDel == UP)
        ioDat->keyDel = OFF;

    if(ioDat->keyEnter == DOWN)
        ioDat->keyEnter = ON;
    else if(ioDat->keyEnter == UP)
        ioDat->keyEnter = OFF;

    if(ioDat->keyEsc == DOWN)
        ioDat->keyEsc = ON;
    else if(ioDat->keyEsc == UP)
        ioDat->keyEsc = OFF;

    if(ioDat->keyHome == DOWN)
        ioDat->keyHome = ON;
    else if(ioDat->keyHome == UP)
        ioDat->keyHome = OFF;

    if(ioDat->keyEnd == DOWN)
        ioDat->keyEnd = ON;
    else if(ioDat->keyEnd == UP)
        ioDat->keyEnd = OFF;

    if(ioDat->keyCopy == DOWN)
        ioDat->keyCopy = OFF;

    if(ioDat->keyPaste == DOWN)
        ioDat->keyPaste = OFF;

    if(ioDat->keyCut == DOWN)
        ioDat->keyCut = OFF;

    if(ioDat->keyUndo == DOWN)
        ioDat->keyUndo = OFF;

    if(ioDat->keyRedo == DOWN)
        ioDat->keyRedo = OFF;

}