#ifndef INTERFACEDB_H
#define INTERFACEDB_H

#include "Usuario.h" // Inclui a definição da classe Usuario
#include <vector>
#include <string>

/**
 * @brief Classe InterfaceDB para gerenciar a persistência de dados.
 * Atualmente implementa persistência em arquivo de texto.
 */
class InterfaceDB {
public:
    /**
     * @brief Salva um objeto Usuario no arquivo de banco de dados.
     * @param usuario O objeto Usuario a ser salvo.
     */
    void salvarUsuario(const Usuario& usuario);

    /**
     * @brief Carrega todos os objetos Usuario do arquivo de banco de dados.
     * @return Um vetor de objetos Usuario carregados.
     */
    std::vector<Usuario> carregarUsuarios();

    /**
     * @brief Exclui um usuário do arquivo de banco de dados com base no login.
     * @param login O login do usuário a ser excluído.
     */
    void excluirUsuarioPorLogin(const std::string& login);
};

#endif // INTERFACEDB_H
