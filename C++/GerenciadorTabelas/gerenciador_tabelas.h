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

    FacadeSingletonController(std::shared_ptr<GerenciadorTabelas> g) : gerenciador(g) {}

public:
    static std::shared_ptr<FacadeSingletonController> getInstance(std::shared_ptr<GerenciadorTabelas> g = nullptr) {
        if (!instancia && g) {
            instancia = std::shared_ptr<FacadeSingletonController>(new FacadeSingletonController(g));
        }
        return instancia;
    }

    // CRUD Usuario
    std::any criarUsuario(const Usuario& u) { return gerenciador->criar(u); }
    std::optional<Usuario> buscarUsuario(const std::any& id) { return gerenciador->buscar<Usuario>("usuarios", id); }
    std::vector<Usuario> buscarTodosUsuarios() { return gerenciador->buscarTodos<Usuario>("usuarios"); }
    bool atualizarUsuario(const Usuario& u) { return gerenciador->atualizar(u); }
    bool excluirUsuario(const Usuario& u) { return gerenciador->excluir(u); }

    // CRUD Rota
    std::any criarRota(const Rota& r) { return gerenciador->criar(r); }
    std::optional<Rota> buscarRota(const std::any& id) { return gerenciador->buscar<Rota>("rotas", id); }
    std::vector<Rota> buscarTodasRotas() { return gerenciador->buscarTodos<Rota>("rotas"); }
    bool atualizarRota(const Rota& r) { return gerenciador->atualizar(r); }
    bool excluirRota(const Rota& r) { return gerenciador->excluir(r); }

    // Método para retornar a quantidade de entidades cadastradas
    int quantidadeEntidades(const std::string& tabela) {
        // Use a conexão diretamente para contar registros
        return static_cast<int>(gerenciador->buscarTodos<Usuario>(tabela).size());
    }
};

std::shared_ptr<FacadeSingletonController> FacadeSingletonController::instancia = nullptr;

// =============================================================================
// 8. EXEMPLO DE USO SUPER SIMPLES
// =============================================================================

class ExemploSimples {
public:
    static void executar() {
        std::cout << "Gerenciador de Tabelas - Versão Simples" << std::endl;
        std::cout << std::string(50, '=') << std::endl;

        // Criar conexão mock e gerenciador
        auto conexao = std::make_shared<ConexaoMock>();
        auto gerenciador = std::make_shared<GerenciadorTabelas>(conexao);

        // Inicializar fachada singleton
        auto facade = FacadeSingletonController::getInstance(gerenciador);

        std::cout << "\nCriando usuários..." << std::endl;
        
        // Criar usuários
        Usuario usuario1("Alice Silva", "alice@email.com", 28);
        Usuario usuario2("Bruno Santos", "bruno@email.com", 35);
        
        auto id1 = gerenciador->criar(usuario1);
        auto id2 = gerenciador->criar(usuario2);

        std::cout << "\nCriando produtos..." << std::endl;
        
        // Criar produtos
        Produto produto1("Notebook", 2500.00, "Informática");
        Produto produto2("Mouse", 50.00, "Informática");
        
        auto idProd1 = gerenciador->criar(produto1);
        auto idProd2 = gerenciador->criar(produto2);

        std::cout << "\nBuscando usuários..." << std::endl;
        
        // Buscar usuários
        auto usuarioEncontrado = gerenciador->buscar<Usuario>("usuarios", id1);
        if (usuarioEncontrado.has_value()) {
            std::cout << "Usuário encontrado: ";
            usuarioEncontrado.value().mostrar();
        }

        std::cout << "\nListando todos os usuários..." << std::endl;
        
        auto todosUsuarios = gerenciador->buscarTodos<Usuario>("usuarios");
        for (const auto& usuario : todosUsuarios) {
            usuario.mostrar();
        }

        std::cout << "\nListando todos os produtos..." << std::endl;
        
        auto todosProdutos = gerenciador->buscarTodos<Produto>("produtos");
        for (const auto& produto : todosProdutos) {
            produto.mostrar();
        }

        std::cout << "\nAtualizando usuário..." << std::endl;
        
        // Atualizar usuário
        if (usuarioEncontrado.has_value()) {
            auto& usuario = usuarioEncontrado.value();
            usuario.setIdade(29);
            usuario.setEmail("alice.nova@email.com");
            
            if (gerenciador->atualizar(usuario)) {
                std::cout << "Usuário atualizado com sucesso!" << std::endl;
                usuario.mostrar();
            }
        }

        std::cout << "\nExcluindo produto..." << std::endl;
        
        // Excluir produto
        if (gerenciador->excluir("produtos", idProd2)) {
            std::cout << "Produto excluído com sucesso!" << std::endl;
        }

        std::cout << "\nProdutos restantes:" << std::endl;
        auto produtosRestantes = gerenciador->buscarTodos<Produto>("produtos");
        for (const auto& produto : produtosRestantes) {
            produto.mostrar();
        }

        // =========================
        // Testando CRUD de Rota
        // =========================
        std::cout << "\nCriando rotas..." << std::endl;
        Rota rota1("Circular Sede", {"07:00", "08:00", "09:00"}, {"Portaria", "RU", "Biblioteca"});
        Rota rota2("Sede - CTDR", {"10:00", "11:00"}, {"Sede", "CTDR"});

        auto idRota1 = facade->criarRota(rota1);
        auto idRota2 = facade->criarRota(rota2);

        std::cout << "\nListando todas as rotas:" << std::endl;
        auto rotas = facade->buscarTodasRotas();
        for (const auto& r : rotas) r.mostrar();

        std::cout << "\nAtualizando nome da primeira rota..." << std::endl;
        auto rotaEncontrada = facade->buscarRota(idRota1);
        if (rotaEncontrada.has_value()) {
            Rota r = rotaEncontrada.value();
            r.setNome("Circular Sede Atualizada");
            facade->atualizarRota(r);
        }

        std::cout << "\nRotas após atualização:" << std::endl;
        rotas = facade->buscarTodasRotas();
        for (const auto& r : rotas) r.mostrar();

        std::cout << "\nExcluindo segunda rota..." << std::endl;
        rotaEncontrada = facade->buscarRota(idRota2);
        if (rotaEncontrada.has_value()) {
            facade->excluirRota(rotaEncontrada.value());
        }

        std::cout << "\nRotas restantes:" << std::endl;
        rotas = facade->buscarTodasRotas();
        for (const auto& r : rotas) r.mostrar();

        std::cout << "\nQuantidade de rotas cadastradas: " << facade->quantidadeEntidades("rotas") << std::endl;

        // Mostrar dados finais
        std::cout << std::endl;
        conexao->mostrarDados();

        std::cout << "\nExemplo concluído com sucesso!" << std::endl;
    }
};

// =============================================================================
// 9. MAIN FUNCTION
// =============================================================================

int main() {
    try {
        ExemploSimples::executar();
    } catch (const std::exception& e) {
        std::cerr << "❌ Erro: " << e.what() << std::endl;
        return 1;
    }
}

// =============================================================================
// 10. COMPILAÇÃO E USO
// =============================================================================

/*
COMPILAÇÃO:
g++ -std=c++17 -Wall -Wextra gerenciador_simples.cpp -o gerenciador_simples

EXECUÇÃO:
./gerenciador_simples
*/