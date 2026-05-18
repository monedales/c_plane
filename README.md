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

## Explicação cliente

*Em construção*


## Explicação técnica
*Em construção*

### Algoritmo: Sweep Line

A solução utiliza o algoritmo Sweep Line (linha de varredura), que opera em
três etapas:

1. Transformação dos dados: cada passageiro gera dois eventos, uma entrada
   marcada com +1 e uma saída marcada com -1.

2. Ordenação dos eventos por momento. Em caso de empate no momento, saídas
   são processadas antes de entradas, o que implementa a regra especial do
   enunciado.

3. Varredura linear dos eventos ordenados, acumulando uma contagem atual e
   registrando o valor máximo atingido.

## Decisões de design

**Sweep Line sobre Difference Array:** escolhido por ser independente do
range de momentos e reutilizável para escalas maiores.

**strtol sobre atoi:** o `strtol` detecta overflow via `errno` e caracteres
inválidos via `endptr`, tornando a validação do input mais robusta.

**goto cleanup no main:** padrão idiomático em C para centralizar o cleanup de recursos. Garante que `free_room` 
é sempre chamado independente de qual etapa falhou, evitando memory leaks.

**Struct t_event interna:** mantida dentro de `algorithm.c` como detalhe de implementação. Outros módulos não precisam 
conhecer a representação interna do Sweep Line (information hiding).

**TDD:** os testes unitários do algoritmo foram escritos antes da implementação, seguindo o ciclo Red (falha) -> Green (passa) -> Refactor.

### Por que Sweep Line e não Difference Array

O Difference Array criaria um array de 1000 posições e marcaria +1/-1 em cada entrada e saída, varrendo depois linearmente. É mais simples de implementar,
mas depende do range de momentos ser pequeno e fixo. O Sweep Line é independente do range e escala com N, tornando-se a escolha mais robusta para cenários além do enunciado.


## Estrutura do projeto

```
c_plane/
├── src/
│   ├── main.c            orquestra leitura, validação e cálculo
│   ├── input_parser.c    lê e parseia as 3 linhas do stdin
│   ├── validation.c      valida regras de negócio (E[i] < S[i])
│   ├── algorithm.c       implementação do Sweep Line
│   └── utils.c           handle_error e free_room
├── lib/
│   ├── ft_string.c       ft_substr e ft_split da libft pessoal
│   └── ft_memory.c       ft_free_arr da libft pessoal
├── include/
│   └── c_plane.h         tipos, constantes e protótipos
├── tests/
│   ├── test_algorithm.c  testes unitários do algoritmo (TDD)
│   ├── input_100_all_overlap.txt
│   └── input_100_sequential.txt
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
