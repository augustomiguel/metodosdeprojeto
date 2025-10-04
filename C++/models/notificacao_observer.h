// =============================================================================
// notificacao_observer.h
// Implementação do Padrão de Projeto Observer
// =============================================================================

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include "ICentralNotificacoes.h"

// =============================================================================
// 1. OBSERVER: A interface para os objetos que serão notificados
// =============================================================================

/**
 * @brief A interface Observer declara o método de atualização (update),
 * que será chamado pelos Subjects.
 */
class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update(const std::string& mensagem) = 0;
};

// =============================================================================
// 2. SUBJECT: A interface para os objetos que enviam as notificações
// =============================================================================

/**
 * @brief A interface Subject declara os métodos para registrar, remover e
 * notificar os observers.
 */
class ISubject {
public:
    virtual ~ISubject() = default;
    virtual void registrar(std::shared_ptr<IObserver> observer) = 0;
    virtual void remover(std::shared_ptr<IObserver> observer) = 0;
    virtual void notificar() = 0;
};

// =============================================================================
// 3. CONCRETE SUBJECT: Implementação concreta do Subject
// =============================================================================

/**
 * @brief A CentralDeNotificacoes é o Subject concreto. Ela mantém uma lista
 * de observers e notifica-os quando seu estado (a mensagem) muda.
 */
class CentralDeNotificacoes : public ISubject, public ICentralNotificacoes {
private:
    std::vector<std::shared_ptr<IObserver>> observers;
    std::string mensagemAtual;

public:
    void registrar(std::shared_ptr<IObserver> observer) override {
        observers.push_back(observer);
    }

    void remover(std::shared_ptr<IObserver> observer) override {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notificar() override {
        std::cout << "Central: Notificando " << observers.size() << " observador(es)..." << std::endl;
        for (const auto& observer : observers) {
            observer->update(mensagemAtual);
        }
    }

    /**
     * @brief Este método é chamado para simular o envio de uma nova mensagem
     * pelo motorista. Ele atualiza o estado e notifica os observers.
     */
    void enviarNovaMensagem(const std::string& mensagem) override {
        this->mensagemAtual = mensagem;
        notificar();
    }
};

// =============================================================================
// 4. CONCRETE OBSERVER: Implementação concreta do Observer
// =============================================================================

/**
 * @brief O AlunoApp é um Observer concreto. Ele representa o aplicativo
 * de um aluno que reage às notificações do motorista.
 */
class AlunoApp : public IObserver {
private:
    std::string nomeDoAluno;

public:
    AlunoApp(const std::string& nome) : nomeDoAluno(nome) {}

    void update(const std::string& mensagem) override {
        std::cout << "[App de " << nomeDoAluno << "]: Nova notificacao -> \"" << mensagem << "\"" << std::endl;
    }
};