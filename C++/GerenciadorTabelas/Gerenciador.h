// =============================================================================
// gerenciador_tabelas.h
// =============================================================================

#pragma once
#include <string>
#include <map>
#include <vector>
#include <any>
#include <optional>
#include <memory>
#include <functional>
#include <iostream>

#include "UsuarioMemento.h"
#include <memory>

#include <stack>                      
#include "../commands/ICommand.h"

// =============================================================================
// 1. INTERFACE BÁSICA PARA ENTIDADES
// =============================================================================

class Entidade {
public:
    virtual ~Entidade() = default;
    virtual std::map<std::string, std::any> paraDados() const = 0;
    virtual void deDados(const std::map<std::string, std::any>& dados) = 0;
    virtual std::string obterTabela() const = 0;
    virtual std::any obterId() const = 0;
};

// =============================================================================
// 2. CONEXÃO
// =============================================================================

class Conexao {
public:
    virtual ~Conexao() = default;
    virtual std::any inserir(const std::string& tabela, const std::map<std::string, std::any>& dados) = 0;
    virtual std::optional<std::map<std::string, std::any>> buscar(const std::string& tabela, const std::any& id) = 0;
    virtual std::vector<std::map<std::string, std::any>> buscarTodos(const std::string& tabela) = 0;
    virtual bool atualizar(const std::string& tabela, const std::any& id, const std::map<std::string, std::any>& dados) = 0;
    virtual bool excluir(const std::string& tabela, const std::any& id) = 0;
};

// =============================================================================
// 3. IMPLEMENTAÇÃO MOCK (PARA TESTES/EXEMPLO)
// =============================================================================

class ConexaoMock : public Conexao {
private:
    // Troque std::any por int na chave do map
    std::map<std::string, std::map<int, std::map<std::string, std::any>>> dados;
    int proximoId = 1;

public:
    std::any inserir(const std::string& tabela, const std::map<std::string, std::any>& registro) override {
        int id = proximoId++;
        auto novoRegistro = registro;
        novoRegistro["id"] = id;
        dados[tabela][id] = novoRegistro;
        std::cout << "Inserido na tabela '" << tabela << "' com ID: " << id << std::endl;
        return id;
    }

    std::optional<std::map<std::string, std::any>> buscar(const std::string& tabela, const std::any& id) override {
        auto itTabela = dados.find(tabela);
        if (itTabela == dados.end()) return std::nullopt;

        int idInt = 0;
        try {
            idInt = std::any_cast<int>(id);
        } catch (...) {
            return std::nullopt;
        }

        auto itRegistro = itTabela->second.find(idInt);
        if (itRegistro == itTabela->second.end()) return std::nullopt;

        return itRegistro->second;
    }

    std::vector<std::map<std::string, std::any>> buscarTodos(const std::string& tabela) override {
        std::vector<std::map<std::string, std::any>> resultado;
        auto itTabela = dados.find(tabela);
        if (itTabela != dados.end()) {
            for (const auto& [id, registro] : itTabela->second) {
                resultado.push_back(registro);
            }
        }
        return resultado;
    }

    bool atualizar(const std::string& tabela, const std::any& id, const std::map<std::string, std::any>& novosDados) override {
        auto itTabela = dados.find(tabela);
        if (itTabela == dados.end()) return false;

        int idInt = 0;
        try {
            idInt = std::any_cast<int>(id);
        } catch (...) {
            return false;
        }

        auto itRegistro = itTabela->second.find(idInt);
        if (itRegistro == itTabela->second.end()) return false;

        for (const auto& [chave, valor] : novosDados) {
            itRegistro->second[chave] = valor;
        }

        std::cout << "Atualizado na tabela '" << tabela << "' ID: " << idInt << std::endl;
        return true;
    }

    bool excluir(const std::string& tabela, const std::any& id) override {
        auto itTabela = dados.find(tabela);
        if (itTabela == dados.end()) return false;

        int idInt = 0;
        try {
            idInt = std::any_cast<int>(id);
        } catch (...) {
            return false;
        }

        auto removidos = itTabela->second.erase(idInt);
        if (removidos > 0) {
            std::cout << "Excluído da tabela '" << tabela << "' ID: " << idInt << std::endl;
            return true;
        }
        return false;
    }

