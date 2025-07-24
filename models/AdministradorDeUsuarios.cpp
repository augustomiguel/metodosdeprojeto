#include "AdministradorDeUsuarios.h"

AdministradorDeUsuarios::AdministradorDeUsuarios(const std::string& nome, const std::string& email, const std::string& login)
    : Usuario(nome, email, login) {}

void AdministradorDeUsuarios::mostrarInterfaceUsuario() const {
    view.mostrar(getLogin());
}