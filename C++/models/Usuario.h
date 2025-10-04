#ifndef USUARIO_H
#define USUARIO_H

#include <string>

/**
 * @brief Classe Usuario representa um usuário no sistema.
 * Contém informações como nome, email e login.
 */
class Usuario {
protected:
    std::string nome;
    std::string email;
    std::string login;

public:
    /**
     * @brief Construtor da classe Usuario.
     * @param nome O nome do usuário.
     * @param email O email do usuário.
     * @param login O login do usuário.
     */
    Usuario(const std::string& nome, const std::string& email, const std::string& login);

    /**
     * @brief Exibe os dados do usuário no console.
     */
    void exibirDados() const;

    /**
     * @brief Retorna o login do usuário.
     * @return O login do usuário.
     */
    std::string getLogin() const;

    /**
     * @brief Define um novo login para o usuário.
     * @param novoLogin O novo login a ser definido.
     */
    void setLogin(const std::string& novoLogin);

    /**
     * @brief Retorna o nome do usuário.
     * @return O nome do usuário.
     */
    std::string getNome() const;

    /**
     * @brief Retorna o email do usuário.
     * @return O email do usuário.
     */
    std::string getEmail() const;
};

#endif // USUARIO_H
