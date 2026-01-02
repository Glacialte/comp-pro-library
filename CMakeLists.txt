cmake_minimum_required(VERSION 3.20)
project(comp_pro_library LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ライブラリ本体
add_library(comp_pro_library INTERFACE)
target_include_directories(comp_pro_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/data-structure)
target_include_directories(comp_pro_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/algorithm)

# ===== google tests =====
include(CTest)
if(BUILD_TESTING)
  include(FetchContent)

  # Windows での CRT 設定差異が問題になるのを防ぐらしい
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
  )
  FetchContent_MakeAvailable(googletest)

  add_subdirectory(tests)
endif()
