#include "AdministradorDeUsuarios.h"
#include <iostream>

AdministradorDeUsuarios::AdministradorDeUsuarios(const std::string& nome, const std::string& email, const std::string& login)
    : Usuario(nome, email, login) {}

void AdministradorDeUsuarios::mostrarInterfaceUsuario() const {
    view.mostrar(getLogin());
}

void AdministradorDeUsuarios::adicionarUsuario(const Usuario& usuario) {
    usuarios.push_back(usuario);
}

void AdministradorDeUsuarios::listarUsuarios() const {
    for (const auto& u : usuarios) {
        u.exibirDados();
    }
}

void AdministradorDeUsuarios::editarUsuario(const std::string& login, const std::string& novoLogin) {
    for (auto& u : usuarios) {
        if (u.getLogin() == login) {
            u.setLogin(novoLogin);
            std::cout << "Login atualizado para: " << novoLogin << std::endl;
            return;
        }
    }
    std::cout << "Usuário não encontrado." << std::endl;
}

void AdministradorDeUsuarios::excluirUsuario(const std::string& login) {
    for (auto it = usuarios.begin(); it != usuarios.end(); ++it) {
        if (it->getLogin() == login) {
            usuarios.erase(it);
            std::cout << "Usuário excluído com sucesso." << std::endl;
            return;
        }
    }
    std::cout << "Usuário não encontrado." << std::endl;
}