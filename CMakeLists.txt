cmake_minimum_required(VERSION 3.10)

project(FlowCode VERSION 1.0.1  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------

option(flow-designer_USE_EXTERNAL_FLOW_UI "Use an external Flow UI library" OFF)

if (flow-designer_USE_EXTERNAL_FLOW_UI)
  find_package(nlohmann_json REQUIRED)
  find_package(flow-core REQUIRED)
  find_package(flow-ui REQUIRED)
endif()

add_subdirectory(third_party)

# -----------------------------------------------------------------------------
# Executable
# -----------------------------------------------------------------------------

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
endif()

hello_imgui_add_app(${PROJECT_NAME}
  src/main.cpp

  src/PropertyWindow.cpp

  ASSETS_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/assets/
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC
  flow-ui::flow-ui
  cxxopts
  imgui_node_editor
)

if(MSVC)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
  )
endif()
