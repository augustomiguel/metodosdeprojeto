#include "Usuario.h"
#include <iostream>

/**
 * @brief Construtor da classe Usuario.
 * @param nome O nome do usuário.
 * @param email O email do usuário.
 * @param login O login do usuário.
 */
Usuario::Usuario(const std::string& nome, const std::string& email, const std::string& login)
    : nome(nome), email(email), login(login) {}

/**
 * @brief Exibe os dados do usuário no console.
 */
void Usuario::exibirDados() const {
    std::cout << "Nome: " << nome << ", Email: " << email << ", Login: " << login << std::endl;
}

/**
 * @brief Retorna o login do usuário.
 * @return O login do usuário.
 */
std::string Usuario::getLogin() const {
    return login;
}

/**
 * @brief Define um novo login para o usuário.
 * @param novoLogin O novo login a ser definido.
 */
void Usuario::setLogin(const std::string& novoLogin) {
    login = novoLogin;
}

/**
 * @brief Retorna o nome do usuário.
 * @return O nome do usuário.
 */
std::string Usuario::getNome() const {
    return nome;
}

/**
 * @brief Retorna o email do usuário.
 * @return O email do usuário.
 */
std::string Usuario::getEmail() const {
    return email;
}
