#include "models/AdministradorDeUsuarios.h" // Inclui a classe para gerenciar usuários em memória
#include "models/InterfaceDB.h"          // Inclui a nova classe para persistência em arquivo
#include <iostream>                      // Para operações de entrada/saída no console

int main() {
    // --- Criação e demonstração do AdministradorDeUsuarios (em memória) ---
    // Cria uma instância do AdministradorDeUsuarios.
    AdministradorDeUsuarios admin("Admin", "admin@email.com", "admin123");

    std::cout << "--- Operações com AdministradorDeUsuarios (em memória) ---" << std::endl;

    // Adiciona usuários ao sistema em memória
    admin.adicionarUsuario(Usuario("Joao", "joao@email.com", "joao123"));
    admin.adicionarUsuario(Usuario("Maria", "maria@email.com", "maria456"));

    std::cout << "\nUsuários cadastrados em memória:" << std::endl;
    admin.listarUsuarios(); // Lista os usuários em memória

    std::cout << "\nEditando login do usuário 'joao123' para 'joao_novo' em memória..." << std::endl;
    admin.editarUsuario("joao123", "joao_novo"); // Edita um usuário em memória

    std::cout << "\nUsuários em memória após edição:" << std::endl;
    admin.listarUsuarios(); // Lista os usuários em memória após edição

    std::cout << "\nExcluindo usuário 'maria456' da memória..." << std::endl;
    admin.excluirUsuario("maria456"); // Exclui um usuário da memória

    std::cout << "\nUsuários em memória após exclusão:" << std::endl;
    admin.listarUsuarios(); // Lista os usuários em memória após exclusão

    // --- Demonstração da persistência em arquivo com InterfaceDB ---

    // Cria uma instância da InterfaceDB para gerenciar a persistência em arquivo
    InterfaceDB db;

    std::cout << "\n--- Operações com persistência em arquivo (InterfaceDB) ---" << std::endl;

    // Salva novos usuários no arquivo
    Usuario lucas("Lucas", "lucas@email.com", "lucas123");
    db.salvarUsuario(lucas);

    Usuario ana("Ana", "ana@email.com", "ana789");
    db.salvarUsuario(ana);

    std::cout << "\nUsuários salvos no arquivo:" << std::endl;
    // Carrega e exibe todos os usuários do arquivo
    auto usuariosDoArquivo = db.carregarUsuarios();
    if (usuariosDoArquivo.empty()) {
        std::cout << "Nenhum usuário encontrado no arquivo." << std::endl;
    } else {
        for (const auto& u : usuariosDoArquivo) {
            u.exibirDados();
        }
    }

    std::cout << "\nExcluindo usuário 'lucas123' do arquivo..." << std::endl;
    // Exclui um usuário do arquivo
    db.excluirUsuarioPorLogin("lucas123");

    std::cout << "\nUsuários no arquivo após exclusão:" << std::endl;
    // Carrega e exibe os usuários restantes no arquivo
    usuariosDoArquivo = db.carregarUsuarios();
    if (usuariosDoArquivo.empty()) {
        std::cout << "Nenhum usuário encontrado no arquivo." << std::endl;
    } else {
        for (const auto& u : usuariosDoArquivo) {
            u.exibirDados();
        }
    }

    return 0; // Retorna 0 indicando sucesso na execução
}
