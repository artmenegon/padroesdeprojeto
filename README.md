# 🎵 Harmonic — Design Patterns em C++

> **Aplicação:** Harmonic é um app fictício de avaliação de músicas inspirado no [Letterboxd](https://letterboxd.com/), porém voltado inteiramente para músicas no lugar de filmes.
> **Linguagem:** C++17
> **LLM utilizada nos exemplos:** Claude (Anthropic)
> **Referência dos padrões:** [Refactoring Guru — Catálogo de Padrões de Projeto](https://refactoring.guru/pt-br/design-patterns)

---

## 📚 Padrões Implementados

| # | Padrão | Categoria | Arquivo |
|---|--------|-----------|---------|
| 1 | [Abstract Factory](#1-abstract-factory--padrão-criacional) | Criacional | `abstract_factory/abstract_factory.cpp` |
| 2 | [Proxy](#2-proxy--padrão-estrutural) | Estrutural | `proxy/proxy.cpp` |
| 3 | [Strategy](#3-strategy--padrão-comportamental) | Comportamental | `strategy/strategy.cpp` |

---

## 1. Abstract Factory — Padrão Criacional

> **Referência:** https://refactoring.guru/pt-br/design-patterns/abstract-factory

### Contexto

O Harmonic oferece dois tipos de conta: **Gratuita** e **Premium**. Cada tipo possui seus próprios objetos — o perfil e a biblioteca de músicas têm características diferentes dependendo do plano.

### Problema

Como criar objetos de famílias diferentes (gratuita × premium) sem que o código cliente precise saber qual classe concreta está sendo instanciada? Usar `if/else` espalhados para decidir qual objeto criar dificulta a adição de novos planos no futuro.

### Solução

O **Abstract Factory** define uma interface (`FabricaDeConta`) com métodos de criação para cada produto da família. Fábricas concretas (`FabricaGratuita`, `FabricaPremium`) implementam essa interface. O cliente (`cadastrarUsuario`) trabalha apenas com abstrações, sem conhecer os tipos concretos.

### Diagrama

```
         FabricaDeConta  <<interface>>
          + criarPerfil()
          + criarBiblioteca()
                 ▲
        _________|_________
       |                   |
 FabricaGratuita     FabricaPremium
       |                   |
 PerfilGratuito       PerfilPremium
 BibliotecaGratuita   BibliotecaPremium
```

### Código Explicado

```cpp
// Interface abstrata: define quais produtos cada família deve oferecer
class FabricaDeConta {
public:
    virtual Perfil*     criarPerfil()     const = 0;
    virtual Biblioteca* criarBiblioteca() const = 0;
};

// Fábrica concreta para o plano gratuito
class FabricaGratuita : public FabricaDeConta {
public:
    Perfil*     criarPerfil()     const override { return new PerfilGratuito();     }
    Biblioteca* criarBiblioteca() const override { return new BibliotecaGratuita(); }
};

// Cliente: recebe qualquer fábrica — não sabe qual plano está sendo criado
void cadastrarUsuario(const FabricaDeConta& fabrica, const std::string& usuario) {
    Perfil*     perfil     = fabrica.criarPerfil();
    Biblioteca* biblioteca = fabrica.criarBiblioteca();
    perfil->exibir();
    biblioteca->exibir();
}
```

### Como compilar e executar

```bash
cd abstract_factory
g++ -std=c++17 abstract_factory.cpp -o abstract_factory
./abstract_factory
```

### Saída esperada

```
=== Harmonic — Cadastro de Usuarios ===

  Cadastrando @lucas_ouvinte...
  Perfil: Gratuito | Foto padrao | Bio: ate 100 caracteres
  Biblioteca: ate 50 musicas salvas | Sem estatisticas
  Cadastro concluido!

  Cadastrando @ana_melomana...
  Perfil: Premium | Foto customizada | Bio ilimitada | Selo verificado
  Biblioteca: musicas ilimitadas | Estatisticas detalhadas | Exportacao
  Cadastro concluido!
```

---

## 2. Proxy — Padrão Estrutural

> **Referência:** https://refactoring.guru/pt-br/design-patterns/proxy

### Contexto

No Harmonic, usuários podem deixar seu perfil **privado** ou **bloquear** outros usuários. Antes de exibir qualquer informação de um perfil, o sistema precisa verificar se o visitante tem permissão.

### Problema

Colocar a lógica de bloqueio e privacidade dentro da própria classe de perfil mistura responsabilidades. O perfil deveria apenas saber se exibir — não decidir quem pode vê-lo.

### Solução

O **Proxy** cria uma classe intermediária (`ProxyPerfil`) que implementa a mesma interface (`IPerfilUsuario`) que o objeto real (`PerfilReal`). O proxy verifica as permissões e, somente se autorizado, delega a chamada ao objeto real. O cliente não percebe a diferença.

### Diagrama

```
IPerfilUsuario  <<interface>>
 + verPerfil()
       ▲
  _____|_____
 |           |
PerfilReal  ProxyPerfil
            - perfilReal
            - bloqueado
            - privado
            + verPerfil() → verifica → delega
```

### Código Explicado

```cpp
// Interface comum para o objeto real e o proxy
class IPerfilUsuario {
public:
    virtual void verPerfil(const std::string& visitante) const = 0;
};

// Proxy: intercepta a chamada e verifica permissões
class ProxyPerfil : public IPerfilUsuario {
public:
    void verPerfil(const std::string& visitante) const override {
        if (visitante == bloqueado) {
            std::cout << "[Bloqueado]\n";
            return;
        }
        if (privado && visitante != dono) {
            std::cout << "[Privado]\n";
            return;
        }
        perfilReal.verPerfil(visitante); // delega ao objeto real
    }
};
```

### Como compilar e executar

```bash
cd proxy
g++ -std=c++17 proxy.cpp -o proxy
./proxy
```

### Saída esperada

```
=== Harmonic — Controle de Acesso a Perfis ===

-- @ana tenta ver @carlos (acesso normal) --
  Usuario        : @carlos
  Musica favorita: Bohemian Rhapsody - Queen

-- @spam_user tenta ver @carlos (bloqueado) --
  [Bloqueado] @spam_user nao pode ver este perfil.

-- @pedro tenta ver @julia (perfil privado) --
  [Privado] Este perfil e privado.

-- @julia ve o proprio perfil --
  Usuario        : @julia
  Musica favorita: Blinding Lights - The Weeknd
```

---

## 3. Strategy — Padrão Comportamental

> **Referência:** https://refactoring.guru/pt-br/design-patterns/strategy

### Contexto

O feed do Harmonic exibe músicas avaliadas. O usuário pode escolher como ordenar esse feed: pela **melhor nota** ou pela **mais recente**.

### Problema

Implementar todos os algoritmos de ordenação diretamente na classe `Feed` torna o código difícil de manter. Adicionar um novo critério exigiria modificar a classe, e trocar o algoritmo em tempo de execução seria complicado.

### Solução

O **Strategy** extrai cada algoritmo para sua própria classe (`OrdenarPorNota`, `OrdenarPorAno`), todas implementando a mesma interface (`EstrategiaOrdenacao`). A classe `Feed` (contexto) mantém uma referência à estratégia atual e delega a ordenação a ela, podendo trocá-la livremente em tempo de execução.

### Diagrama

```
EstrategiaOrdenacao  <<interface>>
 + ordenar()
 + nome()
       ▲
  _____|___________
 |                 |
OrdenarPorNota  OrdenarPorAno

Feed (Contexto)
 - estrategia: EstrategiaOrdenacao*
 + setEstrategia()   ← troca em tempo de execução
 + exibir()          → delega para estrategia->ordenar()
```

### Código Explicado

```cpp
// Interface: contrato que todos os algoritmos de ordenação devem seguir
class EstrategiaOrdenacao {
public:
    virtual void ordenar(std::vector<Musica>& musicas) = 0;
    virtual std::string nome() const = 0;
};

// Estratégia concreta: ordena por nota, da maior para a menor
class OrdenarPorNota : public EstrategiaOrdenacao {
public:
    void ordenar(std::vector<Musica>& musicas) override {
        std::sort(musicas.begin(), musicas.end(),
            [](const Musica& a, const Musica& b) { return a.nota > b.nota; });
    }
};

// Contexto: delega a ordenação à estratégia definida
class Feed {
    EstrategiaOrdenacao* estrategia;
public:
    void setEstrategia(EstrategiaOrdenacao* e) { estrategia = e; }
    void exibir() {
        estrategia->ordenar(musicas); // não sabe qual algoritmo está rodando
    }
};
```

### Como compilar e executar

```bash
cd strategy
g++ -std=c++17 strategy.cpp -o strategy
./strategy
```

### Saída esperada

```
=== Harmonic — Feed de Musicas ===

  -- Feed: Melhor Avaliadas --
  Bohemian Rhapsody - Queen (1975) | Nota: 5
  Lose Yourself - Eminem (2002) | Nota: 4.5
  Blinding Lights - The Weeknd (2019) | Nota: 4

  -- Feed: Mais Recentes --
  Blinding Lights - The Weeknd (2019) | Nota: 4
  Lose Yourself - Eminem (2002) | Nota: 4.5
  Bohemian Rhapsody - Queen (1975) | Nota: 5
```

---

## 🗂️ Estrutura do Repositório

```
harmonic-design-patterns/
├── README.md
├── abstract_factory/
│   └── abstract_factory.cpp    # Padrão Criacional
├── proxy/
│   └── proxy.cpp               # Padrão Estrutural
└── strategy/
    └── strategy.cpp            # Padrão Comportamental
```

---

## ⚙️ Como Compilar

```bash
g++ -std=c++17 abstract_factory/abstract_factory.cpp -o abstract_factory/abstract_factory
g++ -std=c++17 proxy/proxy.cpp -o proxy/proxy
g++ -std=c++17 strategy/strategy.cpp -o strategy/strategy
```

Requisito: compilador com suporte a **C++17** (g++ 7+ ou clang++ 5+).

---

## 📖 Referências

- SHVETS, Alexander. **Mergulho nos Padrões de Projeto**. Refactoring.Guru, 2021.
  Disponível em: https://refactoring.guru/pt-br/design-patterns
- **Abstract Factory:** https://refactoring.guru/pt-br/design-patterns/abstract-factory
- **Proxy:** https://refactoring.guru/pt-br/design-patterns/proxy
- **Strategy:** https://refactoring.guru/pt-br/design-patterns/strategy

> O conteúdo conceitual e as definições dos padrões presentes neste README são baseados no catálogo do **Refactoring.Guru** (© Alexander Shvets), referenciado conforme orientação acadêmica. Os exemplos de código e o contexto de aplicação (Harmonic) foram criados originalmente para este repositório com auxílio da LLM **Claude** (Anthropic).
