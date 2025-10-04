#include "AdministradorDeUsuarios.h"
#include <iostream>

/**
 * @brief Construtor da classe AdministradorDeUsuarios.
 * Chama o construtor da classe base Usuario.
 * @param nome O nome do administrador.
 * @param email O email do administrador.
 * @param login O login do administrador.
 */
AdministradorDeUsuarios::AdministradorDeUsuarios(const std::string& nome, const std::string& email, const std::string& login)
    : Usuario(nome, email, login) {}

/**
 * @brief Mostra a interface do usuário para o administrador.
 */
void AdministradorDeUsuarios::mostrarInterfaceUsuario() const {
    view.mostrar(getLogin());
}

/**
 * @brief Adiciona um novo usuário à coleção gerenciada.
 * @param usuario O objeto Usuario a ser adicionado.
 */
void AdministradorDeUsuarios::adicionarUsuario(const Usuario& usuario) {
    usuarios.push_back(usuario);
    std::cout << "Usuário '" << usuario.getLogin() << "' adicionado com sucesso em memória." << std::endl;
}

/**
 * @brief Lista todos os usuários na coleção gerenciada.
 */
void AdministradorDeUsuarios::listarUsuarios() const {
    if (usuarios.empty()) {
        std::cout << "Nenhum usuário cadastrado em memória." << std::endl;
        return;
    }
    for (const auto& u : usuarios) {
        u.exibirDados();
    }
}

/**
 * @brief Edita o login de um usuário existente na coleção.
 * @param loginAtual O login atual do usuário a ser editado.
 * @param novoLogin O novo login para o usuário.
 */
void AdministradorDeUsuarios::editarUsuario(const std::string& loginAtual, const std::string& novoLogin) {
    for (auto& u : usuarios) {
        if (u.getLogin() == loginAtual) {
            u.setLogin(novoLogin);
            std::cout << "Login do usuário '" << loginAtual << "' atualizado para: '" << novoLogin << "' em memória." << std::endl;
            return;
        }
    }
    std::cout << "Usuário com login '" << loginAtual << "' não encontrado em memória para edição." << std::endl;
}

/**
 * @brief Exclui um usuário da coleção com base no login.
 * @param login O login do usuário a ser excluído.
 */
void AdministradorDeUsuarios::excluirUsuario(const std::string& login) {
    for (auto it = usuarios.begin(); it != usuarios.end(); ++it) {
        if (it->getLogin() == login) {
            usuarios.erase(it);
            std::cout << "Usuário '" << login << "' excluído com sucesso da memória." << std::endl;
            return;
        }
    }
    std::cout << "Usuário com login '" << login << "' não encontrado em memória para exclusão." << std::endl;
}
