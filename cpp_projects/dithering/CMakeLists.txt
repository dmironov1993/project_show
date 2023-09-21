find_package(PNG)

if (PNG_FOUND)
    include_directories(${PNG_INCLUDE_DIRS})
    add_catch(test_fast_dither test.cpp)
    target_link_libraries(test_fast_dither ${PNG_LIBRARY})
endif()
