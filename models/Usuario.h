#ifndef USUARIO_H
#define USUARIO_H

#include <string>

class Usuario {
protected:
    std::string nome;
    std::string email;
    std::string login;

public:
    Usuario(const std::string& nome, const std::string& email, const std::string& login);

    void exibirDados() const;
    std::string getLogin() const;
    void setLogin(const std::string& novoLogin);
};

#endif