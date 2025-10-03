#pragma once

/**
 * @brief Command (Interface): Define a interface para executar e desfazer uma operação.
 */
class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0; // Executa a ação
    virtual void undo() = 0;    // Desfaz a ação
};