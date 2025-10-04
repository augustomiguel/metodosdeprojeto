// commands/EnviarNotificacaoCommand.h

#pragma once
#include "ICommand.h"
#include "../models/notificacao_observer.h" // Precisa da CentralDeNotificacoes
#include <memory>
#include <string>

class EnviarNotificacaoCommand : public ICommand {
private:
    std::shared_ptr<CentralDeNotificacoes> central;
    std::string mensagem;

public:
    EnviarNotificacaoCommand(std::shared_ptr<CentralDeNotificacoes> c, const std::string& msg)
        : central(c), mensagem(msg) {}

    void execute() override {
        central->enviarNovaMensagem(mensagem);
    }

    // "Desfazer" uma notificação não faz sentido, então o undo é vazio.
    void undo() override {}
};