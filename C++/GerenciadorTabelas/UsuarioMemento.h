#pragma once
#include <string>

/**
 * @brief Memento: Armazena o estado interno de um objeto Usuario.
 * O acesso aos seus membros é restrito à classe Usuario (friend).
 */
class UsuarioMemento {
private:
    // Permite que a classe Usuario acesse os membros privados deste Memento.
    friend class Usuario;

    // Estado a ser salvo
    std::string nome;
    std::string email;
    int idade;

    // Construtor privado: somente o Usuario pode criar um memento.
    UsuarioMemento(const std::string& n, const std::string& e, int i)
        : nome(n), email(e), idade(i) {}
};