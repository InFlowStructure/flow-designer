add_subdirectory(cxxopts)

if (NOT flow-designer_USE_EXTERNAL_FLOW_UI)
    add_subdirectory(flow-ui)
endif()