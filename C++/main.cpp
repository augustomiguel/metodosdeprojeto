#include "models/AdministradorDeUsuarios.h" // Inclui a classe para gerenciar usuários em memória
#include "models/InterfaceDB.h"          // Inclui a nova classe para persistência em arquivo
#include <iostream>                      // Para operações de entrada/saída no console
#include "models/notificacao_observer.h"  // Inclui o cabeçalho do padrão Observer

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



    // ====================================================================
    // Exemplo do Padrão Observer - Central de Notificacoes
    // ====================================================================
    std::cout << "\nTeste do Padrao Observer:" << std::endl;

    // 1. Criar o Subject (a central de notificações)
    auto central = std::make_shared<CentralDeNotificacoes>();

    // 2. Criar os Observers (os apps dos alunos)
    auto appAluno1 = std::make_shared<AlunoApp>("Ana");
    auto appAluno2 = std::make_shared<AlunoApp>("Beto");
    auto appAluno3 = std::make_shared<AlunoApp>("Carla");

    // 3. Registrar os observers na central
    std::cout << "Alunos Ana, Beto e Carla estao usando o app." << std::endl;
    central->registrar(appAluno1);
    central->registrar(appAluno2);
    central->registrar(appAluno3);

    // 4. Simular o motorista enviando uma mensagem
    std::cout << "\n>> Motorista envia uma nova mensagem:" << std::endl;
    central->enviarNovaMensagem("Atraso de 15 minutos na rota Sede - CI/CTDR.");

    // 5. Um observer (Beto) deixa de escutar as notificações
    std::cout << "\nAluno Beto fechou o app." << std::endl;
    central->remover(appAluno2);

    // 6. Simular o motorista enviando outra mensagem
    std::cout << "\n>> Motorista envia outra mensagem:" << std::endl;
    central->enviarNovaMensagem("Onibus quebrado. Aguardando substituicao.");

    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "Aplicacao finalizada." << std::endl;


    return 0; // Retorna 0 indicando sucesso na execução
}
