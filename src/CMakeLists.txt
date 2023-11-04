find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

FLEX_TARGET(tabulateScanner lex.l lex.yy.cc)
BISON_TARGET(tabulateParser parser.y parser.tab.cc DEFINES_FILE ../include/parser.tab.hh)
add_flex_bison_dependency(tabulateScanner tabulateParser)

# specify the C++ standard
add_library(tabulate_compiler_flags INTERFACE)
target_compile_features(tabulate_compiler_flags INTERFACE cxx_std_17)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(tabulate_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-O2>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3;-O2>>"
)

file(GLOB 
  link_files
  *.cc
)

# add the executable
add_executable(tabulate main.cc ${link_files})

target_link_libraries(tabulate PUBLIC tabulate_compiler_flags)

# add the binary tree to the search path for include files
target_include_directories(tabulate PUBLIC
                           ${CMAKE_SOURCE_DIR}/include
                           )

# Install tabulate in the bin directory
install(TARGETS tabulate DESTINATION "${PROJECT_BINARY_DIR}")
