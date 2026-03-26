# Gather source files from subdirectories
add_subdirectory(app)
add_subdirectory(gfx)
add_subdirectory(scene)
add_subdirectory(ui)
add_subdirectory(platform)

# Collect all sources and headers for the executable
set(SOURCES
    main.cpp
    ${APP_SOURCES}
    ${GFX_SOURCES}
    ${SCENE_SOURCES}
    ${UI_SOURCES}
    ${PLATFORM_SOURCES}
)

# Create executable
add_executable(3DProject ${SOURCES})

# Hide console in release builds on Windows
if (WIN32)
    set_target_properties(3DProject PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
    # Use WIN32 property but only apply SUBSYSTEM:WINDOWS linker flag for Release
    set_property(TARGET 3DProject APPEND PROPERTY LINK_OPTIONS
        $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
        $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
    )
    # Set entry point for Windows subsystem
    set_property(TARGET 3DProject APPEND PROPERTY LINK_OPTIONS
        $<$<CONFIG:Release>:/ENTRY:mainCRTStartup>
    )
endif()

target_include_directories(3DProject PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Copy shaders to build directory next to executable
add_custom_command(TARGET 3DProject POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:3DProject>/shaders
    COMMENT "Copying shaders to build directory"
)

# Link libs
target_link_libraries(3DProject PRIVATE
    glfw
    assimp::assimp
    glad::glad
    imgui::imgui
    glm::glm
)

# Windows: OpenGL is in opengl32
if (WIN32)
    target_link_libraries(3DProject PRIVATE opengl32)
endif()

target_compile_definitions(3DProject PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
