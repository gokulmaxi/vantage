cmake_minimum_required (VERSION 3.11)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG 21d746e8586a59a39ed5c73317812f17264e68d5
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------------------------

project(vantage
  LANGUAGES CXX
  VERSION 1.0.0
)

add_executable(vantage src/main.cpp)
target_include_directories(vantage PRIVATE src)

target_link_libraries(vantage
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component # Not needed for this example.
)

# C++17 is used. We requires fold expressions at least.
set_target_properties(vantage PROPERTIES CXX_STANDARD 17)

install(TARGETS vantage RUNTIME DESTINATION "bin")
set(CPACK_GENERATOR "DEB;External;RPM;STGZ;TBZ2;TGZ;TXZ;TZ;TZST;ZIP")
set(CPACK_DEBIAN_PACKAGE_DEPENDS " ")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE_URL "https://github.com/gokulmaxi/vantage")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Gokul")
set(CPACK_DEBIAN_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Port of lenova vantage software but in Terminal")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/gokulmaxi/vantage")
set(CPACK_PACKAGE_NAME "vantage")
set(CPACK_PACKAGE_VENDOR "Gokul")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
INCLUDE(CPack)
