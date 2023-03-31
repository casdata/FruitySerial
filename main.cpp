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

SDL_Window* gWindow;
SDL_GLContext gl_context;

Hud *hud;
SerialManager *serialManager;
ClockTime *clockTime;

bool initSDL();
void initApp(AppData* appData);
void setTheme(AppData* appData);
void setStyleColorAppDark();
void setStyleColorAppLight();
void enableWindowTitleBar();
void disableWindowTitleBar(HitTestData &hitTestData);
SDL_HitTestResult SDLCALL hitTest(SDL_Window *window, const SDL_Point *pt, void *data);

// Main code
int main(int, char**)
{

    float deltaTime = 0.0f;
    MenuData menuData = {false, NORMAL, false,
                         TB_2ENABLE};
    AppData appData = {false,
                       1,
                       0,
                       DARK,
                       0, 0,
                       0, 0,
                       IDLE,
                       false,
                       nullptr};
    IOData ioData = {OFF, OFF};
    HitTestData hitTestData = {OFF, 0, 0};

    if(!initSDL())
        return -1;

    initApp(&appData);

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


    // Main loop
    bool runMainLoop = true;
    while (runMainLoop) {

        static bool debugLockClick = false;
        static int debugClickX = 0;

        if(appData.cursorOverSubWinBorder)
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);

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


        ioData.mouseCursorPosition = FunctionTools::getMouseRealPos(appData.windowPosX, appData.windowPosY);
        hitTestData.mouseBtnLeft = ioData.mouseBtnLeft;
        hitTestData.windowSizeW = appData.windowSizeW;
        hitTestData.windowSizeH = appData.windowSizeH;

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

            auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.10f, nullptr, &dockspace_id);

            ImGui::DockBuilderDockWindow("Main Window", dockspace_id);
            ImGui::DockBuilderDockWindow("Tools", dock_id_down);

            ImGui::DockBuilderFinish(dockspace_id);
        }


        ImGui::End();


        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove
                                       | ImGuiWindowFlags_NoTitleBar
                                       | ImGuiWindowFlags_NoCollapse;


        ImGui::Begin("Tools", NULL, windowFlags);
        ImGui::Text("Hello, down!");
        ImGui::End();


        hud->drawMainWin(appData, serialManager);

        ImGui::ShowDemoWindow();

        /*
        ImGui::Begin("Main Window", NULL, windowFlags);


            ImGuiWindowFlags wFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysVerticalScrollbar;

            static ImVec2 debugWinSize(ImGui::GetContentRegionAvail().x * 0.3f, ImGui::GetContentRegionAvail().y);


            if(debugLockClick && ioData.mouseBtnLeft == ON){
                int cursorXPos = FunctionTools::getMouseRealPos(appData.windowPosX, appData.windowPosY).x;

                debugWinSize.x = cursorXPos - ImGui::GetStyle().ItemSpacing.x;
                //debugWinSize.x =
            }
            else if(debugLockClick && ioData.mouseBtnLeft == UP)
                debugLockClick = false;

            ImGui::BeginChild("childA", debugWinSize, true, wFlags);


            ImGui::Button("Hello there my friend whats up?");

            std::cout<<"childA: "<<ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows)<<std::endl;

            ImGui::EndChild();

            ImGui::SameLine(0, 0);

            ImGui::BeginChild("childB", ImVec2(0, ImGui::GetContentRegionAvail().y), true, wFlags );

                std::cout<<"childB: "<<ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows)<<std::endl;

                if(!debugLockClick) {
                    int wPosX = FunctionTools::getRealValue(ImGui::GetWindowPos().x, appData.windowPosX);
                    int wPosY = FunctionTools::getRealValue(ImGui::GetWindowPos().y, appData.windowPosY);

                    ImVec2 mousePos = FunctionTools::getMouseRealPos(appData.windowPosX, appData.windowPosY);

                    if ((int) mousePos.y >= wPosY && mousePos.y < (wPosY + ImGui::GetWindowSize().y)) {
                        if ((int) mousePos.x >= wPosX && mousePos.x < (wPosX + 8)) {
                            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
                            if(ioData.mouseBtnLeft == DOWN)
                                debugLockClick = true;

                        } else
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
                    } else
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);

                }


            ImGui::EndChild();

        ImGui::End();
        */


        /*
        ImGui::Begin("Main Window", &debugChange2, windowFlags);
        ImGui::BeginTabBar("Tab");
        if (ImGui::BeginTabItem("Test_1")) {
            if (ImGui::BeginPopupContextItem()) {

                ImGui::Spacing();

                if (ImGui::Selectable("Close")) {

                }

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                if (ImGui::Selectable("Split Left")) {

                }

                ImGui::Spacing();
                ImGui::Spacing();

                if (ImGui::Selectable("Split Right")) {

                }

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                if (ImGui::Selectable("Rename Tab")) {

                }

                ImGui::Spacing();

                ImGui::EndPopup();
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Test_2"))
            ImGui::EndTabItem();
        ImGui::EndTabBar();

        ImGui::Text("Nani!");

        if (ImGui::Selectable("Change Size"))
            debugChange = true;

        ImGui::End();
        */

        ImGui::PushFont(appData.monoFont);
        //ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(255,255,255,255));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0,0,0,255));
        ImGui::Begin("Nothing");

        std::string bt;

        //ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,0,0,255));
        //bt.assign("[");

        //ImGui::TextUnformatted(bt.c_str());
        //ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 255, 255));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 100, 255, 255));
        ImGui::TextUnformatted("[");
        ImGui::SameLine(0,0);
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,0,0,255));
        ImGui::TextUnformatted("LF");
        ImGui::SameLine(0,0);
        ImGui::PopStyleColor();
        ImGui::TextUnformatted("]");
        ImGui::SameLine(0,0);
        ImGui::PopStyleColor();
        ImGui::TextUnformatted(" what");

        //ImGui::SameLine();
        ImGui::TextUnformatted("[LF] what");

        ImGui::End();
        ImGui::PopStyleColor();
        ImGui::PopFont();



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
    gWindow = SDL_CreateWindow("Fruity Serial Terminal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, window_flags);  //1280, 980,
    gl_context = SDL_GL_CreateContext(gWindow);
    SDL_GL_MakeCurrent(gWindow, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

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

void initApp(AppData* appData) {

#ifdef _WIN32
    if(mkdir("/")){

    }
#elif defined __linux__

    mkdir("Profiles", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);


#endif



    std::ifstream readFile(dataFileName);

    if(readFile.is_open())
        readFile.close();
    else{
        std::ofstream createFile(dataFileName);

        if(createFile.is_open()){
            createFile.put((char)0);
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

        if(buffer[1] == '~' && buffer[0] == (char)1)
            appData->disableExitMessage = true;

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