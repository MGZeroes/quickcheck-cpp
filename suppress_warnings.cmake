# suppress_warnings.cmake

# Function to suppress warnings for a target
function(suppress_warnings target_name)
    if (MSVC)
        target_compile_options(${target_name} PUBLIC /W0)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${target_name} PUBLIC -Wno-deprecated-declarations)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
        target_compile_options(${target_name} PUBLIC -w)
    endif()
endfunction()