    // Método extra para debug
    void mostrarDados() const {
        std::cout << "\nDados armazenados:" << std::endl;
        for (const auto& [tabela, registros] : dados) {
            std::cout << "  Tabela '" << tabela << "': " << registros.size() << " registros" << std::endl;
        }
    }
};

// =============================================================================
// 4. GERENCIADOR PRINCIPAL
// =============================================================================

class GerenciadorTabelas {
private:
    std::shared_ptr<Conexao> conexao;

public:
    GerenciadorTabelas(std::shared_ptr<Conexao> conn) : conexao(conn) {}

    // Criar registro
    template<typename T>
    std::any criar(const T& entidade) {
        auto dados = entidade.paraDados();
        return conexao->inserir(entidade.obterTabela(), dados);
    }

    // Buscar por ID
    template<typename T>
    std::optional<T> buscar(const std::string& tabela, const std::any& id) {
        auto dadosOpt = conexao->buscar(tabela, id);
        if (!dadosOpt.has_value()) {
            return std::nullopt;
        }

        T entidade;
        entidade.deDados(dadosOpt.value());
        return entidade;
    }

    // Buscar todos
    template<typename T>
    std::vector<T> buscarTodos(const std::string& tabela) {
        auto dadosList = conexao->buscarTodos(tabela);
        std::vector<T> resultado;

        for (const auto& dados : dadosList) {
            T entidade;
            entidade.deDados(dados);
            resultado.push_back(entidade);
        }

        return resultado;
    }

    // Atualizar
    template<typename T>
    bool atualizar(const T& entidade) {
        auto dados = entidade.paraDados();
        return conexao->atualizar(entidade.obterTabela(), entidade.obterId(), dados);
    }

    // Excluir
    template<typename T>
    bool excluir(const T& entidade) {
        return conexao->excluir(entidade.obterTabela(), entidade.obterId());
    }

    // Excluir por ID
    bool excluir(const std::string& tabela, const std::any& id) {
        return conexao->excluir(tabela, id);
    }
};

// =============================================================================
// 5. EXEMPLO DE ENTIDADE SIMPLES - USUÁRIO
// =============================================================================

class Usuario : public Entidade {
private:
    int id = 0;
    std::string nome;
    std::string email;
    int idade = 0;

public:
    // Construtores
    Usuario() = default;
    Usuario(const std::string& n, const std::string& e, int i) 
        : nome(n), email(e), idade(i) {}

    // Getters
    int getId() const { return id; }
    const std::string& getNome() const { return nome; }
    const std::string& getEmail() const { return email; }
    int getIdade() const { return idade; }

    // Setters
    void setId(int novoId) { id = novoId; }
    void setNome(const std::string& novoNome) { nome = novoNome; }
    void setEmail(const std::string& novoEmail) { email = novoEmail; }
    void setIdade(int novaIdade) { idade = novaIdade; }

    /**
     * @brief Cria um Memento que contém uma cópia do estado atual do usuário.
     * Este é o método "Salvar Estado".
     * @return Um ponteiro inteligente para o objeto Memento recém-criado.
     */
    std::shared_ptr<UsuarioMemento> criarMemento() const {
        // 1. Usa o construtor privado de UsuarioMemento (lembra do 'friend'?).
        // 2. Passa os valores dos atributos *atuais* deste objeto (this->nome, etc.).
        // 3. std::make_shared cria o objeto Memento de forma segura.
        return std::make_shared<UsuarioMemento>(this->nome, this->email, this->idade);
    }

    /**
     * @brief Restaura o estado do usuário a partir de um Memento.
     * Este é o método "Restaurar Estado".
     * @param memento O objeto Memento contendo o estado salvo a ser restaurado.
     */
    void restaurarMemento(std::shared_ptr<UsuarioMemento> memento) {
        // 1. Pega os valores guardados dentro do objeto memento.
        // 2. Sobrescreve os atributos *atuais* deste objeto com esses valores salvos.
        //    (Isso só é possível porque declaramos 'friend class Usuario' no Memento).
        this->nome = memento->nome;
        this->email = memento->email;
        this->idade = memento->idade;
    }

