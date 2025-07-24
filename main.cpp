#include "models/AdministradorDeUsuarios.h"
#include <iostream>

int main() {
    AdministradorDeUsuarios admin("Ana", "ana@email.com", "admin123");

    admin.exibirDados();
    admin.mostrarInterfaceUsuario();

    std::cout << "Alterando login...\n";
    admin.setLogin("novo_login456");
    admin.mostrarInterfaceUsuario();

    return 0;
}