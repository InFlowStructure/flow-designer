#include "PropertyWindow.hpp"

#include <cxxopts.hpp>
#include <flow/ui/Config.hpp>
#include <flow/ui/Editor.hpp>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <csignal>
#include <fstream>
#include <iostream>

int main(int argc, char** argv)
{
    std::string filename;

#ifndef FLOW_WINDOWS
    // clang-format off
    cxxopts::Options options("FlowCode");
    options.add_options()
        ("f,flow", "Flow file to open", cxxopts::value<std::string>())
        ("l,log_level", "Logging level [trace = 0, debug = 1, info = 2, warn = 3, err = 4, critical = 5, off = 6]", cxxopts::value<int>())
        ("h,help", "Print usage");
    // clang-format on

    cxxopts::ParseResult result;

    try
    {
        result = options.parse(argc, argv);
    }
    catch (const cxxopts::exceptions::exception& e)
    {
        std::cerr << "Caught exception while parsing arguments: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    if (result.count("help"))
    {
        std::cerr << options.help() << std::endl;
        return EXIT_SUCCESS;
    }

    if (result.count("flow"))
    {
        filename = result["flow"].as<std::string>();
    }

    if (result.count("log_level"))
    {
        spdlog::set_level(static_cast<spdlog::level::level_enum>(result["log_level"].as<int>()));
    }
#endif
    flow::ui::Editor app(filename);

    app.LoadFonts = [](ImGuiIO& io, flow::ui::Config& config) {
        HelloImGui::FontLoadingParams font_params;
        font_params.useFullGlyphRange                 = true;
        font_params.fontConfig.RasterizerDensity      = 4.f;
        font_params.reduceMemoryUsageIfFullGlyphRange = true;

        config.DefaultFont    = flow::ui::LoadFont("fonts/DroidSans.ttf", 18.f, font_params);
        config.NodeHeaderFont = flow::ui::LoadFont("fonts/DroidSans.ttf", 20.f, font_params);
        config.IconFont       = flow::ui::LoadFont("fonts/fontawesome-webfont.ttf", 18.f, font_params);

        io.FontDefault = config.DefaultFont;
    };

    app.SetupStyle = [](ImGuiStyle& imgui_style, flow::ui::Style& style) {
        imgui_style.CircleTessellationMaxError = 0.1f;
        imgui_style.CurveTessellationTol       = 0.1f;
        imgui_style.WindowBorderSize           = 5.f;
        imgui_style.FrameBorderSize            = 2.f;
        imgui_style.TabRounding                = 8.f;
        imgui_style.TabBarBorderSize           = 0.f;
        imgui_style.CellPadding                = ImVec2(7.f, 7.f);

        auto& imgui_colours = imgui_style.Colors;

        imgui_colours[ImGuiCol_WindowBg]           = ImColor(21, 21, 21);
        imgui_colours[ImGuiCol_PopupBg]            = ImColor(15, 15, 15, 175);
        imgui_colours[ImGuiCol_Border]             = ImColor(15, 15, 15);
        imgui_colours[ImGuiCol_PopupBg]            = imgui_colours[ImGuiCol_WindowBg];
        imgui_colours[ImGuiCol_FrameBg]            = ImColor(15, 15, 15);
        imgui_colours[ImGuiCol_MenuBarBg]          = ImColor(21, 21, 21);
        imgui_colours[ImGuiCol_TitleBg]            = ImColor(21, 21, 21);
        imgui_colours[ImGuiCol_TitleBgActive]      = imgui_colours[ImGuiCol_TitleBg];
        imgui_colours[ImGuiCol_Tab]                = ImColor(21, 21, 21);
        imgui_colours[ImGuiCol_TabUnfocused]       = ImColor(21, 21, 21);
        imgui_colours[ImGuiCol_TabHovered]         = ImColor(47, 47, 47);
        imgui_colours[ImGuiCol_TabActive]          = ImColor(3, 98, 195);
        imgui_colours[ImGuiCol_TabUnfocusedActive] = imgui_colours[ImGuiCol_TabActive];
        imgui_colours[ImGuiCol_Button]             = ImColor(32, 32, 32);
        imgui_colours[ImGuiCol_ButtonHovered]      = ImColor(3, 98, 195);
        imgui_colours[ImGuiCol_ButtonActive]       = ImColor(13, 39, 77);
        imgui_colours[ImGuiCol_ScrollbarBg]        = ImColor(21, 21, 21);
        imgui_colours[ImGuiCol_ScrollbarGrab]      = ImColor(86, 86, 86);
        imgui_colours[ImGuiCol_TableBorderLight]   = ImColor(21, 21, 21);
        imgui_colours[ImGuiCol_TableBorderStrong]  = ImColor(21, 21, 21);
        imgui_colours[ImGuiCol_TableRowBg]         = ImColor(36, 36, 36);
        imgui_colours[ImGuiCol_TableRowBgAlt]      = ImColor(36, 36, 36);
        imgui_colours[ImGuiCol_Header]             = ImColor(47, 47, 47);
        imgui_colours[ImGuiCol_HeaderHovered]      = ImColor(50, 50, 50);
        imgui_colours[ImGuiCol_CheckMark]          = ImColor(3, 98, 195);

        auto& colours = style.Colours.NodeEdtiorColours;

        colours[ed::StyleColor_Bg]                  = ImColor(38, 38, 38);
        colours[ed::StyleColor_Grid]                = ImColor(52, 52, 52);
        colours[ed::StyleColor_NodeBg]              = ImColor(15, 17, 15, 240);
        colours[ed::StyleColor_NodeBorder]          = ImColor(0, 0, 0);
        colours[ed::StyleColor_SelNodeBorder]       = ImColor(255, 255, 255);
        colours[ed::StyleColor_Flow]                = ImColor(32, 191, 85);
        colours[ed::StyleColor_FlowMarker]          = ImColor(32, 191, 85);
        colours[ed::StyleColor_HighlightLinkBorder] = ImColor(0, 188, 235);
        colours[ed::StyleColor_SelLinkBorder]       = ImColor(0, 188, 235);
    };

    try
    {
        using namespace flow::ui;

        // TODO(trigaux): Fix up imgui exporting to allow for this extending to work.
        // app.AddDockspace("PropertySpace", DefaultDockspaces::Main, 0.25f, DockspaceSplitDirection::Left);
        // app.AddDockspace("MiscSpace", DefaultDockspaces::Main, 0.25f, DockspaceSplitDirection::Down);

        // auto property_window = std::make_shared<PropertyWindow>(app.GetEnv());
        // app.OnActiveGraphChanged.Bind(flow::IndexableName{property_window->GetName()},
        //                               [=](const auto& g) { property_window->SetCurrentGraph(g); });
        // app.AddWindow(property_window, "PropertySpace");

        app.Run();
    }
    catch (const std::exception& e)
    {
        SPDLOG_CRITICAL("Exiting with error: {0}", e.what());
        return EXIT_FAILURE;
    }
    catch (...)
    {
        SPDLOG_CRITICAL("Exiting with unknown error");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
