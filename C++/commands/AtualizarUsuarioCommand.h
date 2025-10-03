#pragma once
#include "ICommand.h"
#include <iostream> // Verifique se esta linha existe
#include <memory>   // Verifique se esta linha existe

// A linha abaixo é a mais importante. Verifique o caminho e o nome do arquivo.
// O caminho "../" significa "voltar uma pasta".
#include "../GerenciadorTabelas/Gerenciador.h" 

class AtualizarUsuarioCommand : public ICommand {
private:
    std::shared_ptr<GerenciadorTabelas> gerenciador;
    Usuario usuarioNovo;
    std::shared_ptr<UsuarioMemento> memento;

public:
    AtualizarUsuarioCommand(std::shared_ptr<GerenciadorTabelas> g, Usuario novo)
        : gerenciador(g), usuarioNovo(novo) {}

    void execute() override {
        auto usuarioOriginalOpt = gerenciador->buscar<Usuario>("usuarios", usuarioNovo.obterId());
        if (usuarioOriginalOpt.has_value()) {
            memento = usuarioOriginalOpt.value().criarMemento();
            std::cout << "[COMMAND] Estado do usuário '" << usuarioOriginalOpt.value().getNome() << "' salvo para 'undo'.\n";
        }
        gerenciador->atualizar(usuarioNovo);
        std::cout << "[COMMAND] Usuário '" << usuarioNovo.getNome() << "' atualizado.\n";
    }

    void undo() override {
        if (memento) {
            Usuario usuarioRestaurado;
            usuarioRestaurado.deDados({{"id", std::any_cast<int>(usuarioNovo.obterId())}});
            usuarioRestaurado.restaurarMemento(memento);
            gerenciador->atualizar(usuarioRestaurado);
            std::cout << "[UNDO] Ação de atualização desfeita. Usuário voltou a ser '" << usuarioRestaurado.getNome() << "'.\n";
        } else {
            std::cout << "[UNDO] Nada a ser desfeito.\n";
        }
    }
};