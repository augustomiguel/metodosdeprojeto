#ifndef ADMINISTRADORDEUSUARIOS_H
#define ADMINISTRADORDEUSUARIOS_H

#include "Usuario.h"
#include "../views/UserView.h"
#include <vector>
#include <string>

class AdministradorDeUsuarios : public Usuario {
private:
    UserView view;
    std::vector<Usuario> usuarios;

public:
    AdministradorDeUsuarios(const std::string& nome, const std::string& email, const std::string& login);

    void mostrarInterfaceUsuario() const;
    void adicionarUsuario(const Usuario& usuario);
    void listarUsuarios() const;
    void editarUsuario(const std::string& login, const std::string& novoLogin);
    void excluirUsuario(const std::string& login);
};

#endif