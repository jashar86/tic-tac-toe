# Presentation Layer - UI rendering and input handling
# Contains: UI implementations (Terminal, GUI, etc.)

add_library(presentation
    src/Console.cpp
)

target_include_directories(presentation
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Presentation layer depends on Application layer (which depends on Core)
target_link_libraries(presentation
    PUBLIC application
)

# ============================================================================
# Presentation Tests
# ============================================================================
# add_executable(presentation_tests
#     tests/ConsoleViewTest.cpp
# )

# target_link_libraries(presentation_tests
#     PRIVATE
#         presentation
#         GTest::gtest_main
# )

# gtest_discover_tests(presentation_tests)
