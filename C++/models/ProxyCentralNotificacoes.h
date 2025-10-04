#pragma once
#include "ICentralNotificacoes.h"
#include "notificacao_observer.h"
#include "../GerenciadorTabelas/Gerenciador.h" // Para a classe Usuario
#include <memory>

/**
 * @brief Proxy: Controla o acesso à CentralDeNotificacoes.
 * Só permite que usuários com o papel "motorista" enviem mensagens.
 */
class ProxyCentralNotificacoes : public ICentralNotificacoes {
private:
    std::shared_ptr<CentralDeNotificacoes> real_central; // O objeto real
    Usuario usuarioLogado; // O usuário tentando realizar a ação

public:
    ProxyCentralNotificacoes(std::shared_ptr<CentralDeNotificacoes> real, Usuario usuario)
        : real_central(real), usuarioLogado(usuario) {}

    void enviarNovaMensagem(const std::string& mensagem) override {
        if (checkAccess()) {
            std::cout << "[PROXY] Acesso permitido para " << usuarioLogado.getNome() << ".\n";
            // Se o acesso for permitido, delega a chamada para o objeto real
            real_central->enviarNovaMensagem(mensagem);
        } else {
            std::cout << "[PROXY] ACESSO NEGADO para " << usuarioLogado.getNome() << "! Papel '" << usuarioLogado.getPapel() << "' não autorizado a enviar notificações.\n";
        }
    }

private:
    bool checkAccess() const {
        // A lógica de verificação de permissão
        return usuarioLogado.getPapel() == "motorista";
    }
};