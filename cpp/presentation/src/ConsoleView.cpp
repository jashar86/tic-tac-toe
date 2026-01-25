#include "presentation/ConsoleView.h"
#include <iostream>

namespace tictactoe::presentation {

ConsoleView::ConsoleView() {
    // TODO: Initialize console view
}

void ConsoleView::showWelcome() const {
    std::cout << "Welcome to Tic-Tac-Toe!" << std::endl;
}

bool ConsoleView::isInitialized() const {
    // Skeleton implementation
    return true;
}

} // namespace tictactoe::presentation
