#ifndef ADMINISTRADORDEUSUARIOS_H
#define ADMINISTRADORDEUSUARIOS_H

#include "Usuario.h"
#include "../views/UserView.h"

class AdministradorDeUsuarios : public Usuario {
private:
    UserView view;

public:
    AdministradorDeUsuarios(const std::string& nome, const std::string& email, const std::string& login);

    void mostrarInterfaceUsuario() const;
};

#endif