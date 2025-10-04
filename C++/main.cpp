#include <iostream>
#include <memory>

// --- INCLUDES CORRIGIDOS ---
// Inclui o ficheiro principal que define a Fachada, Gerenciador, Usuario, etc.
#include "GerenciadorTabelas/Gerenciador.h" 

// Inclui a definição do comando para atualizar o usuário
#include "commands/AtualizarUsuarioCommand.h" 

// Inclui a definição do comando para enviar notificações
#include "commands/EnviarNotificacaoCommand.h" 

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

        // ====================================================================
        // Teste do Padrão Observer (Notificações)
        // ====================================================================
        std::cout << "\n--- Teste de Observer e Command ---\n";

        // Criando os alunos (observers)
        auto appAluno1 = std::make_shared<AlunoApp>("Ana");
        auto appAluno2 = std::make_shared<AlunoApp>("Beto");

        // Registrando os alunos na CentralDeNotificacoes através da fachada
        facade->getCentralNotificacoes()->registrar(appAluno1);
        facade->getCentralNotificacoes()->registrar(appAluno2);

        // Criando o comando para enviar a notificação
        auto comandoAviso = std::make_shared<EnviarNotificacaoCommand>(
            facade->getCentralNotificacoes(),
            "O circular está atrasado 15 minutos."
        );

        // Pedindo para a fachada executar o comando de notificação
        facade->executarComando(comandoAviso);

    } catch (const std::exception& e) {
        std::cerr << "❌ Erro: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\n--- Aplicação finalizada com sucesso! ---\n";
    return 0;
}