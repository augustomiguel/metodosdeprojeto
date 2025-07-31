#include "models/AdministradorDeUsuarios.h"
#include "models/MongoDBInterface.h"
#include <iostream>

int main() {
    AdministradorDeUsuarios admin("Admin", "admin@email.com", "admin123");

    admin.adicionarUsuario(Usuario("Joao", "joao@email.com", "joao123"));
    admin.adicionarUsuario(Usuario("Maria", "maria@email.com", "maria456"));

    std::cout << "Usuários cadastrados:" << std::endl;
    admin.listarUsuarios();

    std::cout << "\nEditando login do usuário 'joao123'..." << std::endl;
    admin.editarUsuario("joao123", "joao_novo");

    std::cout << "\nUsuários após edição:" << std::endl;
    admin.listarUsuarios();

    std::cout << "\nExcluindo usuário 'maria456'..." << std::endl;
    admin.excluirUsuario("maria456");

    std::cout << "\nUsuários após exclusão:" << std::endl;
    admin.listarUsuarios();

    MongoDBInterface mongo;
    mongo.salvarUsuario(Usuario("Lucas", "lucas@email.com", "lucas123"));

    std::cout << "\nUsuários salvos no MongoDB:" << std::endl;
    auto usuariosMongo = mongo.carregarUsuarios();
    for (const auto& u : usuariosMongo) {
        u.exibirDados();
    }

    mongo.excluirUsuarioPorLogin("lucas123");

    return 0;
}