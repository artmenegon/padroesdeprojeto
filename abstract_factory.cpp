// =============================================================================
// Padrão Criacional: Abstract Factory
// Aplicação: Harmonic — App de avaliação de músicas (estilo Letterboxd)
// Contexto: Criação de contas de usuário (Gratuita e Premium)
// LLM utilizada: Claude (Anthropic)
// Referência: https://refactoring.guru/pt-br/design-patterns/abstract-factory
// =============================================================================

#include <iostream>
#include <string>

// -----------------------------------------------------------------------------
// PRODUTOS ABSTRATOS
// -----------------------------------------------------------------------------

class Perfil {
public:
    virtual void exibir() const = 0;
    virtual ~Perfil() {}
};

class Biblioteca {
public:
    virtual void exibir() const = 0;
    virtual ~Biblioteca() {}
};

// -----------------------------------------------------------------------------
// FAMÍLIA 1: Conta Gratuita
// -----------------------------------------------------------------------------

class PerfilGratuito : public Perfil {
public:
    void exibir() const override {
        std::cout << "  Perfil: Gratuito | Foto padrao | Bio: ate 100 caracteres\n";
    }
};

class BibliotecaGratuita : public Biblioteca {
public:
    void exibir() const override {
        std::cout << "  Biblioteca: ate 50 musicas salvas | Sem estatisticas\n";
    }
};

// -----------------------------------------------------------------------------
// FAMÍLIA 2: Conta Premium
// -----------------------------------------------------------------------------

class PerfilPremium : public Perfil {
public:
    void exibir() const override {
        std::cout << "  Perfil: Premium | Foto customizada | Bio ilimitada | Selo verificado\n";
    }
};

class BibliotecaPremium : public Biblioteca {
public:
    void exibir() const override {
        std::cout << "  Biblioteca: musicas ilimitadas | Estatisticas detalhadas | Exportacao\n";
    }
};

// -----------------------------------------------------------------------------
// FÁBRICA ABSTRATA
// -----------------------------------------------------------------------------

class FabricaDeConta {
public:
    virtual Perfil*    criarPerfil()    const = 0;
    virtual Biblioteca* criarBiblioteca() const = 0;
    virtual ~FabricaDeConta() {}
};

// -----------------------------------------------------------------------------
// FÁBRICAS CONCRETAS
// -----------------------------------------------------------------------------

class FabricaGratuita : public FabricaDeConta {
public:
    Perfil*    criarPerfil()    const override { return new PerfilGratuito();    }
    Biblioteca* criarBiblioteca() const override { return new BibliotecaGratuita(); }
};

class FabricaPremium : public FabricaDeConta {
public:
    Perfil*    criarPerfil()    const override { return new PerfilPremium();    }
    Biblioteca* criarBiblioteca() const override { return new BibliotecaPremium(); }
};

// -----------------------------------------------------------------------------
// CLIENTE
// -----------------------------------------------------------------------------

void cadastrarUsuario(const FabricaDeConta& fabrica, const std::string& usuario) {
    std::cout << "\n  Cadastrando @" << usuario << "...\n";

    Perfil*    perfil    = fabrica.criarPerfil();
    Biblioteca* biblioteca = fabrica.criarBiblioteca();

    perfil->exibir();
    biblioteca->exibir();

    std::cout << "  Cadastro concluido!\n";

    delete perfil;
    delete biblioteca;
}

// -----------------------------------------------------------------------------
// MAIN
// -----------------------------------------------------------------------------

int main() {
    std::cout << "=== Harmonic — Cadastro de Usuarios ===\n";

    FabricaGratuita gratuita;
    cadastrarUsuario(gratuita, "lucas_ouvinte");

    FabricaPremium premium;
    cadastrarUsuario(premium, "ana_melomana");

    return 0;
}
