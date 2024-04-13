# CMakeList.txt : CMake project for Project3app, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (GUI "GUI.cpp" "GUI.h")

target_link_libraries(GUI  "fltk" "fltk_gl" "fltk_images" "fltk_jpeg" "fltk_png")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GUI PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
