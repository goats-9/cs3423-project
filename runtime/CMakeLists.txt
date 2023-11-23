# specify the C++ standard
add_library(runtime_compiler_flags INTERFACE)
target_compile_features(runtime_compiler_flags INTERFACE cxx_std_17)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(
  runtime_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-O2;-g>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3;-O2>>"
)

set(src_files any.cc helper.cc inbuilt.cc types.cc state.cc)

include_directories(
  ${CMAKE_BINARY_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(runtime ${src_files})

target_link_libraries(
  runtime 
  PUBLIC runtime_compiler_flags
)

# Copy location.hh to include subdirectory
file(COPY_FILE ${CMAKE_BINARY_DIR}/src/location.hh include/location.hh)

install(
  TARGETS runtime
  DESTINATION "${PROJECT_BINARY_DIR}"
)