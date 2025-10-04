#include "models/AdministradorDeUsuarios.h" // Inclui a classe para gerenciar usuários em memória
#include "models/InterfaceDB.h"          // Inclui a nova classe para persistência em arquivo
#include <iostream>                      // Para operações de entrada/saída no console
#include "models/notificacao_observer.h"  // Inclui o cabeçalho do padrão Observer
=======
// C++/main.cpp
#include "GerenciadorTabelas/Gerenciador.h"
#include "commands/AtualizarUsuarioCommand.h"
#include <iostream>
#include <memory>
>>>>>>> 3bc5455b2e4f3e814f4d50746a5f0101b584f7fe

int main() {
    try {
        // --- 1. Configuração Inicial ---
        auto conexao = std::make_shared<ConexaoMock>();
        auto gerenciador = std::make_shared<GerenciadorTabelas>(conexao);
        auto facade = FacadeSingletonController::getInstance(gerenciador);

        std::cout << "--- Criando usuário inicial ---\n";
        Usuario alice("Alice Silva", "alice@email.com", 28);
        auto idAliceAny = gerenciador->criar(alice);
        int idAlice = std::any_cast<int>(idAliceAny);
        alice.setId(idAlice);

        std::cout << "Usuário original:\n";
        alice.mostrar();
        std::cout << std::string(50, '=') << std::endl;

        // --- 2. Executando o Comando de Atualização ---
        std::cout << "\n--- Preparando para atualizar usuário ---\n";
        Usuario aliceAtualizada = alice;
        aliceAtualizada.setNome("Alice Souza");
        aliceAtualizada.setIdade(29);
        
        auto comandoAtualizar = std::make_shared<AtualizarUsuarioCommand>(facade->getGerenciador(), aliceAtualizada);
        facade->executarComando(comandoAtualizar);

        std::cout << "\nUsuário após atualização:\n";
        auto usuarioBuscado = gerenciador->buscar<Usuario>("usuarios", idAlice);
        if(usuarioBuscado) usuarioBuscado->mostrar();
        std::cout << std::string(50, '=') << std::endl;

        // --- 3. Desfazendo a Ação ---
        std::cout << "\n--- Desfazendo a última ação ---\n";
        facade->desfazerUltimaAcao();

        std::cout << "\nUsuário após o 'undo':\n";
        usuarioBuscado = gerenciador->buscar<Usuario>("usuarios", idAlice);
        if(usuarioBuscado) usuarioBuscado->mostrar();
        std::cout << std::string(50, '=') << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "❌ Erro: " << e.what() << std::endl;
        return 1;
    }
<<<<<<< HEAD

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
=======
    return 0;
}
>>>>>>> 3bc5455b2e4f3e814f4d50746a5f0101b584f7fe
