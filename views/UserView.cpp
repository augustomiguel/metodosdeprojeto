#include "UserView.h"
#include <iostream>

void UserView::mostrar(const std::string& login) const {
    std::cout << "Interface do usuário: login atual é '" << login << "'" << std::endl;
}