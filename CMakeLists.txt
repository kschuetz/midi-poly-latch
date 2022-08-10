cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME "midi_poly_latch")

set(FORMATS AU VST3 AUv3)

file(READ VERSION CURRENT_VERSION)

project(${PROJECT_NAME} VERSION ${CURRENT_VERSION})

set_property(GLOBAL PROPERTY USE_FOLDERS YES)

option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Show all module sources in IDE projects" ON)

add_subdirectory(libs/JUCE)

juce_add_plugin("${PROJECT_NAME}"
        # VERSION ...                               # Set this if the plugin version is different to the project version
        # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
        # ICON_SMALL ...
        COMPANY_NAME KES
        #        BUNDLE_ID com.pamplejuce.pamplejuce
        IS_SYNTH FALSE              # Is this a synth or an effect?
        NEEDS_MIDI_INPUT TRUE               # Does the plugin need midi input?
        NEEDS_MIDI_OUTPUT TRUE              # Does the plugin need midi output?
        IS_MIDI_EFFECT TRUE                 # Is this plugin a MIDI effect?
        EDITOR_WANTS_KEYBOARD_FOCUS TRUE    # Does the editor need keyboard focus?
        COPY_PLUGIN_AFTER_BUILD TRUE # On MacOS, plugin will be copied to /Users/you/Library/Audio/Plug-Ins/
        PLUGIN_MANUFACTURER_CODE Kes_               # This has to be one uppercase, rest lower for AU formats
        PLUGIN_CODE Mplc                            # A unique four-character plugin id with at least one upper-case character
        FORMATS "${FORMATS}"
        PRODUCT_NAME "${PROJECT_NAME}")        # The name of the final executable, which can differ from the target name

target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_20)

set(SourceFiles
        src/PluginEditor.h
        src/PluginProcessor.h
        src/core/CommonTypes.h
        src/core/Consts.h
        src/core/Engine.h
        src/core/Parameters.h
        src/core/NoteIndex.h
        src/core/NoteState.h
        src/core/NoteList.h
        src/core/Rng.h
        src/core/SpecialKeys.h
        src/core/ChannelState.h
        src/core/Stealing.h
        src/PluginEditor.cpp
        src/PluginProcessor.cpp
        src/core/CommonTypes.cpp
        src/core/Engine.cpp
        src/core/NoteIndex.cpp
        src/core/NoteState.cpp
        src/core/NoteList.cpp
        src/core/Parameters.cpp
        src/core/Rng.cpp
        src/core/ChannelState.cpp
        src/core/Stealing.cpp
        )
target_sources("${PROJECT_NAME}" PRIVATE ${SourceFiles})

set_target_properties("${PROJECT_NAME}" PROPERTIES FOLDER "")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "" FILES ${SourceFiles})

set(JUCE_DEPENDENCIES
        juce::juce_audio_utils)

target_compile_definitions("${PROJECT_NAME}"
        PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries("${PROJECT_NAME}"
        PRIVATE
        #        Assets
        ${JUCE_DEPENDENCIES}
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

# **************** Testing *****************

enable_testing()

file(GLOB_RECURSE TestFiles CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/tests PREFIX "" FILES ${TestFiles})

# Use Catch2 v3 on the devel branch
Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_PROGRESS TRUE
        GIT_SHALLOW TRUE
        GIT_TAG origin/devel)
FetchContent_MakeAvailable(Catch2) # find_package equivalent

# Set up the test executable
add_executable(tests ${TestFiles})
target_compile_features(tests PRIVATE cxx_std_20)

# Our test executable also wants to know about our plugin code...
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain "${PROJECT_NAME}" ${JUCE_DEPENDENCIES})

# Make an Xcode Scheme for the test executable so we can run tests in the IDE
set_target_properties(tests PROPERTIES XCODE_GENERATE_SCHEME ON)

# Organize the test source in the tests/ folder in the IDE
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/tests PREFIX "" FILES ${TestFiles})

# Load and use the .cmake file provided by Catch2
# https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md
# We have to manually provide the source directory here for now
include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)
catch_discover_tests(tests)

# Color our warnings and errors
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-fcolor-diagnostics)
endif ()
