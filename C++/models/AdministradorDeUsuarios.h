#ifndef ADMINISTRADORDEUSUARIOS_H
#define ADMINISTRADORDEUSUARIOS_H

#include "Usuario.h"
#include "../views/UserView.h"
#include <vector>
#include <string>

/**
 * @brief Classe AdministradorDeUsuarios gerencia uma coleção de objetos Usuario.
 * Herda de Usuario para representar um administrador que também é um usuário.
 */
class AdministradorDeUsuarios : public Usuario {
private:
    UserView view; // Objeto para interações com a interface do usuário
    std::vector<Usuario> usuarios; // Coleção de usuários gerenciados em memória

public:
    /**
     * @brief Construtor da classe AdministradorDeUsuarios.
     * @param nome O nome do administrador.
     * @param email O email do administrador.
     * @param login O login do administrador.
     */
    AdministradorDeUsuarios(const std::string& nome, const std::string& email, const std::string& login);

    /**
     * @brief Mostra a interface do usuário para o administrador.
     */
    void mostrarInterfaceUsuario() const;

    /**
     * @brief Adiciona um novo usuário à coleção gerenciada.
     * @param usuario O objeto Usuario a ser adicionado.
     */
    void adicionarUsuario(const Usuario& usuario);

    /**
     * @brief Lista todos os usuários na coleção gerenciada.
     */
    void listarUsuarios() const;

    /**
     * @brief Edita o login de um usuário existente na coleção.
     * @param loginAtual O login atual do usuário a ser editado.
     * @param novoLogin O novo login para o usuário.
     */
    void editarUsuario(const std::string& loginAtual, const std::string& novoLogin);

    /**
     * @brief Exclui um usuário da coleção com base no login.
     * @param login O login do usuário a ser excluído.
     */
    void excluirUsuario(const std::string& login);
};

#endif // ADMINISTRADORDEUSUARIOS_H
