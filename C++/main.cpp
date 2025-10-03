// C++/main.cpp
#include "GerenciadorTabelas/Gerenciador.h"
#include "commands/AtualizarUsuarioCommand.h"
#include <iostream>
#include <memory>

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
    return 0;
}