#ifndef MONGODB_INTERFACE_H
#define MONGODB_INTERFACE_H

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include "../models/Usuario.h"
#include <vector>
#include <iostream>

class MongoDBInterface {
private:
    mongocxx::client client;
    mongocxx::database db;
    mongocxx::collection collection;

public:
    MongoDBInterface()
        : client(mongocxx::uri{}), db(client["sistema_usuarios"]), collection(db["usuarios"]) {}

    void salvarUsuario(const Usuario& usuario) {
        using namespace bsoncxx::builder::stream;
        document doc{};
        doc << "nome" << usuario.getLogin()  // substituindo por login por simplicidade
            << "email" << "placeholder@example.com"
            << "login" << usuario.getLogin();

        collection.insert_one(doc.view());
        std::cout << "Usuário salvo no MongoDB." << std::endl;
    }

    std::vector<Usuario> carregarUsuarios() {
        std::vector<Usuario> lista;
        auto cursor = collection.find({});

        for (auto&& doc : cursor) {
            std::string nome = doc["nome"].get_utf8().value.to_string();
            std::string email = doc["email"].get_utf8().value.to_string();
            std::string login = doc["login"].get_utf8().value.to_string();
            lista.emplace_back(nome, email, login);
        }

        return lista;
    }

    void excluirUsuarioPorLogin(const std::string& login) {
        collection.delete_one(bsoncxx::builder::stream::document{} << "login" << login << bsoncxx::builder::stream::finalize);
        std::cout << "Usuário com login '" << login << "' removido do MongoDB." << std::endl;
    }
};

#endif
