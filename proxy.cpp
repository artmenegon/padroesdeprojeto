// =============================================================================
// Padrão Estrutural: Proxy
// Aplicação: Harmonic — App de avaliação de músicas (estilo Letterboxd)
// Contexto: Controle de acesso ao perfil de um usuário (privado ou bloqueado)
// LLM utilizada: Claude (Anthropic)
// Referência: https://refactoring.guru/pt-br/design-patterns/proxy
// =============================================================================

#include <iostream>
#include <string>

// -----------------------------------------------------------------------------
// INTERFACE DO SUJEITO
// -----------------------------------------------------------------------------

class IPerfilUsuario {
public:
    virtual void verPerfil(const std::string& visitante) const = 0;
    virtual ~IPerfilUsuario() {}
};

// -----------------------------------------------------------------------------
// SUJEITO REAL — exibe o perfil sem restrições
// -----------------------------------------------------------------------------

class PerfilReal : public IPerfilUsuario {
private:
    std::string usuario;
    std::string musica_favorita;
public:
    PerfilReal(const std::string& u, const std::string& m)
        : usuario(u), musica_favorita(m) {}

    void verPerfil(const std::string& visitante) const override {
        std::cout << "  Usuario        : @" << usuario << "\n";
        std::cout << "  Musica favorita: " << musica_favorita << "\n";
    }
};

// -----------------------------------------------------------------------------
// PROXY — intercepta e verifica permissões antes de delegar ao real
// -----------------------------------------------------------------------------

class ProxyPerfil : public IPerfilUsuario {
private:
    PerfilReal   perfilReal;
    std::string  dono;
    std::string  bloqueado;
    bool         privado;

public:
    ProxyPerfil(const std::string& dono,
                const std::string& musica,
                bool privado,
                const std::string& bloqueado = "")
        : perfilReal(dono, musica),
          dono(dono),
          bloqueado(bloqueado),
          privado(privado) {}

    void verPerfil(const std::string& visitante) const override {
        if (visitante == bloqueado) {
            std::cout << "  [Bloqueado] @" << visitante
                      << " nao pode ver este perfil.\n";
            return;
        }
        if (privado && visitante != dono) {
            std::cout << "  [Privado] Este perfil e privado.\n";
            return;
        }
        perfilReal.verPerfil(visitante);
    }
};

// -----------------------------------------------------------------------------
// MAIN
// -----------------------------------------------------------------------------

int main() {
    std::cout << "=== Harmonic — Controle de Acesso a Perfis ===\n";

    // Perfil publico com um usuario bloqueado
    ProxyPerfil perfil1("carlos", "Bohemian Rhapsody - Queen", false, "spam_user");

    std::cout << "\n-- @ana tenta ver @carlos (acesso normal) --\n";
    perfil1.verPerfil("ana");

    std::cout << "\n-- @spam_user tenta ver @carlos (bloqueado) --\n";
    perfil1.verPerfil("spam_user");

    // Perfil privado
    ProxyPerfil perfil2("julia", "Blinding Lights - The Weeknd", true);

    std::cout << "\n-- @pedro tenta ver @julia (perfil privado) --\n";
    perfil2.verPerfil("pedro");

    std::cout << "\n-- @julia ve o proprio perfil --\n";
    perfil2.verPerfil("julia");

    return 0;
}
