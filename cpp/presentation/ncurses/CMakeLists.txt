# Presentation Layer - ncurses UI implementation
# Contains: ncurses-based terminal UI for tic-tac-toe

find_package(Curses REQUIRED)

add_library(presentation_ncurses
    src/Ncurses.cpp
)

target_include_directories(presentation_ncurses
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CURSES_INCLUDE_DIRS}
)

target_link_libraries(presentation_ncurses
    PUBLIC application
    PRIVATE ${CURSES_LIBRARIES}
)
