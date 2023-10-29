set(PROJECT_NAME PFL)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "PFL.h"
    "winproof88.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "PFL.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

#use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
# use maybe this instead of above?
#set_target_properties(${PROJECT_NAME} PROPERTIES VS_PROPERTY_SHEETS "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE PFL)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE             "TRUE"
)
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Debug>:
        MultiThreadedDebug
    >
    $<$<CONFIG:DebugPR00FPS>:
        MultiThreadedDebug
    >
    $<$<CONFIG:DebugTest_PRooFPSdd>:
        MultiThreadedDebug
    >
    $<$<CONFIG:EV2008P03Debug>:
        MultiThreadedDebug
    >
    $<$<CONFIG:EV2008P03Release>:
        MultiThreaded
    >
    $<$<CONFIG:Release>:
        MultiThreaded
    >
    $<$<CONFIG:ReleasePR00FPS>:
        MultiThreaded
    >
    $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:DebugPR00FPS>,$<CONFIG:DebugTest_PRooFPSdd>,$<CONFIG:EV2008P03Debug>,$<CONFIG:EV2008P03Release>,$<CONFIG:Release>,$<CONFIG:ReleasePR00FPS>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:DebugPR00FPS>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:DebugTest_PRooFPSdd>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:EV2008P03Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:EV2008P03Release>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:ReleasePR00FPS>:"
        "NDEBUG"
    ">"
    "NOMINMAX;"
    "WIN32;"
    "_LIB;"
    "_MBCS"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od;
            /Gy
        >
        $<$<CONFIG:DebugPR00FPS>:
            /Od;
            /Gy;
            /Zc:wchar_t
        >
        $<$<CONFIG:DebugTest_PRooFPSdd>:
            /Od;
            /Gy
        >
        $<$<CONFIG:EV2008P03Debug>:
            /Od;
            /Gy;
            /Zc:wchar_t
        >
        $<$<CONFIG:EV2008P03Release>:
            /GL-;
            /Gm-;
            /O2;
            /Oi;
            /Gy-
        >
        $<$<CONFIG:Release>:
            /GL-;
            /O2;
            /Oi;
            /Gy-
        >
        $<$<CONFIG:ReleasePR00FPS>:
            /GL-;
            /Gm-;
            /O2;
            /Oi;
            /Gy-
        >
        /W4;
        /WX;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        /GR;
        /Zc:__cplusplus;
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /Y-
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        /SUBSYSTEM:WINDOWS
    )
endif()

