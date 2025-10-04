#include <iostream>
#include <memory>
#include "GerenciadorTabelas/Gerenciador.h"
#include "commands/EnviarNotificacaoCommand.h"
#include "models/ProxyCentralNotificacoes.h" // <-- NOVO INCLUDE DO PROXY

int main() {
    try {
        // --- 1. Configuração Inicial ---
        auto conexao = std::make_shared<ConexaoMock>();
        auto gerenciador = std::make_shared<GerenciadorTabelas>(conexao);
        auto facade = FacadeSingletonController::getInstance(gerenciador);

        // Acessa a central de notificações REAL através da fachada
        auto centralReal = facade->getCentralNotificacoes();

        // --- 2. Criando Usuários para o Teste ---
        Usuario motorista("Carlos", "carlos@circular.com", 45, "motorista");
        Usuario aluno("Ana", "ana@aluno.com", 20, "aluno");

        // Criando os apps dos alunos (Observers)
        auto appAna = std::make_shared<AlunoApp>("Ana");
        auto appBeto = std::make_shared<AlunoApp>("Beto");
        centralReal->registrar(appAna);
        centralReal->registrar(appBeto);
        
        std::cout << std::string(50, '=') << std::endl;

        // --- 3. TESTE 1: Motorista (deve ter sucesso) ---
        std::cout << "\n--- Tentativa de envio pelo MOTORISTA ---\n";
        // Criamos o Proxy, dando a ele acesso à central real e identificando o usuário como 'motorista'
        auto proxyMotorista = std::make_shared<ProxyCentralNotificacoes>(centralReal, motorista);
        
        // O comando agora recebe o PROXY, não a central real
        auto comandoAtraso = std::make_shared<EnviarNotificacaoCommand>(proxyMotorista, "O circular está 10 minutos atrasado.");
        facade->executarComando(comandoAtraso);

        std::cout << std::string(50, '=') << std::endl;

        // --- 4. TESTE 2: Aluno (deve falhar) ---
        std::cout << "\n--- Tentativa de envio pelo ALUNO ---\n";
        // Criamos um NOVO Proxy, agora identificando o usuário como 'aluno'
        auto proxyAluno = std::make_shared<ProxyCentralNotificacoes>(centralReal, aluno);

        // Criamos um novo comando com o proxy do aluno
        auto comandoFalso = std::make_shared<EnviarNotificacaoCommand>(proxyAluno, "Mensagem falsa que não será enviada.");
        facade->executarComando(comandoFalso);


    } catch (const std::exception& e) {
        std::cerr << "❌ Erro: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\n--- Aplicação finalizada com sucesso! ---\n";
    return 0;
}