    // Interface Entidade
    std::map<std::string, std::any> paraDados() const override {
        return {
            {"id", id},
            {"nome", nome},
            {"email", email},
            {"idade", idade}
        };
    }

    void deDados(const std::map<std::string, std::any>& dados) override {
        auto getValue = [&](const std::string& key, auto defaultVal) {
            auto it = dados.find(key);
            if (it != dados.end()) {
                try {
                    return std::any_cast<decltype(defaultVal)>(it->second);
                } catch (const std::bad_any_cast&) {
                    return defaultVal;
                }
            }
            return defaultVal;
        };

        id = getValue("id", 0);
        nome = getValue("nome", std::string{});
        email = getValue("email", std::string{});
        idade = getValue("idade", 0);
    }

    std::string obterTabela() const override {
        return "usuarios";
    }

    std::any obterId() const override {
        return id;
    }

    // Método para exibição
    void mostrar() const {
        std::cout << "Usuario[" << id << "]: " << nome 
                  << " (" << email << ", " << idade << " anos)" << std::endl;
    }
};

// =============================================================================
// 6. EXEMPLO DE ENTIDADE SIMPLES - PRODUTO
// =============================================================================

class Produto : public Entidade {
private:
    int id = 0;
    std::string nome;
    double preco = 0.0;
    std::string categoria;

public:
    // Construtores
    Produto() = default;
    Produto(const std::string& n, double p, const std::string& c) 
        : nome(n), preco(p), categoria(c) {}

    // Getters
    int getId() const { return id; }
    const std::string& getNome() const { return nome; }
    double getPreco() const { return preco; }
    const std::string& getCategoria() const { return categoria; }

    // Setters
    void setId(int novoId) { id = novoId; }
    void setNome(const std::string& novoNome) { nome = novoNome; }
    void setPreco(double novoPreco) { preco = novoPreco; }
    void setCategoria(const std::string& novaCategoria) { categoria = novaCategoria; }

    // Interface Entidade
    std::map<std::string, std::any> paraDados() const override {
        return {
            {"id", id},
            {"nome", nome},
            {"preco", preco},
            {"categoria", categoria}
        };
    }

    void deDados(const std::map<std::string, std::any>& dados) override {
        auto getValue = [&](const std::string& key, auto defaultVal) {
            auto it = dados.find(key);
            if (it != dados.end()) {
                try {
                    return std::any_cast<decltype(defaultVal)>(it->second);
                } catch (const std::bad_any_cast&) {
                    return defaultVal;
                }
            }
            return defaultVal;
        };

        id = getValue("id", 0);
        nome = getValue("nome", std::string{});
        preco = getValue("preco", 0.0);
        categoria = getValue("categoria", std::string{});
    }

    std::string obterTabela() const override {
        return "produtos";
    }

    std::any obterId() const override {
        return id;
    }

    // Método para exibição
    void mostrar() const {
        std::cout << "Produto[" << id << "]: " << nome 
                  << " - R$ " << preco << " (" << categoria << ")" << std::endl;
    }
};

// =============================================================================
// 6.1. NOVA ENTIDADE - ROTA
// =============================================================================

class Rota : public Entidade {
private:
    int id = 0;
    std::string nome;
    std::vector<std::string> horarios;
    std::vector<std::string> pontosDeParada;

public:
    // Construtores
    Rota() = default;
    Rota(const std::string& n, const std::vector<std::string>& h, const std::vector<std::string>& p)
        : nome(n), horarios(h), pontosDeParada(p) {}

    // Getters
    int getId() const { return id; }
    const std::string& getNome() const { return nome; }
    const std::vector<std::string>& getHorarios() const { return horarios; }
    const std::vector<std::string>& getPontosDeParada() const { return pontosDeParada; }

    // Setters
    void setId(int novoId) { id = novoId; }
    void setNome(const std::string& novoNome) { nome = novoNome; }
    void setHorarios(const std::vector<std::string>& novosHorarios) { horarios = novosHorarios; }
    void setPontosDeParada(const std::vector<std::string>& novosPontos) { pontosDeParada = novosPontos; }

