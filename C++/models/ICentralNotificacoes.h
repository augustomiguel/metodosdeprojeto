#pragma once
#include <string>

class ICentralNotificacoes {
public:
    virtual ~ICentralNotificacoes() = default;
    virtual void enviarNovaMensagem(const std::string& mensagem) = 0;
};