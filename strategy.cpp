// =============================================================================
// Padrão Comportamental: Strategy
// Aplicação: Harmonic — App de avaliação de músicas (estilo Letterboxd)
// Contexto: Diferentes formas de ordenar o feed de músicas avaliadas
// LLM utilizada: Claude (Anthropic)
// Referência: https://refactoring.guru/pt-br/design-patterns/strategy
// =============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// -----------------------------------------------------------------------------
// ESTRUTURA DE DADOS
// -----------------------------------------------------------------------------

struct Musica {
    std::string titulo;
    std::string artista;
    int         ano;
    double      nota;
};

// -----------------------------------------------------------------------------
// INTERFACE DA ESTRATÉGIA
// -----------------------------------------------------------------------------

class EstrategiaOrdenacao {
public:
    virtual void ordenar(std::vector<Musica>& musicas) = 0;
    virtual std::string nome() const = 0;
    virtual ~EstrategiaOrdenacao() {}
};

// -----------------------------------------------------------------------------
// ESTRATÉGIAS CONCRETAS
// -----------------------------------------------------------------------------

class OrdenarPorNota : public EstrategiaOrdenacao {
public:
    void ordenar(std::vector<Musica>& musicas) override {
        std::sort(musicas.begin(), musicas.end(),
            [](const Musica& a, const Musica& b) { return a.nota > b.nota; });
    }
    std::string nome() const override { return "Melhor Avaliadas"; }
};

class OrdenarPorAno : public EstrategiaOrdenacao {
public:
    void ordenar(std::vector<Musica>& musicas) override {
        std::sort(musicas.begin(), musicas.end(),
            [](const Musica& a, const Musica& b) { return a.ano > b.ano; });
    }
    std::string nome() const override { return "Mais Recentes"; }
};

// -----------------------------------------------------------------------------
// CONTEXTO
// -----------------------------------------------------------------------------

class Feed {
private:
    std::vector<Musica>    musicas;
    EstrategiaOrdenacao*   estrategia = nullptr;

public:
    void adicionar(const Musica& m) { musicas.push_back(m); }

    void setEstrategia(EstrategiaOrdenacao* e) { estrategia = e; }

    void exibir() {
        if (!estrategia) return;

        std::vector<Musica> copia = musicas;
        estrategia->ordenar(copia);

        std::cout << "\n  -- Feed: " << estrategia->nome() << " --\n";
        for (const auto& m : copia) {
            std::cout << "  " << m.titulo << " - " << m.artista
                      << " (" << m.ano << ") | Nota: " << m.nota << "\n";
        }
    }
};

// -----------------------------------------------------------------------------
// MAIN
// -----------------------------------------------------------------------------

int main() {
    std::cout << "=== Harmonic — Feed de Musicas ===\n";

    Feed feed;
    feed.adicionar({"Blinding Lights",    "The Weeknd", 2019, 4.0});
    feed.adicionar({"Bohemian Rhapsody",  "Queen",      1975, 5.0});
    feed.adicionar({"Lose Yourself",      "Eminem",     2002, 4.5});

    OrdenarPorNota porNota;
    feed.setEstrategia(&porNota);
    feed.exibir();

    OrdenarPorAno porAno;
    feed.setEstrategia(&porAno);
    feed.exibir();

    return 0;
}