    // Interface Entidade
    std::map<std::string, std::any> paraDados() const override {
        return {
            {"id", id},
            {"nome", nome},
            {"horarios", horarios},
            {"pontosDeParada", pontosDeParada}
        };
    }

    void deDados(const std::map<std::string, std::any>& dados) override {
        auto getValue = [&](const std::string& key, auto defaultVal) {
            auto it = dados.find(key);
            if (it != dados.end()) {
                try {
                    return std::any_cast<decltype(defaultVal)>(it->second);
                } catch (const std::bad_any_cast&) {
                    return defaultVal;
                }
            }
            return defaultVal;
        };
        id = getValue("id", 0);
        nome = getValue("nome", std::string{});
        try {
            horarios = std::any_cast<std::vector<std::string>>(dados.at("horarios"));
        } catch (...) {
            horarios.clear();
        }
        try {
            pontosDeParada = std::any_cast<std::vector<std::string>>(dados.at("pontosDeParada"));
        } catch (...) {
            pontosDeParada.clear();
        }
    }

    std::string obterTabela() const override {
        return "rotas";
    }

    std::any obterId() const override {
        return id;
    }

    // Método para exibição
    void mostrar() const {
        std::cout << "Rota[" << id << "]: " << nome << "\n  Horários: ";
        for (const auto& h : horarios) std::cout << h << " ";
        std::cout << "\n  Pontos: ";
        for (const auto& p : pontosDeParada) std::cout << p << " ";
        std::cout << std::endl;
    }
};

// =============================================================================
// 7. FACHADA SINGLETON PARA OS GERENTES (CONTROLLERS)
// =============================================================================

class FacadeSingletonController {
private:
    static std::shared_ptr<FacadeSingletonController> instancia;
    std::shared_ptr<GerenciadorTabelas> gerenciador;

     std::stack<std::shared_ptr<ICommand>> historicoComandos;


    FacadeSingletonController(std::shared_ptr<GerenciadorTabelas> g) : gerenciador(g) {}

public:
    static std::shared_ptr<FacadeSingletonController> getInstance(std::shared_ptr<GerenciadorTabelas> g = nullptr) {
        if (!instancia && g) {
            instancia = std::shared_ptr<FacadeSingletonController>(new FacadeSingletonController(g));
        }
        return instancia;
    }

    /**
     * @brief Permite que os comandos acessem o gerenciador de dados.
     * @return Um ponteiro para o GerenciadorTabelas (o Receiver).
     */
    std::shared_ptr<GerenciadorTabelas> getGerenciador() {
        return gerenciador;
    }

     /**
     * @brief Invoker: Executa um comando e o armazena no histórico.
     * @param comando O objeto de comando a ser executado.
     */
    void executarComando(std::shared_ptr<ICommand> comando) {
        comando->execute();
        historicoComandos.push(comando); // Empilha o comando para um futuro 'undo'
    }

    /**
     * @brief Desfaz a última operação executada.
     */
    void desfazerUltimaAcao() {
        if (!historicoComandos.empty()) {
            auto ultimoComando = historicoComandos.top(); // Pega o último comando
            ultimoComando->undo();                         // Chama o 'undo' dele
            historicoComandos.pop();                       // Remove do histórico
        } else {
            std::cout << "Nenhuma ação para desfazer.\n";
        }
    }


    // Método para retornar a quantidade de entidades cadastradas
    int quantidadeEntidades(const std::string& tabela) {
        // Use a conexão diretamente para contar registros
        return static_cast<int>(gerenciador->buscarTodos<Usuario>(tabela).size());
    }
};

std::shared_ptr<FacadeSingletonController> FacadeSingletonController::instancia = nullptr;

// =============================================================================
// 10. COMPILAÇÃO E USO
// =============================================================================

/*
COMPILAÇÃO:
g++ -std=c++17 -Wall -Wextra gerenciador_simples.cpp -o gerenciador_simples

EXECUÇÃO:
./gerenciador_simples
*/