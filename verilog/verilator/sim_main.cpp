// Verilator + Dear ImGui harness for the Dorado RTL.
//
// Deliberately compact and self-contained: SDL2 + OpenGL2 + imgui directly,
// rather than the full MiSTer sim_* helper library that ships in sim/. Those
// helpers (sim_video, sim_bus, sim_input, ...) are kept alongside because
// they are what a MiSTer core needs once the design produces video and reads
// a bus -- but the Dorado boards do not compute yet, and a harness that
// builds and runs today is worth more than one that models peripherals the
// RTL cannot yet drive.
//
// What it does: clock `emu` at the Dorado's 60 ns microcycle, let you
// run/step/reset, and show what the design is doing. Extend the panels as
// the cell library gains behaviour.

#include <verilated.h>
#include "Vemu.h"

#include <SDL.h>
#include <SDL_opengl.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"

#include <cstdio>
#include <cstring>
#include <deque>
#include <string>

// The real machine's microinstruction is 60 ns (16.667 MHz). Every timing
// figure in this project is quoted in microinstructions, so the harness
// counts the same unit -- see docs/performance-methodology.md, where reading
// a different counter as microinstructions produced a published speed figure
// that was wrong by 3.70x.
static const double kMicrocycleNs = 60.0;

static Vemu*       top = nullptr;
static vluint64_t  main_time = 0;

double sc_time_stamp() { return (double)main_time; }

// One full clock: the design is posedge-triggered, so a cycle is low->high.
static void tick()
{
    top->clk_sys = 0; top->eval();
    top->clk_sys = 1; top->eval();
    main_time++;
}

int main(int argc, char** argv)
{
    Verilated::commandArgs(argc, argv);

    bool     headless = false;
    uint64_t run_cycles = 0;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--headless")) headless = true;
        else if (!strcmp(argv[i], "--cycles") && i + 1 < argc)
            run_cycles = strtoull(argv[++i], nullptr, 0);
    }

    top = new Vemu;
    top->CLK_50M    = 0;
    top->RESET      = 1;
    top->clk_sys    = 0;
    top->joystick_0 = 0;
    top->ps2_key    = 0;
    top->probe_sel  = 0;

    // Hold reset briefly, as the board would at power-up.
    for (int i = 0; i < 8; i++) tick();
    top->RESET = 0;

    // Headless mode exists so the harness can be a GATE, not only a window:
    // it runs a fixed number of microcycles and prints what the design
    // reached, which is diffable in CI.
    if (headless) {
        if (!run_cycles) run_cycles = 1000;
        for (uint64_t i = 0; i < run_cycles; i++) tick();
        printf("dorado-rtl: %llu microcycles = %.3f us emulated; "
               "status_out=%u\n",
               (unsigned long long)run_cycles,
               run_cycles * kMicrocycleNs / 1000.0,
               (unsigned)top->status_out);
        top->final();
        delete top;
        return 0;
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_Window* window = SDL_CreateWindow(
        "Xerox Dorado -- RTL (generated from PARC's Sil netlists)",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 800,
        (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                          SDL_WINDOW_ALLOW_HIGHDPI));
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        return 1;
    }
    SDL_GLContext gl = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl);
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window, gl);
    ImGui_ImplOpenGL2_Init();

    bool     running = true;
    bool     free_run = false;
    int      cycles_per_frame = 1000;
    std::deque<float> history;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL2_ProcessEvent(&e);
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_WINDOWEVENT &&
                e.window.event == SDL_WINDOWEVENT_CLOSE &&
                e.window.windowID == SDL_GetWindowID(window)) running = false;
        }

        if (free_run)
            for (int i = 0; i < cycles_per_frame; i++) tick();

        history.push_back((float)top->status_out);
        if (history.size() > 240) history.pop_front();

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // ---- control -------------------------------------------------
        ImGui::Begin("Dorado");
        ImGui::Text("microcycles: %llu", (unsigned long long)main_time);
        ImGui::Text("emulated:    %.3f us  (60 ns microcycle)",
                    main_time * kMicrocycleNs / 1000.0);
        ImGui::Separator();
        if (ImGui::Button(free_run ? "Pause" : "Run")) free_run = !free_run;
        ImGui::SameLine();
        if (ImGui::Button("Step"))   tick();
        ImGui::SameLine();
        if (ImGui::Button("Step 1k")) for (int i = 0; i < 1000; i++) tick();
        ImGui::SameLine();
        if (ImGui::Button("Reset")) {
            top->RESET = 1;
            for (int i = 0; i < 8; i++) tick();
            top->RESET = 0;
        }
        ImGui::SliderInt("cycles/frame", &cycles_per_frame, 1, 100000);
        ImGui::End();

        // ---- probe ---------------------------------------------------
        ImGui::Begin("Probe");
        static int sel = 0;
        const char* names[] = { "ucycle counter", "reset" };
        ImGui::Combo("signal", &sel, names, IM_ARRAYSIZE(names));
        top->probe_sel = (uint16_t)sel;
        top->eval();
        ImGui::Text("value: 0x%08X  (%u)",
                    (unsigned)top->probe_val, (unsigned)top->probe_val);
        if (!history.empty())
            ImGui::PlotLines("status_out", &history[0], (int)history.size(),
                             0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0, 60));
        ImGui::End();

        // ---- what this is --------------------------------------------
        ImGui::Begin("About");
        ImGui::TextWrapped(
            "The RTL under ../generated is GENERATED from Xerox PARC's own "
            "Sil design-automation output -- the wire lists the Dorado was "
            "actually built from. 16 boards, 12,841 nets, 5,563 packages.");
        ImGui::Separator();
        ImGui::TextWrapped(
            "The boards ELABORATE but do not yet COMPUTE: the 125 cell "
            "models have correct ports (generated from PARC's own "
            "EclDict/TtlDict) and TODO bodies. 48 logic types cover 90%% of "
            "logic packages, so that is the order to fill them in.");
        ImGui::Separator();
        ImGui::TextWrapped(
            "Top module is MiSTer's `emu`, so RTL that runs here runs on "
            "MiSTer without a second port.");
        ImGui::End();

        ImGui::Render();
        ImGuiIO& io = ImGui::GetIO();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.08f, 0.09f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl);
    SDL_DestroyWindow(window);
    SDL_Quit();

    top->final();
    delete top;
    return 0;
}
