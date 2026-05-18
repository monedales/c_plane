# Challenge

Solução implementada em C para desafio técnico.

O programa calcula o número máximo de passageiros que estiveram simultaneamente
em uma sala de espera de aeroporto, dado o histórico de entradas e saídas de N
passageiros ao longo de um período de tempo.

## O problema

Dado um número N de passageiros (1 <= N <= 100) que passaram pela sala de espera,
uma lista E com os momentos de entrada de cada passageiro e uma lista S com os
momentos de saída, o programa determina qual foi o pico máximo de ocupação
simultânea da sala.

**Regra especial:** se um passageiro entra no exato momento em que outro sai, o que
saiu já não é contabilizado. Apenas o que entrou conta naquele instante.

## Como compilar

```bash
# Compilar o programa
make

# Limpar objetos
make clean

# Limpar tudo (objetos + binário)
make fclean

# Recompilar do zero
make re

# Rodar os testes unitários
make test
```


## Como usar

O programa lê da entrada padrão (stdin) exatamente 3 linhas:

```
N
E[0] E[1] ... E[N-1]
S[0] S[1] ... S[N-1]
```

Onde:
- N: número de passageiros (1 <= N <= 100)
- E: momentos de entrada separados por espaço (1 <= valor <= 1000)
- S: momentos de saída separados por espaço (1 <= valor <= 1000)

O programa imprime um único inteiro: o número máximo de passageiros
simultâneos na sala.

## Exemplos

**Via pipe:**

```bash
echo "3
1 5 7
9 13 12" | ./c_plane
# Saída: 3

echo "4
1 4 8 10
3 8 10 17" | ./c_plane
# Saída: 1
```

**Via redirecionamento de arquivo:**

```bash
./c_plane < tests/input_100_all_overlap.txt
# Saída: 100

./c_plane < tests/input_100_sequential.txt
# Saída: 1
```

**Via input interativo:**

```bash
./c_plane
3
1 5 7
9 13 12
# Saída: 3
```

## Explicação para o cliente

Imagine o visor digital de vagas de um estacionamento de shopping. Quando um carro entra, o número sobe 1. Quando sai, desce 1. No final do dia, o gerente quer saber uma coisa simples: qual foi o maior número que apareceu no visor?

Para a sala de espera do aeroporto, o problema é exatamente esse. Os registros de entrada e saída de cada passageiro são como os logs da cancela. A solução organiza esses registros em ordem cronológica e os percorre um a um, somando 1 nas entradas e subtraindo 1 nas saídas, anotando o maior valor encontrado ao longo do caminho.

O truque que torna isso eficiente: em vez de verificar a lotação segundo a segundo (como revisar uma câmera de segurança quadro a quadro), o programa só olha os momentos em que algo muda. Se ninguém entra ou sai, a lotação não muda. Por isso ele ignora os momentos "vazios" e pula direto para os eventos relevantes.

Sobre a regra especial: pense numa catraca de metrô. Se uma pessoa passa pela catraca de saída e outra passa pela de entrada exatamente ao mesmo tempo, o sistema registra primeiro a saída e depois a entrada. Elas não ocupam a sala simultaneamente naquele instante. O programa segue essa mesma lógica: em momentos empatados, saídas são sempre processadas antes de entradas.


## Explicação técnica

### Pipeline

O programa opera em três etapas sequenciais orquestradas pelo `main.c`:

```
stdin → read_input → validate_input → max_simultaneous → stdout
```

Qualquer falha em uma etapa interrompe o pipeline, libera a memória alocada via `goto cleanup` e imprime a mensagem de erro correspondente para stderr.

### Leitura e parsing do input

O `input_parser.c` lê três linhas do stdin com `fgets` e um buffer fixo de 1024 bytes, suficiente para o pior caso do enunciado (100 números de 4 dígitos com espaços).

A conversão de string para inteiro usa `strtol` em vez de `atoi` por duas razões: o `strtol` detecta overflow via `errno` e expõe o ponteiro `endptr` para identificar o primeiro caractere não parseado. Isso permite distinguir com precisão entre input não numérico, valor fora do range e lista com tamanho incorreto, gerando mensagens de erro específicas para cada caso.

### Validação de regras de negócio

O `validation.c` verifica que para cada passageiro `i`, o momento de entrada é estritamente menor que o momento de saída (`E[i] < S[i]`). A igualdade `E[i] == S[j]` entre passageiros diferentes é permitida e tratada pelo algoritmo.

O design separa validação de formato (input_parser) de validação de semântica (validation), seguindo o princípio de responsabilidade única.

### Algoritmo: Sweep Line

O `algorithm.c` implementa o Sweep Line em três funções estáticas coordenadas pela função pública `max_simultaneous`:

**Etapa 1: populate_events**
Transforma as duas listas separadas em um array unificado de `N*2` eventos do tipo `t_event { int moment; bool is_entry; }`. Cada passageiro gera dois eventos: entrada (`is_entry = true`) e saída (`is_entry = false`).

A struct `t_event` é declarada internamente em `algorithm.c` e não exposta no header. Outros módulos não precisam conhecer a representação interna do algoritmo (information hiding).

