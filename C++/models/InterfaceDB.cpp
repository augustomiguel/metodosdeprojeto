#include "InterfaceDB.h"
#include <fstream> // Para operações de arquivo (ofstream, ifstream)
#include <sstream> // Para manipulação de strings (stringstream)
#include <iostream> // Para saída de console (cout, cerr)

// Define o nome do arquivo que será usado como "banco de dados"
const std::string NOME_ARQUIVO_DB = "usuarios_db.txt";

/**
 * @brief Salva um objeto Usuario no arquivo de banco de dados.
 * Cada usuário é salvo em uma nova linha no formato: Nome;Email;Login
 * @param usuario O objeto Usuario a ser salvo.
 */
void InterfaceDB::salvarUsuario(const Usuario& usuario) {
    // Abre o arquivo em modo de adição (ios::app) para não sobrescrever o conteúdo existente
    std::ofstream arquivo(NOME_ARQUIVO_DB, std::ios::app);
    if (arquivo.is_open()) {
        // Escreve os dados do usuário no arquivo, separados por ponto e vírgula
        arquivo << usuario.getNome() << ";" << usuario.getEmail() << ";" << usuario.getLogin() << std::endl;
        arquivo.close(); // Fecha o arquivo
        std::cout << "Usuário salvo no arquivo: " << usuario.getLogin() << std::endl;
    } else {
        // Exibe uma mensagem de erro se o arquivo não puder ser aberto
        std::cerr << "Erro: Não foi possível abrir o arquivo para salvar o usuário." << std::endl;
    }
}

/**
 * @brief Carrega todos os objetos Usuario do arquivo de banco de dados.
 * Lê cada linha do arquivo e parseia os dados para criar objetos Usuario.
 * @return Um vetor de objetos Usuario carregados.
 */
std::vector<Usuario> InterfaceDB::carregarUsuarios() {
    std::vector<Usuario> usuarios;
    // Abre o arquivo em modo de leitura
    std::ifstream arquivo(NOME_ARQUIVO_DB);
    if (arquivo.is_open()) {
        std::string linha;
        // Lê o arquivo linha por linha
        while (std::getline(arquivo, linha)) {
            std::stringstream ss(linha); // Usa stringstream para parsear a linha
            std::string nome, email, login;

            // Extrai os campos separados por ponto e vírgula
            std::getline(ss, nome, ';');
            std::getline(ss, email, ';');
            std::getline(ss, login, ';');

            // Cria um novo objeto Usuario e o adiciona ao vetor
            usuarios.emplace_back(nome, email, login);
        }
        arquivo.close(); // Fecha o arquivo
        std::cout << "Usuários carregados do arquivo." << std::endl;
    } else {
        // Exibe uma mensagem de erro se o arquivo não puder ser aberto ou não existir
        std::cerr << "Aviso: Arquivo de usuários não encontrado ou erro ao abrir. Criando um novo se necessário." << std::endl;
    }
    return usuarios;
}

/**
 * @brief Exclui um usuário do arquivo de banco de dados com base no login.
 * Lê todos os usuários, reescreve o arquivo excluindo o usuário desejado.
 * @param loginParaExcluir O login do usuário a ser excluído.
 */
void InterfaceDB::excluirUsuarioPorLogin(const std::string& loginParaExcluir) {
    // Carrega todos os usuários existentes
    std::vector<Usuario> usuariosAtuais = carregarUsuarios();
    bool encontrado = false;

    // Abre o arquivo em modo de truncamento (ios::trunc) para sobrescrever o conteúdo
    std::ofstream arquivo(NOME_ARQUIVO_DB, std::ios::trunc);

    if (arquivo.is_open()) {
        // Itera sobre os usuários e escreve no arquivo apenas aqueles que não serão excluídos
        for (const auto& u : usuariosAtuais) {
            if (u.getLogin() != loginParaExcluir) {
                // Reescreve os dados do usuário
                arquivo << u.getNome() << ";" << u.getEmail() << ";" << u.getLogin() << std::endl;
            } else {
                encontrado = true; // Marca que o usuário foi encontrado e será "excluído" (não reescrito)
            }
        }
        arquivo.close(); // Fecha o arquivo

        if (encontrado) {
            std::cout << "Usuário '" << loginParaExcluir << "' excluído do arquivo." << std::endl;
        } else {
            std::cout << "Usuário '" << loginParaExcluir << "' não encontrado no arquivo para exclusão." << std::endl;
        }
    } else {
        // Exibe uma mensagem de erro se o arquivo não puder ser aberto
        std::cerr << "Erro: Não foi possível abrir o arquivo para excluir o usuário." << std::endl;
    }
}
