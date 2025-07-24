#include "Usuario.h"
#include <iostream>

Usuario::Usuario(const std::string& nome, const std::string& email, const std::string& login)
    : nome(nome), email(email), login(login) {}

void Usuario::exibirDados() const {
    std::cout << "Nome: " << nome << ", Email: " << email << ", Login: " << login << std::endl;
}

std::string Usuario::getLogin() const {
    return login;
}

void Usuario::setLogin(const std::string& novoLogin) {
    login = novoLogin;
}