**Etapa 2: compare_events + qsort**
Os eventos são ordenados por momento com `qsort`. A função de comparação `compare_events` resolve empates colocando saídas antes de entradas (`is_entry false = 0` vem antes de `is_entry true = 1`). Isso implementa diretamente a regra do enunciado: se dois eventos ocorrem no mesmo momento, a saída é processada primeiro.

**Etapa 3: sweep_events**
Varredura linear dos eventos ordenados. Incrementa `current` nas entradas e decrementa nas saídas. Atualiza `max` sempre que `current` supera o valor anterior. Retorna `max` ao final.

### Complexidade

- Tempo: O(N log N), dominado pelo `qsort` na etapa de ordenação
- Espaço: O(N), para o array de 2N eventos alocado dinamicamente

Para N = 100 (limite do enunciado), o algoritmo processa em microssegundos. Em escala real com N na casa dos milhares, o Sweep Line continua eficiente pois sua complexidade cresce com N, não com o range de momentos. A alternativa Difference Array teria complexidade O(N + range), menos adequada se o range crescesse.

### Padrões aplicados

- **TDD**: testes unitários escritos antes da implementação, ciclo Red → Green → Refactor
- **Information hiding**: `t_event` encapsulada dentro de `algorithm.c`
- **Fail-fast**: input rejeitado imediatamente com erro específico ao primeiro dado inválido
- **goto cleanup**: único ponto de liberação de memória no `main`, evita memory leaks em qualquer caminho de erro
- **Static functions**: helpers do algoritmo invisíveis fora do módulo

## Estrutura do projeto

```
c_plane/
├── src/
│   ├── main.c            orquestra leitura, validação e cálculo
│   ├── input_parser.c    lê e parseia as 3 linhas do stdin
│   ├── validation.c      valida regras de negócio (E[i] < S[i])
│   ├── algorithm.c       implementação do Sweep Line
│   └── utils.c           handle_error e free_room
├── include/
│   └── c_plane.h         tipos, constantes e protótipos
├── tests/
│   ├── test_algorithm.c          testes unitários do algoritmo (TDD)
│   ├── input_100_all_overlap.txt teste com 100 passageiros sobrepostos
│   └── input_100_sequential.txt  teste com 100 passageiros sequenciais
└── Makefile
```

## Tratamento de erros

O programa valida o input e retorna mensagens de erro descritivas para stderr:

| Erro | Mensagem |
|---|---|
| Falha na leitura | `Error: failed to read input from stdin` |
| N fora do range | `Error: N must be an integer between 1 and 100` |
| Momento fora do range | `Error: moment values must be integers between 1 and 1000` |
| Tamanho incorreto da lista | `Error: list size does not match the number of passengers` |
| Entrada após saída | `Error: a passenger entry must occur before its exit` |
| Caracteres inválidos | `Error: input contains invalid characters. Use plain numbers separated by spaces.` |
| Valor fora do range numérico | `Error: numeric value is out of allowed range` |
| Falha de memória | `Error: memory allocation failed` |

## Testes

### Testes unitários (`make test`)

| Arquivo | Casos cobertos |
|---|---|
| `tests/test_algorithm.c` | Exemplos do enunciado, passageiro único, todos sobrepostos, todos sequenciais, entrada e saída no mesmo momento, 50 e 100 passageiros sobrepostos e sequenciais, pico formado no meio do período |
| `tests/test_algorithm.c` | Validação de E < S, E == S, E > S, room nulo |

### Testes e2e (`make e2e`)

| Categoria | Casos cobertos |
|---|---|
| Exemplos do enunciado | N=3 (max=3), N=4 (max=1) |
| Casos extremos | Passageiro único, momentos mínimos, entrada igual à saída entre passageiros diferentes, 100 sobrepostos, 100 sequenciais |
| Erros de validação | N=0, N=101, E==S, E>S |
| Erros de formato | N alfabético, momento alfabético, caractere especial, overflow numérico, momento acima de 1000, abaixo de 1, lista menor que N, lista maior que N |

### Arquivos de input

| Arquivo | Descrição | Esperado |
|---|---|---|
| `input_100_all_overlap.txt` | 100 passageiros, todos sobrepostos | 100 |
| `input_100_sequential.txt` | 100 passageiros, todos sequenciais | 1 |
| `input_53_all_overlap.txt` | 53 passageiros, todos sobrepostos | 53 |
| `input_69_two_groups.txt` | 69 passageiros em dois grupos que se encontram | 69 |
| `input_81_peak_middle.txt` | 81 passageiros com pico formado no meio do período | 81 |

---

## Uso de IA

Claude (Anthropic) foi utilizado como ferramenta de apoio ao longo do desenvolvimento deste desafio nas seguintes frentes:

- Discussão e comparação de abordagens algorítmicas (Sweep Line, Difference Array, Força Bruta)
- Explicação e aprofundamento de conceitos
- Suporte na geração dos casos de teste unitários e e2e
- Revisão de boas práticas de C e discussão de padrões idiomáticos da linguagem

Todas as decisões de implementação, escolhas de design e o código em si foram desenvolvidos e revisados ativamente durante o processo.
