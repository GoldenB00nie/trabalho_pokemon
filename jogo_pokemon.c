#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>

#define MAX_POKEMON 20              // Número máximo de pokemóns
#define MAX_LENGTH 300              // O número máximo de caracteres em cada linha do status_pokemon e das artes dos pokemons

int pokemonsSelecionadosAliados[3] = {1, 2, 3}, pokemonsSelecionadosInimigos[3] = {4, 5, 6};

typedef struct
{
    char nome_ataque[40];
    int usos_totais;
    char descricao_ataqueL1[70];
    int poder;
    int chance_acertar;
} Ataque;

typedef struct 
{
    int poke_num;                   // variável que armazena o número do pokemon
    char nome[30];                  // variável que armazena o nome do pokemon
    char tipo1[20];                 // variável que armazena o tipo 1 do pokemon
    char tipo2[20];                 // variável que armazena o tipo 2 do pokemon
    int vida_max;                   // variável que armazena a vida do pokemon
    float vida_rest;                // variável que armazena a vida restante do pokemon
    int linha_inicial_aliado;       // variável que armazena a linha inicial do pokemon nas artes ascii para os aliados
    int linha_inicial_inimigo;      // variável que armazena a linha inicial do pokemon nas artes ascii para os inimigos
    int linha_final_aliado;         // variável que armazena a linha final do pokemon nas artes ascii para os aliados
    int linha_final_inimigo;        // variável que armazena a linha final do pokemon nas artes ascii para os inimigos
    char descricao_pokemonL1[70];   // variável que armazena a linha um da descrição do pokémon
    char descricao_pokemonL2[70];   // variável que armazena a linha dois da descrição do pokémon
    char descricao_pokemonL3[70];   // variável que armazena a linha três da descrição do pokémon
    int ataque;                     // variável que armazena o ataque do pokemon
    int defesa;                     // variável que armazena a defesa do pokemon
    int velocidade;                 // variável que armazena a velocidade do pokemon -|- Se a velocidade for maior que a do oponente, você ataca primeiro. -|-
    int especial;                   // variável que armazena o dano do especial do pokemon
    int pokemon_vivo;               // variável que armazena se o pokemon está vivo ou não
    Ataque ataques[4];
} Pokemon;                     // nome da estrutura dos pokemons



Ataque ataques[4];

void lerPokemon(Pokemon *pokemons, int index, char *line) {
    char *token = strtok(line, " ");// A função strtok() lê string1 como uma série de zero ou mais tokens e string2 como o
                                                // conjunto de caracteres que servem como delimitadores dos tokens em string1. 
                                                // No caso, será lido entre o caractere NULL até o espaço. 
    while (token != NULL) {
        switch (token[0]) {
            case '.':                               // poke_num
                pokemons[index].poke_num = atoi(token + 1); //A função atoi() converte uma sequência de caracteres em um valor inteiro.
                break;
            case '!':                               // nome
                strcpy(pokemons[index].nome, token + 1);
                break;
            case '@':                               // tipo 1
                strcpy(pokemons[index].tipo1, token + 1);
                break;
            case '#':                               // tipo 2
                strcpy(pokemons[index].tipo2, token + 1);
                break;
            case '$':                               // vida máxima
                pokemons[index].vida_max = atoi(token + 1) * 4.5;
                break;
            case '%':                               // ataque
                pokemons[index].ataque = atoi(token + 1);
                break;
            case '&':                               // defesa
                pokemons[index].defesa = atoi(token + 1) * 2;
                break;
            case '*':                               // velocidade
                pokemons[index].velocidade = atoi(token + 1);
                break;
            case '(':                               // especial
                pokemons[index].especial = atoi(token + 1);
                break;
        }
        token = strtok(NULL, " ");          
        }
}

void limpar_terminal()                          // Função para limpar o terminal
{
    #if defined(_WIN32) || defined(_WIN64) // verifica se o sistema é windows 32 bits ou 64 bits
        system("cls"); // windows
    #else
        system("clear"); // linux e mac
    #endif
}

void exibirPokemon(Pokemon *p, int index)       // Exibe os dados do pokémons (Não usado)
{
    printf("Pokemon %d:\n", index); 
    printf("Nome: %s\n", p->nome); // As setas servem para acessar membros de uma estrutura quando se tem um ponteiro para a estrutura
    printf("Tipo 1: %s\n", p->tipo1);
    printf("Tipo 2: %s\n", p->tipo2);
    printf("Vida: %d\n", p->vida_max);
    printf("Ataque: %d\n", p->ataque);
    printf("Defesa: %d\n", p->defesa);
    printf("Velocidade: %d\n", p->velocidade);
    printf("Especial: %d\n", p->especial);
    printf("\n");
}

int obterIndiceAleatorio(int max, int *indicesEscolhidos, int tamanho)
{
    int indice;
    int repetido;
    do {
        indice = rand() % max; // rand escolhe um número aleatório
        
        repetido = 0;
        for (int i = 0; i < tamanho; i++) 
            {
                if (indicesEscolhidos[i] == indice) 
                    {
                        repetido = 1;
                        break;
                    }
            }
    } while (repetido);
    return indice;
}

void exibirNomesPokemons(Pokemon *p, int index) // para o menu de configurações dos pokemons
{
    printf("%2d: %-15s", index, p->nome); // os numeros antes das letras servem para alinhar em algum lugar. - é para esquerda, + para direita
}

int leitura_pokemon(short int nomesPokemonsConfig, char opcao_config_equipe_selecionada, int *pokemonsSelecionadosAliados, int *pokemonsSelecionadosInimigos, int res_config_poke_aleat) 
{
    FILE *file;
    char line[MAX_LENGTH];
    Pokemon pokemons[MAX_POKEMON];
    short int pokemon_count = 0;

    file = fopen("C:\\Users\\GUI\\OneDrive\\Documentos\\gui\\Projetos VSC\\trabalho_pokemon\\status_pokemon", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    // Lê cada linha do arquivo e armazena em uma estrutura Pokemon
    while (fgets(line, MAX_LENGTH, file) != NULL && pokemon_count < MAX_POKEMON) { //A função fgets() lê caracteres da posição atual 
        lerPokemon(pokemons, pokemon_count, line);                      // do fluxo até e incluindo o primeiro caractere de nova linha (\n), até o final 
        pokemon_count++;                                                // do fluxo ou até que o número de caracteres lidos seja igual a n-1, o que ocorrer primeiro
    }

    fclose(file); // fecha o arquivo e usa o tempo para determinar os pokemons aleatorios
    
    if (nomesPokemonsConfig == 0 || nomesPokemonsConfig == 2) 
    {
        srand(time(NULL));
        int indicesAliados[3];
        int indicesInimigos[3];

        for (int i = 1; i < 4; i++)
            {
                indicesAliados[i] = obterIndiceAleatorio(pokemon_count, indicesAliados, i);
                indicesInimigos[i] = obterIndiceAleatorio(pokemon_count, indicesInimigos, i);
                while (indicesAliados[i] == 0)
                    indicesAliados[i] = obterIndiceAleatorio(pokemon_count, indicesAliados, i);
                while (indicesInimigos[i] == 0)
                    indicesInimigos[i] = obterIndiceAleatorio(pokemon_count, indicesInimigos, i);
            }

        // Seleciona os pokémons para o time aliado com verificação de repetição
        for (int i = 1; i < 4; i++) 
        {
            if (res_config_poke_aleat == 1)
                pokemonsSelecionadosAliados[i-1] = indicesAliados[i];
            if (nomesPokemonsConfig == 2)
                printf("%d ", indicesAliados[i]); // Dev
        }

        // Seleciona os pokémons para o time inimigo com verificação de repetição
        for (int i = 1; i < 4; i++) 
        {
            if (res_config_poke_aleat == 1)
                pokemonsSelecionadosInimigos[i-1] = indicesInimigos[i];
            if (nomesPokemonsConfig == 2)
                printf("  \t%d ", indicesInimigos[i]); // Dev
        }
        if (nomesPokemonsConfig == 2)
            putchar('\n');
    }

    if (nomesPokemonsConfig == 1) 
    {
        for (int i = 0; i < pokemon_count; i++) 
        { // i = variável controle
            printf("\t\t");
            
            if (opcao_config_equipe_selecionada == 'j' && (i + 1 == pokemonsSelecionadosAliados[0] || i + 1 == pokemonsSelecionadosAliados[1] || i + 1 == pokemonsSelecionadosAliados[2])) 
                printf("> ");
                
            if (opcao_config_equipe_selecionada == 'c' && (i + 1 == pokemonsSelecionadosInimigos[0] || i + 1 == pokemonsSelecionadosInimigos[1] || i + 1 == pokemonsSelecionadosInimigos[2]))
                printf("> ");
            
            exibirNomesPokemons(&pokemons[i], i + 1);

            if (i + 1 == 4 || i + 1 == 8 || i + 1 == 12 || i + 1 == 16)
                putchar('\n');
        }
        
        printf("\n\n\t\t\t> Voltar (V)\t\t\t> Help (H)\t\t\t> Opção: ");
    }
    return 0;
}

void ler_arte_ascii(const char *caminho_arquivo, int linha_inicial, int linha_final) 
{
    FILE *file;
    char line[MAX_LENGTH];
    int linha_atual = 0;

    file = fopen(caminho_arquivo, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    while (fgets(line, MAX_LENGTH, file) != NULL) {
        linha_atual++;
        if (linha_atual < linha_inicial) {
            continue; // Pula as linhas até a linha inicial
        }
        if (linha_atual > linha_final) {
            break; // Para se passar da linha final
        }
        printf("%s", line);
    }

    fclose(file);
}

const char *tipos[14] = 
{    "Normal", "Fogo", "água", "Elétrico", "Planta", "Lutador", "Veneno", "Terrestre", "Voador", "Psíquico", "Pedra", "Dragão", "Aço", "Fada" };

float tabelaEfetiva[14][14] = {
    // NOR  FIR  WAT  ELE  GRA  ICE  FIG  POI  GRO  FLY  PSY  BUG  ROC  GHO  DRA  DAR  STE  FAI
    { 1,     1,   1,   1,   1,        1,   1,   1,   1,   1,       0.5,       1,       0.5,  1  }, // NOR
    { 1,    0.5, 0.5,  1,   2,        1,   1,   1,   1,   1,       0.5,      0.5,       2,   1  }, // FIR
    { 1,     2,  0.5,  1,  0.5,       1,   1,   2,   1,   1,        2,       0.5,       1,   1  }, // WAT
    { 1,     1,   2,  0.5, 0.5,       1,   1,   0,   2,   1,        1,       0.5,      0.5,  1  }, // ELE
    { 1,    0.5,  2,   1,  0.5,       1,  0.5,  2,  0.5,  1,        2,       0.5,      0.5,  1  }, // GRA
    { 2,     1,   1,   1,   1,        1,  0.5,  1,  0.5, 0.5,       2,        1,        2,  0.5 }, // FIG
    { 1,     1,   1,   1,   2,        1,  0.5, 0.5,  1,   1,       0.5,       1,        0,   2  }, // POI
    { 1,     2,   1,   2,  0.5,       1,   2,   1,   0,   1,        2,        1,        2,   1  }, // GRO
    { 1,     1,   1,  0.5,  2,        2,   1,   1,   1,   1,       0.5,       1,       0.5,  1  }, // FLY
    { 1,     1,   1,   1,   1,        2,   2,   1,   1,  0.5,       1,        1,       0.5,  1  }, // PSY
    { 1,     2,   1,   1,   1,       0.5,  1,  0.5,  2,   1,        1,        1,       0.5,  1  }, // ROC
    { 1,     1,   1,   1,   1,        1,   1,   1,   1,   1,        1,        2,       0.5,  0  }, // DRA
    { 1,    0.5, 0.5,  1,   1,        1,   1,   1,   1,   1,        2,        1,       0.5,  2  }, // STE
    { 1,    0.5,  1,   1,   1,        1,  0.5,  1,   1,   1,        1,        2,       0.5,  1  }  // FAI
};

int get_type_index(char *tipo) {
    for (int i = 0; i < 14; i++) {
        if (strcmp(tipo, tipos[i]) == 0) {
            return i;
        }
    }
    return -1; // Tipo não encontrado
}

float Calcular_tiposEfetivos(char *Tipo1Atacante, char *Tipo2Atacante, char *Tipo1Alvo, char *Tipo2Alvo) {
    float multiplier = 1.0;

    int attackIndex1 = get_type_index(Tipo1Atacante);
    int attackIndex2 = get_type_index(Tipo2Atacante);
    int defenseIndex1 = get_type_index(Tipo1Alvo);
    int defenseIndex2 = get_type_index(Tipo2Alvo);

    if (attackIndex1 != -1 && defenseIndex1 != -1) {
        multiplier *= tabelaEfetiva[attackIndex1][defenseIndex1];
    }
    if (attackIndex1 != -1 && defenseIndex2 != -1) {
        multiplier *= tabelaEfetiva[attackIndex1][defenseIndex2];
    }
    if (attackIndex2 != -1 && defenseIndex1 != -1) {
        multiplier *= tabelaEfetiva[attackIndex2][defenseIndex1];
    }
    if (attackIndex2 != -1 && defenseIndex2 != -1) {
        multiplier *= tabelaEfetiva[attackIndex2][defenseIndex2];
    }

    return multiplier;
}

float calcular_dano(int AtaqueBase, int PoderAtaque, int ChanceAcerto, char Tipo1Atacante[20], char Tipo2Atacante[20], char Tipo1Alvo[20], char Tipo2Alvo[20], int ChanceCritico, int defesaAlvo)
{
    int res_ChanceAcerto, num_aleat = rand() % 100;
    float res_ChanceCritico;
    if (num_aleat < ChanceAcerto)
        res_ChanceAcerto = 1;
    else
        res_ChanceAcerto = 0;
    
    num_aleat = rand() % 100;
    if (num_aleat < ChanceCritico)
        res_ChanceCritico = 1.35;
    else
        res_ChanceCritico = 1;
    
    float min = 0.75; float max = 1.1;
    float VariacaoAleatoria = min + ((float)rand() / RAND_MAX) * (max - min);

    float res_Efetivo = Calcular_tiposEfetivos(Tipo1Atacante, Tipo2Atacante, Tipo1Alvo, Tipo2Alvo);

    float danoTotal = (AtaqueBase * PoderAtaque * VariacaoAleatoria * res_ChanceAcerto * res_Efetivo *res_ChanceCritico) / (defesaAlvo + 15);
    
    // printf("%d\t%.2f\t%.2f\t%.2f", res_ChanceAcerto, VariacaoAleatoria, res_Efetivo, res_ChanceCritico); - dev

    return danoTotal;
}



int main()
{

    Pokemon pokemons_aliados[100], pokemons_inimigos[100]; // Supondo um máximo de 100 Pokémons
    char line[256];
    int index = 0;

    FILE *file = fopen("C:\\Users\\GUI\\OneDrive\\Documentos\\gui\\Projetos VSC\\trabalho_pokemon\\status_pokemon", "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '\0') continue; // Ignora linhas vazias
        lerPokemon(pokemons_aliados, index, line);
        lerPokemon(pokemons_inimigos, index, line);
        index++;
    }

    // Escrever linhas iniciais e finais de cada pokemon nas artes
    pokemons_aliados[0].linha_inicial_aliado = 19; pokemons_aliados[0].linha_final_aliado = 33; pokemons_aliados[0].linha_inicial_inimigo = 338; pokemons_aliados[0].linha_final_inimigo = 352;
    pokemons_aliados[1].linha_inicial_aliado = 19; pokemons_aliados[1].linha_final_aliado = 33; pokemons_aliados[1].linha_inicial_inimigo = 338; pokemons_aliados[1].linha_final_inimigo = 352;
    pokemons_aliados[2].linha_inicial_aliado = pokemons_aliados[1].linha_final_aliado + 3; pokemons_aliados[2].linha_final_aliado = 50; pokemons_aliados[2].linha_inicial_inimigo = pokemons_aliados[1].linha_final_inimigo + 3; pokemons_aliados[2].linha_final_inimigo = pokemons_aliados[1].linha_final_aliado + 319;
    pokemons_aliados[3].linha_inicial_aliado = pokemons_aliados[2].linha_final_aliado + 3; pokemons_aliados[3].linha_final_aliado = 65; pokemons_aliados[3].linha_inicial_inimigo = pokemons_aliados[2].linha_final_inimigo + 3; pokemons_aliados[3].linha_final_inimigo = pokemons_aliados[2].linha_final_aliado + 319;
    pokemons_aliados[4].linha_inicial_aliado = pokemons_aliados[3].linha_final_aliado + 3; pokemons_aliados[4].linha_final_aliado = 82; pokemons_aliados[4].linha_inicial_inimigo = pokemons_aliados[3].linha_final_inimigo + 3; pokemons_aliados[4].linha_final_inimigo = pokemons_aliados[3].linha_final_aliado + 319;
    pokemons_aliados[5].linha_inicial_aliado = pokemons_aliados[4].linha_final_aliado + 3; pokemons_aliados[5].linha_final_aliado = 99; pokemons_aliados[5].linha_inicial_inimigo = pokemons_aliados[4].linha_final_inimigo + 3; pokemons_aliados[5].linha_final_inimigo = pokemons_aliados[4].linha_final_aliado + 319;
    pokemons_aliados[6].linha_inicial_aliado = pokemons_aliados[5].linha_final_aliado + 3; pokemons_aliados[6].linha_final_aliado = 116; pokemons_aliados[6].linha_inicial_inimigo = pokemons_aliados[5].linha_final_inimigo + 3; pokemons_aliados[6].linha_final_inimigo = pokemons_aliados[5].linha_final_aliado + 319;
    pokemons_aliados[7].linha_inicial_aliado = pokemons_aliados[6].linha_final_aliado + 3; pokemons_aliados[7].linha_final_aliado = 129; pokemons_aliados[7].linha_inicial_inimigo = pokemons_aliados[6].linha_final_inimigo + 3; pokemons_aliados[7].linha_final_inimigo = pokemons_aliados[6].linha_final_aliado + 319;
    pokemons_aliados[8].linha_inicial_aliado = pokemons_aliados[7].linha_final_aliado + 3; pokemons_aliados[8].linha_final_aliado = 151; pokemons_aliados[8].linha_inicial_inimigo = pokemons_aliados[7].linha_final_inimigo + 3; pokemons_aliados[8].linha_final_inimigo = pokemons_aliados[7].linha_final_aliado + 319;
    pokemons_aliados[9].linha_inicial_aliado = pokemons_aliados[8].linha_final_aliado + 3; pokemons_aliados[9].linha_final_aliado = 168; pokemons_aliados[9].linha_inicial_inimigo = pokemons_aliados[8].linha_final_inimigo + 3; pokemons_aliados[9].linha_final_inimigo = pokemons_aliados[8].linha_final_aliado + 319;
    pokemons_aliados[10].linha_inicial_aliado = pokemons_aliados[9].linha_final_aliado + 3; pokemons_aliados[10].linha_final_aliado = 185; pokemons_aliados[10].linha_inicial_inimigo = pokemons_aliados[9].linha_final_inimigo + 3; pokemons_aliados[10].linha_final_inimigo = pokemons_aliados[9].linha_final_aliado + 319;
    pokemons_aliados[11].linha_inicial_aliado = pokemons_aliados[10].linha_final_aliado + 3; pokemons_aliados[11].linha_final_aliado = 198; pokemons_aliados[11].linha_inicial_inimigo = pokemons_aliados[10].linha_final_inimigo + 3; pokemons_aliados[11].linha_final_inimigo = pokemons_aliados[10].linha_final_aliado + 319;
    pokemons_aliados[12].linha_inicial_aliado = pokemons_aliados[11].linha_final_aliado + 3; pokemons_aliados[12].linha_final_aliado = 211; pokemons_aliados[12].linha_inicial_inimigo = pokemons_aliados[11].linha_final_inimigo + 3; pokemons_aliados[12].linha_final_inimigo = pokemons_aliados[11].linha_final_aliado + 319;
    pokemons_aliados[13].linha_inicial_aliado = pokemons_aliados[12].linha_final_aliado + 3; pokemons_aliados[13].linha_final_aliado = 228; pokemons_aliados[13].linha_inicial_inimigo = pokemons_aliados[12].linha_final_inimigo + 3; pokemons_aliados[13].linha_final_inimigo = pokemons_aliados[12].linha_final_aliado + 319;
    pokemons_aliados[14].linha_inicial_aliado = pokemons_aliados[13].linha_final_aliado + 3; pokemons_aliados[14].linha_final_aliado = 236; pokemons_aliados[14].linha_inicial_inimigo = pokemons_aliados[13].linha_final_inimigo + 3; pokemons_aliados[14].linha_final_inimigo = pokemons_aliados[13].linha_final_aliado + 319;
    pokemons_aliados[15].linha_inicial_aliado = pokemons_aliados[14].linha_final_aliado + 3; pokemons_aliados[15].linha_final_aliado = 247; pokemons_aliados[15].linha_inicial_inimigo = pokemons_aliados[14].linha_final_inimigo + 3; pokemons_aliados[15].linha_final_inimigo = pokemons_aliados[14].linha_final_aliado + 319;
    pokemons_aliados[16].linha_inicial_aliado = pokemons_aliados[15].linha_final_aliado + 3; pokemons_aliados[16].linha_final_aliado = 264; pokemons_aliados[16].linha_inicial_inimigo = pokemons_aliados[15].linha_final_inimigo + 3; pokemons_aliados[16].linha_final_inimigo = pokemons_aliados[15].linha_final_aliado + 319;
    pokemons_aliados[17].linha_inicial_aliado = pokemons_aliados[16].linha_final_aliado + 3; pokemons_aliados[17].linha_final_aliado = 281; pokemons_aliados[17].linha_inicial_inimigo = pokemons_aliados[16].linha_final_inimigo + 3; pokemons_aliados[17].linha_final_inimigo = pokemons_aliados[16].linha_final_aliado + 319;
    pokemons_aliados[18].linha_inicial_aliado = pokemons_aliados[17].linha_final_aliado + 3; pokemons_aliados[18].linha_final_aliado = 296; pokemons_aliados[18].linha_inicial_inimigo = pokemons_aliados[17].linha_final_inimigo + 3; pokemons_aliados[18].linha_final_inimigo = pokemons_aliados[17].linha_final_aliado + 319;
    pokemons_aliados[19].linha_inicial_aliado = pokemons_aliados[18].linha_final_aliado + 3; pokemons_aliados[19].linha_final_aliado = 311; pokemons_aliados[19].linha_inicial_inimigo = pokemons_aliados[18].linha_final_inimigo + 3; pokemons_aliados[19].linha_final_inimigo = pokemons_aliados[18].linha_final_aliado + 319;
    pokemons_aliados[20].linha_inicial_aliado = pokemons_aliados[19].linha_final_aliado + 3; pokemons_aliados[20].linha_final_aliado = 328; pokemons_aliados[20].linha_inicial_inimigo = pokemons_aliados[19].linha_final_inimigo + 3; pokemons_aliados[20].linha_final_inimigo = pokemons_aliados[19].linha_final_aliado + 319;
    pokemons_aliados[21].linha_inicial_aliado = pokemons_aliados[20].linha_final_aliado + 3; pokemons_aliados[21].linha_final_aliado = 328; pokemons_aliados[21].linha_inicial_inimigo = pokemons_aliados[20].linha_final_inimigo + 3; pokemons_aliados[21].linha_final_inimigo = pokemons_aliados[20].linha_final_aliado + 319;

    // Atribuir as descrições de cada pokémon
    for (int i = 0; i < 20; i++)
        {
            strcpy(pokemons_aliados[i].descricao_pokemonL1, " ");
            strcpy(pokemons_aliados[i].descricao_pokemonL2, " ");
        }
    strcpy(pokemons_aliados[1-1].descricao_pokemonL1, "    Quando ameaçado, Pikachu irá imediatamente descarregar\t ⎹"); strcpy(pokemons_aliados[1-1].descricao_pokemonL2, "\t     a energia armazenada nas suas bochechas.\t\t ⎹"); strcpy(pokemons_aliados[1-1].descricao_pokemonL3, "\t\t\t\t\t\t\t\t ⎹");
    strcpy(pokemons_aliados[2-1].descricao_pokemonL1, "    A chama na sua cauda mostra a força da sua energia vital.\t ⎹"); strcpy(pokemons_aliados[2-1].descricao_pokemonL2, "\t Se Charmander estiver fraco, sua chama também ficará\t ⎹"); strcpy(pokemons_aliados[2-1].descricao_pokemonL3, "\t\t\t     mais fraca.\t\t\t ⎹");
    strcpy(pokemons_aliados[3-1].descricao_pokemonL1, "   Por algum tempo após o seu nascimento, Bulbassauro utiliza\t ⎹"); strcpy(pokemons_aliados[3-1].descricao_pokemonL2, "\t os nutrientes contidos na semente em suas costas\t ⎹"); strcpy(pokemons_aliados[3-1].descricao_pokemonL3, "\t\t\t    para crescer.\t\t\t ⎹");
    strcpy(pokemons_aliados[4-1].descricao_pokemonL1, " Após o nascimento de Squirtle, suas costas incham e endurecem,⎹"); strcpy(pokemons_aliados[4-1].descricao_pokemonL2, "  formando uma concha. Ele consegue soltar uma espuma potente  ⎹"); strcpy(pokemons_aliados[4-1].descricao_pokemonL3, "\t\t\t     pela boca.\t\t\t\t ⎹");
    strcpy(pokemons_aliados[5-1].descricao_pokemonL1, "   Nas noites de lua cheia, Clefairy's se reúnem em todos os \t ⎹"); strcpy(pokemons_aliados[5-1].descricao_pokemonL2, "   lugares e dançam. Tomar banho ao luar faz elas flutuarem.\t ⎹"); strcpy(pokemons_aliados[5-1].descricao_pokemonL3, "\t\t\t\t\t\t\t\t ⎹");
    strcpy(pokemons_aliados[6-1].descricao_pokemonL1, "  Inepto para voar alto. No entanto, Spearow pode voar muito \t ⎹"); strcpy(pokemons_aliados[6-1].descricao_pokemonL2, "\t       rápido para proteger o seu território. \t\t ⎹"); strcpy(pokemons_aliados[6-1].descricao_pokemonL3, "\t\t\t\t\t\t\t\t ⎹");
    strcpy(pokemons_aliados[7-1].descricao_pokemonL1, "   Mankey vive em grupos nas copas das árvores. Se perder de \t ⎹"); strcpy(pokemons_aliados[7-1].descricao_pokemonL2, "\t  vista seu grupo, fica enfurecido com sua solidão. \t ⎹"); strcpy(pokemons_aliados[7-1].descricao_pokemonL3, "\t\t\t\t\t\t\t\t ⎹");
    strcpy(pokemons_aliados[8-1].descricao_pokemonL1, "   Tudo o que Meowth faz é dormir durante o dia. À noite, \t ⎹"); strcpy(pokemons_aliados[8-1].descricao_pokemonL2, "\t   patrulha seu território com os olhos brilhando. \t ⎹"); strcpy(pokemons_aliados[8-1].descricao_pokemonL3, "\t\t\t\t\t\t\t\t ⎹");
    strcpy(pokemons_aliados[9-1].descricao_pokemonL1, "  Psyduck é constantemente atormentado por uma dor de cabeça.\t ⎹"); strcpy(pokemons_aliados[9-1].descricao_pokemonL2, "   Quando a dor de cabeça se intensifica, ele começa a usar \t ⎹"); strcpy(pokemons_aliados[9-1].descricao_pokemonL3, "\t\t\t poderes misteriosos\t\t\t ⎹");
    strcpy(pokemons_aliados[10-1].descricao_pokemonL1, "    Ekans pode soltar livremente sua mandíbula para engolir\t ⎹"); strcpy(pokemons_aliados[10-1].descricao_pokemonL2, "  presas grandes inteiras. No entanto, pode ficar muito pesado ⎹"); strcpy(pokemons_aliados[10-1].descricao_pokemonL3, "\t\t\t    para se mover.\t\t\t ⎹");
    strcpy(pokemons_aliados[11-1].descricao_pokemonL1, "\t O redemoinho em sua barriga é seus órgãos internos \t ⎹"); strcpy(pokemons_aliados[11-1].descricao_pokemonL2, "  aparecendo através da pele. Ele fica mais visível depois que ⎹"); strcpy(pokemons_aliados[11-1].descricao_pokemonL3, "\t\t\t    Poliwag come.\t\t\t ⎹");
    strcpy(pokemons_aliados[12-1].descricao_pokemonL1, "    Este Pokémon usa seus poderes psíquicos enquanto dorme. \t ⎹"); strcpy(pokemons_aliados[12-1].descricao_pokemonL2, "  O conteúdo dos sonhos de Abra afeta os poderes que o Pokémon ⎹"); strcpy(pokemons_aliados[12-1].descricao_pokemonL3, "\t\t\t        possui.\t\t\t\t ⎹");
    strcpy(pokemons_aliados[13-1].descricao_pokemonL1, "  Todo o corpo de Machop é composto por músculos. Mesmo sendo  ⎹"); strcpy(pokemons_aliados[13-1].descricao_pokemonL2, "   do tamanho de uma criança humana, ele pode arremessar 100 \t ⎹"); strcpy(pokemons_aliados[13-1].descricao_pokemonL3, "\t\t\t      adultos.\t\t\t\t ⎹");
    strcpy(pokemons_aliados[14-1].descricao_pokemonL1, "\t Em repouso, Geodude parece apenas uma pedra. Pisar \t ⎹"); strcpy(pokemons_aliados[14-1].descricao_pokemonL2, "  descuidadosamente nele fará com que balance seus punhos com  ⎹"); strcpy(pokemons_aliados[14-1].descricao_pokemonL3, "\t\t\t\traiva.\t\t\t\t ⎹");
    strcpy(pokemons_aliados[15-1].descricao_pokemonL1, "\tAs ondas eletromagnéticas emitidas pelas unidades nas \t ⎹"); strcpy(pokemons_aliados[15-1].descricao_pokemonL2, " nas laterais da cabeça de Magnemite expulsam a antigravidade, ⎹"); strcpy(pokemons_aliados[15-1].descricao_pokemonL3, "\t\t    o que permite que ele flutue.\t\t ⎹");
    strcpy(pokemons_aliados[16-1].descricao_pokemonL1, "    Quando a memória de sua mãe falecida faz Cubone chorar, \t ⎹"); strcpy(pokemons_aliados[16-1].descricao_pokemonL2, "    seus gritos ecoam de forma melancólica dentro do crânio \t ⎹"); strcpy(pokemons_aliados[16-1].descricao_pokemonL3, "\t\t      que ele carrega na cabeça\t\t\t ⎹");
    strcpy(pokemons_aliados[17-1].descricao_pokemonL1, "\t  Ele muda várias camadas de pele à medida que cresce. \t ⎹"); strcpy(pokemons_aliados[17-1].descricao_pokemonL2, "    Durante esse processo, Dratini é protegido por uma cascata ⎹"); strcpy(pokemons_aliados[17-1].descricao_pokemonL3, "\t\t\t       rápida.\t\t\t\t ⎹");
    strcpy(pokemons_aliados[18-1].descricao_pokemonL1, "\tA habilidade de transformação de Ditto é perfeita. No \t ⎹"); strcpy(pokemons_aliados[18-1].descricao_pokemonL2, "  entanto, se for feito rir, não consegue manter seu disfarce. ⎹"); strcpy(pokemons_aliados[18-1].descricao_pokemonL3, "\t\t\t\t\t\t\t\t ⎹");
    strcpy(pokemons_aliados[19-1].descricao_pokemonL1, "  Se Vulpix for atacada por um inimigo mais forte do que ela,  ⎹"); strcpy(pokemons_aliados[19-1].descricao_pokemonL2, "\t finge-se de ferida para enganar o inimigo e escapar. \t ⎹"); strcpy(pokemons_aliados[19-1].descricao_pokemonL3, "\t\t\t\t\t\t\t\t ⎹");
    strcpy(pokemons_aliados[20-1].descricao_pokemonL1, "    Não importa o que Bellsprout esteja fazendo, se detectar \t ⎹"); strcpy(pokemons_aliados[20-1].descricao_pokemonL2, "      movimento nas proximidades, ele reagirá imediatamente \t ⎹"); strcpy(pokemons_aliados[20-1].descricao_pokemonL3, "\t\t    estendendo suas finas vinhas \t\t ⎹");
    

    fclose(file);

    /****************************************************** Menu Principal ************************************************************/

    short int menu_principal = 1, config_poke_aleat = 1, res_config_poke_aleat = config_poke_aleat % 2;
    char opcao, opcao_config;

    for (int i = 0; i < 3; i++)
        {
            pokemons_aliados[pokemonsSelecionadosAliados[i-1]].pokemon_vivo = 1;
            pokemons_inimigos[pokemonsSelecionadosInimigos[i-1]].pokemon_vivo = 1;
        }
    
    // int pokemonsSelecionadosAliados[3] = {1, 2, 3}, pokemonsSelecionadosInimigos[3] = {4, 5, 6}; - movido para o topo
    // Variável menu principal: Modifica qual menu irá aparecer. 0 = sair, 1 = principal, 2 = confirmação jogo,
    //                                                           3 = config, 4 = config pokemons, 5 = jogo principal;

    do
    {   
        limpar_terminal();
        SetConsoleOutputCP(CP_UTF8); // Configura o terminal para UTF-8
        const char *caminho_arquivo = "C:\\Users\\GUI\\OneDrive\\Documentos\\gui\\Projetos VSC\\trabalho_pokemon\\arte_pokemon"; 
        // Caminho para o arquivo contendo a arte ASCII
        int dev = 0;
        

                    /********************************************\
                     *              Página inicial              *
                    \********************************************/
                    
        while(menu_principal == 1)
            {
                ler_arte_ascii(caminho_arquivo, 2, 16); // Chamando a função para ler e exibir a arte ASCII
                printf("\n\t\t\t\t\t\t\t\t> Jogar (J)\n\t\t\t\t\t\t\t\t> Configurações (C)\n\t\t\t\t\t\t\t\t> Sair (S)\n\n\t\t\t\t\t\t\t\t> Opção: ");
                scanf(" %c", &opcao);
                opcao = tolower(opcao);
                switch (opcao)
                {    
                    case 'j': limpar_terminal(); printf("\7"); menu_principal = 2; break;
                    case 'c': limpar_terminal(); printf("\7"); menu_principal = 3; break;
                    case 's': limpar_terminal(); printf("\7\n\t\t\t\t\tTenha uma boa jornada e nos vemos na próxima, aventureiro!\n\n"); menu_principal = 0; break;
                    case '$': limpar_terminal(); leitura_pokemon(2, ' ', pokemonsSelecionadosAliados, pokemonsSelecionadosInimigos, ' '); dev = 1; break;
                    // $ - Função para dev
                    default: limpar_terminal(); printf("\7\nOpção inválida!!!\n");
                }
                fflush(stdin);
            }
        
                    /*******************************************\
                     *              Configurações              *
                    \*******************************************/
        
        while (menu_principal == 3)
            {
                printf("\n\n\n\n\t\t\t\t\t/************* Configurações ************\\\n\n");
                if (res_config_poke_aleat == 1)
                    printf("\t\t\t\t\t\t\t> Pokémons aleatórios (A): ON\n\t\t\t\t\t\t\t> Voltar (V)\n\n\t\t\t\t\t\t\t> Opção: ");
                else
                    printf("\t\t\t\t\t\t\t> Pokémons aleatórios (A): OFF\n\t\t\t\t\t\t\t\t> Configurações dos Pokémons (P)\n\t\t\t\t\t\t\t> Voltar (V)\n\n\t\t\t\t\t\t\t> Opção: ");
                scanf(" %c", &opcao_config);
                opcao_config = tolower(opcao_config);
                switch (opcao_config)
                    {
                        case 'v': limpar_terminal(); printf("\7"); menu_principal = 1; break;
                        case 'a': limpar_terminal(); printf("\7"); config_poke_aleat = config_poke_aleat + 1; break;
                        case 'p': if (res_config_poke_aleat == 0)
                                        {
                                            limpar_terminal(); printf("\7");
                                            menu_principal = 4;
                                        }
                                    else
                                        {limpar_terminal(); printf("\7\nDesabilite os Pokémons aleatórios para usar essa configuração");}
                                    break;
                        default: limpar_terminal(); printf("7\nOpção inválida!!!"); break;
                    }
                fflush(stdin);
                res_config_poke_aleat = config_poke_aleat % 2;
            }
            
                    /*******************************************\
                     *       Configurações dos pokemons        *
                    \*******************************************/                
            
                char opcao_config_equipe_selecionada = 'j', escolher_numouchar[1]; // tinha que ser escrito fora do loop
                short int selecao = 0, selecao_num_pokemon = 0;

        while (menu_principal == 4)
            {
                if (dev == 1)
                    {

                        printf("\n\n\t%s (vida: %d / %d)\n", pokemons_aliados[pokemonsSelecionadosAliados[1-1] -1].nome, pokemons_aliados[pokemonsSelecionadosInimigos[1-1] -1].vida_max, pokemons_aliados[pokemonsSelecionadosInimigos[1-1] -1].vida_max);
                        
                        ler_arte_ascii(caminho_arquivo, pokemons_aliados[pokemonsSelecionadosInimigos[1-1] +1].linha_inicial_inimigo, pokemons_aliados[pokemonsSelecionadosInimigos[1-1] + 1].linha_final_inimigo);
                        //printf("%d\n", pokemons[pokemonsSelecionadosInimigos[1-1] +1]);
                        printf("%d %d %d\n", pokemonsSelecionadosInimigos[1-1], pokemons_aliados[pokemonsSelecionadosInimigos[1-1]].linha_inicial_inimigo, pokemons_aliados[pokemonsSelecionadosInimigos[1-1]].linha_final_inimigo);
                        ler_arte_ascii(caminho_arquivo, pokemons_aliados[pokemonsSelecionadosAliados[1-1]].linha_inicial_aliado, pokemons_aliados[pokemonsSelecionadosAliados[1-1]].linha_final_aliado);
                    }
                if (opcao_config_equipe_selecionada == 'j' || opcao_config_equipe_selecionada == 'J')
                    {
                        printf("\n\nEscolha a equipe que deseja modificar. \nCaso queira, selecione o Pokémon escolhido indicado por (>) e depois o número do Pokémon \nque deseja para trocar o Pokémon escolhido. ");
                        printf("Se precisar, acesse o menu \"Help\" para mostrar mais informações.");
                        printf("\n\n\n\n\t\t\t\t> Equipe do Jogador (J)\t\t\t\tEquipe do Computador (C)\n\n");
                        leitura_pokemon(1, 'j', pokemonsSelecionadosAliados, pokemonsSelecionadosInimigos, ' ');
                    }
                if (opcao_config_equipe_selecionada == 'c' || opcao_config_equipe_selecionada == 'C')
                    {
                        printf("\n\nEscolha a equipe que deseja modificar. \nCaso queira, selecione o Pokémon escolhido indicado por (>) e depois o número do Pokémon \nque deseja para trocar o Pokémon escolhido. ");
                        printf("Se precisar, acesse o menu \"Help\" para mostrar mais informações.");
                        printf("\n\n\n\n\t\t\t\tEquipe do Jogador (J)\t\t\t\t> Equipe do Computador (C)\n\n");
                        leitura_pokemon(1, 'c', pokemonsSelecionadosAliados, pokemonsSelecionadosInimigos, ' ');
                    }
                scanf("%c", &escolher_numouchar[0]);
                if (isdigit(escolher_numouchar[0]))
                    {
                        if(escolher_numouchar[0] < '1' || escolher_numouchar[0] > '3')
                            {limpar_terminal(); printf("\nVocê só pode selecionar três Pokémons, 1 até 3.");}
                        else
                            {
                                selecao = atoi(escolher_numouchar);
                                if (opcao_config_equipe_selecionada == 'j' || opcao_config_equipe_selecionada == 'J')
                                    printf("\n\nVocê selecionou o pokémon %s. Insira o número do Pokémon em que deseja trocar: ", pokemons_aliados[pokemonsSelecionadosAliados[selecao -1] -1].nome);
                                if (opcao_config_equipe_selecionada == 'c' || opcao_config_equipe_selecionada == 'C')
                                    printf("\n\nVocê selecionou o pokémon %s. Insira o número do Pokémon em que deseja trocar: ", pokemons_aliados[pokemonsSelecionadosInimigos[selecao -1] -1].nome);
                                while (1)
                                    {
                                        scanf("%hd", &selecao_num_pokemon);
                                        if (selecao_num_pokemon < 1 || selecao_num_pokemon > 20)
                                            printf("\nPokémon não existe. Tente novamente: ");
                                        else
                                            {
                                                if (opcao_config_equipe_selecionada == 'j' || opcao_config_equipe_selecionada == 'J')
                                                    pokemonsSelecionadosAliados[selecao - 1] = selecao_num_pokemon;
                                                else if (opcao_config_equipe_selecionada == 'c' || opcao_config_equipe_selecionada == 'C')
                                                    pokemonsSelecionadosInimigos[selecao - 1] = selecao_num_pokemon;
                                                limpar_terminal();
                                                printf("Seleção atualizada: %d -> %d\n", selecao, selecao_num_pokemon); // Ver depois
                                                break;
                                            }  
                                    } 
                            }
                    }
                else
                    {
                        fflush(stdin);
                        switch (escolher_numouchar[0])
                            {
                                case 'v': case 'V': limpar_terminal(); printf("\7"); menu_principal = 3; break;
                                case 'h': case 'H': 
                                    limpar_terminal();
                                    printf("\n\n\n\n\n\n\n\tVocê pode Escolher uma equipe que deseja modificar ao escolher (C) para a equipe do computador, ou (J) para a equipe do jogador."); 
                                    printf("\n\t\t\t\t\tA equipe que tiver o (>) antes será a que você estará modificando.");
                                    printf("\n\t\t\t    Em cada equipe você pode escolher 3 Pokémons no máximo e eles estão representados pelas (>).");
                                    printf("\n\t\tPara alterar, selecione um Pokémon com (>) pelo número da seta (>) e coloque o número do Pokémon em que deseja trocar.");
                                    printf("\n\n\t\tAviso: as setas (>) podem se acumular e não serão exibidos individualmente, mas elas não se fundem umas com as outras.");
                                    printf("\n\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t> Ok <ENTER>");
                                    getchar();
                                    limpar_terminal(); printf("\7");
                                    break;
                                case 'j': case 'J': limpar_terminal(); printf("\7"); opcao_config_equipe_selecionada = 'j'; break;
                                case 'c': case 'C': limpar_terminal(); printf("\7"); opcao_config_equipe_selecionada = 'c'; break;
                                default: limpar_terminal(); printf("\7\nNúmero ou caractere inválido!!!");
                            }
                    }
                fflush(stdin);
            }
            
            /******************************************************* Confirmar Jogo *************************************************************/

                        char opcao_confirmacao_jogo;
                        
            while (menu_principal == 2)
                {
                    fflush(stdin);
                    printf("\n\n\n\t\t\t\t\t\t\t    Começar partida?");
                    if (res_config_poke_aleat == 0)
                        {
                            printf("\n\n\n\n\t\t\t\tOs Pokémons Aliados selecionados são: ");
                            printf("%s, %s e %s ", pokemons_aliados[pokemonsSelecionadosAliados[0] -1].nome, pokemons_aliados[pokemonsSelecionadosAliados[1] -1].nome, pokemons_aliados[pokemonsSelecionadosAliados[2] -1].nome);
                            printf("\n\n\t\t\t\tE os Pokémons Inimigos selecionados são: ");
                            printf("%s, %s e %s ", pokemons_aliados[pokemonsSelecionadosInimigos[0] -1].nome, pokemons_aliados[pokemonsSelecionadosInimigos[1] -1].nome, pokemons_aliados[pokemonsSelecionadosInimigos[2] -1].nome);
                        }
                    else
                        printf("\n\n\t\t\t\t\t\tOs Pokémons de cada time serão aleatórios.");
                    printf("\n\n\n\n\t\t\t\t> Sim (S)\t\t\t> Não (N)\t\t\tOpção: ");
                    opcao_confirmacao_jogo = getchar(); getchar();
                    opcao_confirmacao_jogo = tolower(opcao_confirmacao_jogo);
                    switch (opcao_confirmacao_jogo)
                        {
                            case 's': limpar_terminal(); if(res_config_poke_aleat == 1 && dev == 1) leitura_pokemon(2, 'j', pokemonsSelecionadosAliados, pokemonsSelecionadosInimigos, 1); 
                                                         else if(res_config_poke_aleat == 1 && dev == 0) leitura_pokemon(0, 'j', pokemonsSelecionadosAliados, pokemonsSelecionadosInimigos, 1);
                                                         menu_principal = 5; printf("\7"); break;
                            case 'n': limpar_terminal(); menu_principal = 1; printf("\7"); break;
                            default: limpar_terminal(); printf("\7Caractere inválido."); break;
                        }
                }

            /******************************************************* Jogo Principal *************************************************************/
            
            short int menu_jogo = 0, pokemonSelecionadoAliado_jogo = 1, pokemonSelecionadoInimigo_jogo = 1;
            pokemons_aliados[pokemonsSelecionadosAliados[1-1]-1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[1-1]-1].vida_max;
            pokemons_aliados[pokemonsSelecionadosAliados[2-1]-1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[2-1]-1].vida_max;
            pokemons_aliados[pokemonsSelecionadosAliados[3-1]-1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[3-1]-1].vida_max;
            pokemons_aliados[pokemonsSelecionadosInimigos[1-1]-1].vida_rest = pokemons_aliados[pokemonsSelecionadosInimigos[1-1]-1].vida_max;
            pokemons_aliados[pokemonsSelecionadosInimigos[2-1]-1].vida_rest = pokemons_aliados[pokemonsSelecionadosInimigos[2-1]-1].vida_max;
            pokemons_aliados[pokemonsSelecionadosInimigos[3-1]-1].vida_rest = pokemons_aliados[pokemonsSelecionadosInimigos[3-1]-1].vida_max;
            
            strcpy(pokemons_aliados[1-1].ataques[1-1].nome_ataque, "ᶠᶦˢ\t\t      Ataque Rápido"); strcpy(pokemons_aliados[1-1].ataques[2-1].nome_ataque, "ᶠᶦˢ\t\t\tInvestida"); strcpy(pokemons_aliados[1-1].ataques[3-1].nome_ataque, "ˢᵗᵃ\t\t     Onda de Choque"); strcpy(pokemons_aliados[1-1].ataques[4-1].nome_ataque, "ˢᵖᵉ\t\t\t   Raio ");
                strcpy(pokemons_aliados[1-1].ataques[1-1].descricao_ataqueL1, "Ataque rápido que atinge primeiro.");
                strcpy(pokemons_aliados[1-1].ataques[2-1].descricao_ataqueL1, "Ataque que nunca erra.");
                strcpy(pokemons_aliados[1-1].ataques[3-1].descricao_ataqueL1, "Paralisa o alvo.");
                strcpy(pokemons_aliados[1-1].ataques[4-1].descricao_ataqueL1, "Poderoso ataque elétrico com chance de paralisar o alvo.");
                    pokemons_aliados[1-1].ataques[1-1].usos_totais = 30;        pokemons_aliados[1-1].ataques[2-1].usos_totais = 20;        pokemons_aliados[1-1].ataques[3-1].usos_totais = 20;        pokemons_aliados[1-1].ataques[4-1].usos_totais = 10;
                    pokemons_aliados[1-1].ataques[1-1].poder = 40;              pokemons_aliados[1-1].ataques[2-1].poder = 60;              pokemons_aliados[1-1].ataques[3-1].poder = 0;               pokemons_aliados[1-1].ataques[4-1].poder = 120;
                    pokemons_aliados[1-1].ataques[1-1].chance_acertar = 100;    pokemons_aliados[1-1].ataques[2-1].chance_acertar = 100;    pokemons_aliados[1-1].ataques[3-1].chance_acertar = 100;    pokemons_aliados[1-1].ataques[4-1].chance_acertar = 70;
            strcpy(pokemons_aliados[2-1].ataques[1-1].nome_ataque, "ᶠᶦˢ\t\t\t Arranhar"); strcpy(pokemons_aliados[2-1].ataques[2-1].nome_ataque, "ˢᵗᵃ\t\t    Olhar malicioso"); strcpy(pokemons_aliados[2-1].ataques[3-1].nome_ataque, "ˢᵖᵉ\t\t       Lança-chamas"); strcpy(pokemons_aliados[2-1].ataques[4-1].nome_ataque, "ˢᵖᵉ\t\t    Rotação de fogo");
                strcpy(pokemons_aliados[2-1].ataques[1-1].descricao_ataqueL1, "Arranha o inimigo com garras.");
                strcpy(pokemons_aliados[2-1].ataques[2-1].descricao_ataqueL1, "Reduz a defesa do oponente.");
                strcpy(pokemons_aliados[2-1].ataques[3-1].descricao_ataqueL1, "Lança uma rajada de fogo.");
                strcpy(pokemons_aliados[2-1].ataques[4-1].descricao_ataqueL1, "Envolve o inimigo em um vórtice de fogo."); 
                    pokemons_aliados[2-1].ataques[1-1].usos_totais = 35;        pokemons_aliados[2-1].ataques[2-1].usos_totais = 30;        pokemons_aliados[2-1].ataques[3-1].usos_totais = 15;        pokemons_aliados[2-1].ataques[4-1].usos_totais = 15;
                    pokemons_aliados[2-1].ataques[1-1].poder = 40;              pokemons_aliados[2-1].ataques[2-1].poder = 0;               pokemons_aliados[2-1].ataques[3-1].poder = 95;              pokemons_aliados[2-1].ataques[4-1].poder = 15;
                    pokemons_aliados[2-1].ataques[1-1].chance_acertar = 100;    pokemons_aliados[2-1].ataques[2-1].chance_acertar = 100;    pokemons_aliados[2-1].ataques[3-1].chance_acertar = 100;    pokemons_aliados[2-1].ataques[4-1].chance_acertar = 70;
            strcpy(pokemons_aliados[3-1].ataques[1-1].nome_ataque, "ᶠᶦˢ\t\t\t Atacar"); strcpy(pokemons_aliados[3-1].ataques[2-1].nome_ataque, "ˢᵗᵃ\t\t      Crescimento"); strcpy(pokemons_aliados[3-1].ataques[3-1].nome_ataque, "ˢᵗᵃ\t\t      Pó para dormir"); strcpy(pokemons_aliados[3-1].ataques[4-1].nome_ataque, "ˢᵖᵉ\t\t      Feixe solar");
                strcpy(pokemons_aliados[3-1].ataques[1-1].descricao_ataqueL1, "Investida física.");
                strcpy(pokemons_aliados[3-1].ataques[2-1].descricao_ataqueL1, "Aumenta o dano do especial.");
                strcpy(pokemons_aliados[3-1].ataques[3-1].descricao_ataqueL1, "Coloca o oponente para dormir.");
                strcpy(pokemons_aliados[3-1].ataques[4-1].descricao_ataqueL1, "Poderoso raio solar que leva um turno para carregar.");
                    pokemons_aliados[3-1].ataques[1-1].usos_totais = 35;        pokemons_aliados[3-1].ataques[2-1].usos_totais = 20;        pokemons_aliados[3-1].ataques[3-1].usos_totais = 15;        pokemons_aliados[3-1].ataques[4-1].usos_totais = 10;
                    pokemons_aliados[3-1].ataques[1-1].poder = 40;              pokemons_aliados[3-1].ataques[2-1].poder = 0;               pokemons_aliados[3-1].ataques[3-1].poder = 0;               pokemons_aliados[3-1].ataques[4-1].poder = 120;
                    pokemons_aliados[3-1].ataques[1-1].chance_acertar = 100;    pokemons_aliados[3-1].ataques[2-1].chance_acertar = 100;    pokemons_aliados[3-1].ataques[3-1].chance_acertar = 75;     pokemons_aliados[3-1].ataques[4-1].chance_acertar = 100;
            strcpy(pokemons_aliados[4-1].ataques[1-1].nome_ataque, "ᶠᶦˢ\t\t\t  Atacar"); strcpy(pokemons_aliados[4-1].ataques[2-1].nome_ataque, "ᶠᶦˢ\t\t     Giro rápido"); strcpy(pokemons_aliados[4-1].ataques[3-1].nome_ataque, "ˢᵗᵃ\t\t         Proteger"); strcpy(pokemons_aliados[4-1].ataques[4-1].nome_ataque, "ˢᵖᵉ\t\t     Hidro-bomba");
                strcpy(pokemons_aliados[4-1].ataques[1-1].descricao_ataqueL1, "Investida física.");
                strcpy(pokemons_aliados[4-1].ataques[2-1].descricao_ataqueL1, "Ataque giratório que remove efeitos negativos.");
                strcpy(pokemons_aliados[4-1].ataques[3-1].descricao_ataqueL1, "Protege contra todos os ataques por um turno.");
                strcpy(pokemons_aliados[4-1].ataques[4-1].descricao_ataqueL1, "Jato de água de alta pressão.");
                    pokemons_aliados[4-1].ataques[1-1].usos_totais = 35;        pokemons_aliados[4-1].ataques[2-1].usos_totais = 40;        pokemons_aliados[4-1].ataques[3-1].usos_totais = 10;        pokemons_aliados[4-1].ataques[4-1].usos_totais = 5;
                    pokemons_aliados[4-1].ataques[1-1].poder = 40;              pokemons_aliados[4-1].ataques[2-1].poder = 50;              pokemons_aliados[4-1].ataques[3-1].poder = 0;               pokemons_aliados[4-1].ataques[4-1].poder = 110;
                    pokemons_aliados[4-1].ataques[1-1].chance_acertar = 100;    pokemons_aliados[4-1].ataques[2-1].chance_acertar = 100;    pokemons_aliados[4-1].ataques[3-1].chance_acertar = 100;    pokemons_aliados[4-1].ataques[4-1].chance_acertar = 80;
            strcpy(pokemons_aliados[5-1].ataques[1-1].nome_ataque, "ˢᵗᵃ\t\t\t  Charme"); strcpy(pokemons_aliados[5-1].ataques[2-1].nome_ataque, "ˢᵗᵃ\t\t    Beijo amoroso"); strcpy(pokemons_aliados[5-1].ataques[3-1].nome_ataque, "ˢᵗᵃ\t\t       Noite ao luar"); strcpy(pokemons_aliados[5-1].ataques[4-1].nome_ataque, "ˢᵖᵉ\t\t    Explosão lunar");
                strcpy(pokemons_aliados[5-1].ataques[1-1].descricao_ataqueL1, "Reduz o ataque do oponente.");
                strcpy(pokemons_aliados[5-1].ataques[2-1].descricao_ataqueL1, "Confunde o oponente.");
                strcpy(pokemons_aliados[5-1].ataques[3-1].descricao_ataqueL1, "Restaura HP aleatóriamente.");
                strcpy(pokemons_aliados[5-1].ataques[4-1].descricao_ataqueL1, "Ataque poderoso do tipo fada.");
                    pokemons_aliados[5-1].ataques[1-1].usos_totais = 20;        pokemons_aliados[5-1].ataques[2-1].usos_totais = 10;        pokemons_aliados[5-1].ataques[3-1].usos_totais = 5;         pokemons_aliados[5-1].ataques[4-1].usos_totais = 15;
                    pokemons_aliados[5-1].ataques[1-1].poder = 0;               pokemons_aliados[5-1].ataques[2-1].poder = 0;               pokemons_aliados[5-1].ataques[3-1].poder = 0;               pokemons_aliados[5-1].ataques[4-1].poder = 95;
                    pokemons_aliados[5-1].ataques[1-1].chance_acertar = 100;    pokemons_aliados[5-1].ataques[2-1].chance_acertar = 75;     pokemons_aliados[5-1].ataques[3-1].chance_acertar = 100;    pokemons_aliados[5-1].ataques[4-1].chance_acertar = 100;
            strcpy(pokemons_aliados[6-1].ataques[1-1].nome_ataque, "ᶠᶦˢ\t\t\t  Bicada"); strcpy(pokemons_aliados[6-1].ataques[2-1].nome_ataque, "ᶠᶦˢ\t\t       Ás aéreo"); strcpy(pokemons_aliados[6-1].ataques[3-1].nome_ataque, "ˢᵗᵃ\t\t       Focar energia"); strcpy(pokemons_aliados[6-1].ataques[4-1].nome_ataque, "ˢᵖᵉ\t\t     Rugido aéreo");
                strcpy(pokemons_aliados[6-1].ataques[1-1].descricao_ataqueL1, "É uma bicada.");
                strcpy(pokemons_aliados[6-1].ataques[2-1].descricao_ataqueL1, "Ataque que nunca erra.");
                strcpy(pokemons_aliados[6-1].ataques[3-1].descricao_ataqueL1, "Aumenta a taxa de críticos.");
                strcpy(pokemons_aliados[6-1].ataques[4-1].descricao_ataqueL1, "Ataque contínuo que impede o sono.");
                    pokemons_aliados[6-1].ataques[1-1].usos_totais = 35;        pokemons_aliados[6-1].ataques[2-1].usos_totais = 20;        pokemons_aliados[6-1].ataques[3-1].usos_totais = 30;        pokemons_aliados[6-1].ataques[4-1].usos_totais = 10;
                    pokemons_aliados[6-1].ataques[1-1].poder = 35;              pokemons_aliados[6-1].ataques[2-1].poder = 60;              pokemons_aliados[6-1].ataques[3-1].poder = 0;               pokemons_aliados[6-1].ataques[4-1].poder = 90;
                    pokemons_aliados[6-1].ataques[1-1].chance_acertar = 100;    pokemons_aliados[6-1].ataques[2-1].chance_acertar = 100;    pokemons_aliados[6-1].ataques[3-1].chance_acertar = 100;    pokemons_aliados[6-1].ataques[4-1].chance_acertar = 70;
            strcpy(pokemons_aliados[7-1].ataques[1-1].nome_ataque, "ᶠᶦˢ\t\t\t  Cobiçar"); strcpy(pokemons_aliados[7-1].ataques[2-1].nome_ataque, "ᶠᶦˢ\t\t Luta corpo a corpo"); strcpy(pokemons_aliados[7-1].ataques[3-1].nome_ataque, "ˢᵗᵃ\t\t       Focar energia"); strcpy(pokemons_aliados[7-1].ataques[4-1].nome_ataque, "ˢᵖᵉ\t\t    Jogada final");
                strcpy(pokemons_aliados[7-1].ataques[1-1].descricao_ataqueL1, "Rouba o item do oponente.");
                strcpy(pokemons_aliados[7-1].ataques[2-1].descricao_ataqueL1, "Ataque poderoso que reduz a defesa.");
                strcpy(pokemons_aliados[7-1].ataques[3-1].descricao_ataqueL1, "Aumenta a taxa de críticos.");
                strcpy(pokemons_aliados[7-1].ataques[4-1].descricao_ataqueL1, "Causa dano igual ao HP do usuário.");
                    pokemons_aliados[7-1].ataques[1-1].usos_totais = 25;        pokemons_aliados[7-1].ataques[2-1].usos_totais = 5;         pokemons_aliados[7-1].ataques[3-1].usos_totais = 30;        pokemons_aliados[7-1].ataques[4-1].usos_totais = 5;
                    pokemons_aliados[7-1].ataques[1-1].poder = 60;              pokemons_aliados[7-1].ataques[2-1].poder = 120;             pokemons_aliados[7-1].ataques[3-1].poder = 0;               pokemons_aliados[7-1].ataques[4-1].poder = 100000;
                    pokemons_aliados[7-1].ataques[1-1].chance_acertar = 100;    pokemons_aliados[7-1].ataques[2-1].chance_acertar = 100;    pokemons_aliados[7-1].ataques[3-1].chance_acertar = 100;    pokemons_aliados[7-1].ataques[4-1].chance_acertar = 40;
            strcpy(pokemons_aliados[8-1].ataques[1-1].nome_ataque, "ᶠᶦˢ\t\t\t Arranhar"); strcpy(pokemons_aliados[8-1].ataques[2-1].nome_ataque, "ˢᵗᵃ\t\t        Rosnar"); strcpy(pokemons_aliados[8-1].ataques[3-1].nome_ataque, "ˢᵗᵃ\t\t\t  Berrar"); strcpy(pokemons_aliados[8-1].ataques[4-1].nome_ataque, "ᶠᶦˢ\t\t   Golpes de fúria");
                strcpy(pokemons_aliados[8-1].ataques[1-1].descricao_ataqueL1, "Arranha o inimigo com garras.");
                strcpy(pokemons_aliados[8-1].ataques[2-1].descricao_ataqueL1, "Reduz o ataque do oponente.");
                strcpy(pokemons_aliados[8-1].ataques[3-1].descricao_ataqueL1, "Reduz a defesa do oponente.");
                strcpy(pokemons_aliados[8-1].ataques[4-1].descricao_ataqueL1, "Ataque múltiplo com garras.");
                    pokemons_aliados[8-1].ataques[1-1].usos_totais = 35;        pokemons_aliados[8-1].ataques[2-1].usos_totais = 40;        pokemons_aliados[8-1].ataques[3-1].usos_totais = 40;        pokemons_aliados[8-1].ataques[4-1].usos_totais = 15;
                    pokemons_aliados[8-1].ataques[1-1].poder = 40;              pokemons_aliados[8-1].ataques[2-1].poder = 0;               pokemons_aliados[8-1].ataques[3-1].poder = 0;               pokemons_aliados[8-1].ataques[4-1].poder = 18;
                    pokemons_aliados[8-1].ataques[1-1].chance_acertar = 100;    pokemons_aliados[8-1].ataques[2-1].chance_acertar = 100;    pokemons_aliados[8-1].ataques[3-1].chance_acertar = 85;     pokemons_aliados[8-1].ataques[4-1].chance_acertar = 80;
            strcpy(pokemons_aliados[9-1].ataques[1-1].nome_ataque, "ᶠᶦˢ\t\t\t Arranhar"); strcpy(pokemons_aliados[9-1].ataques[2-1].nome_ataque, "ˢᵗᵃ\t\t     Desabilitar"); strcpy(pokemons_aliados[9-1].ataques[3-1].nome_ataque, "ˢᵗᵃ\t\t  Quarto maravilhoso"); strcpy(pokemons_aliados[9-1].ataques[4-1].nome_ataque, "ˢᵖᵉ\t\t     Hidro-bomba");
                strcpy(pokemons_aliados[9-1].ataques[1-1].descricao_ataqueL1, "Arranha o inimigo com garras.");
                strcpy(pokemons_aliados[9-1].ataques[2-1].descricao_ataqueL1, "Impede o uso do último movimento do oponente.");
                strcpy(pokemons_aliados[9-1].ataques[3-1].descricao_ataqueL1, "Inverte a defesa e o ataque por 5 turnos.");
                strcpy(pokemons_aliados[9-1].ataques[4-1].descricao_ataqueL1, "Pulso de água com chance de confundir.");
                    pokemons_aliados[9-1].ataques[1-1].usos_totais = 35;        pokemons_aliados[9-1].ataques[2-1].usos_totais = 20;        pokemons_aliados[9-1].ataques[3-1].usos_totais = 10;        pokemons_aliados[9-1].ataques[4-1].usos_totais = 5;
                    pokemons_aliados[9-1].ataques[1-1].poder = 40;              pokemons_aliados[9-1].ataques[2-1].poder = 0;               pokemons_aliados[9-1].ataques[3-1].poder = 0;               pokemons_aliados[9-1].ataques[4-1].poder = 110;
                    pokemons_aliados[9-1].ataques[1-1].chance_acertar = 100;    pokemons_aliados[9-1].ataques[2-1].chance_acertar = 100;    pokemons_aliados[9-1].ataques[3-1].chance_acertar = 100;    pokemons_aliados[9-1].ataques[4-1].chance_acertar = 80;
            strcpy(pokemons_aliados[10-1].ataques[1-1].nome_ataque, "ᶠᶦˢ\t\t\t Morder"); strcpy(pokemons_aliados[10-1].ataques[2-1].nome_ataque, "ᶠᶦˢ\t\t  Picada venenosa"); strcpy(pokemons_aliados[10-1].ataques[3-1].nome_ataque, "ˢᵗᵃ\t\t       Gastroácido"); strcpy(pokemons_aliados[10-1].ataques[4-1].nome_ataque, "ˢᵖᵉ\t\t  Vômito venenoso");
                strcpy(pokemons_aliados[10-1].ataques[1-1].descricao_ataqueL1, "Mordida com chance de causar flinch.");
                strcpy(pokemons_aliados[10-1].ataques[2-1].descricao_ataqueL1, "Ataque venenoso com chance de envenenar.");
                strcpy(pokemons_aliados[10-1].ataques[3-1].descricao_ataqueL1, "Remove a habilidade do oponente");
                strcpy(pokemons_aliados[10-1].ataques[4-1].descricao_ataqueL1, "Ataque poderoso que requer uma baga consumida.");
                    pokemons_aliados[10-1].ataques[1-1].usos_totais = 25;       pokemons_aliados[10-1].ataques[2-1].usos_totais = 35;       pokemons_aliados[10-1].ataques[3-1].usos_totais = 10;       pokemons_aliados[10-1].ataques[4-1].usos_totais = 10;
                    pokemons_aliados[10-1].ataques[1-1].poder = 60;             pokemons_aliados[10-1].ataques[2-1].poder = 15;             pokemons_aliados[10-1].ataques[3-1].poder = 0;             pokemons_aliados[10-1].ataques[4-1].poder = 120;
                    pokemons_aliados[10-1].ataques[1-1].chance_acertar = 100;   pokemons_aliados[10-1].ataques[2-1].chance_acertar = 100;   pokemons_aliados[10-1].ataques[3-1].chance_acertar = 100;   pokemons_aliados[10-1].ataques[4-1].chance_acertar = 90;
            strcpy(pokemons_aliados[11-1].ataques[1-1].nome_ataque, "ᶠᶦˢ\t\t\tEsmagar"); strcpy(pokemons_aliados[11-1].ataques[2-1].nome_ataque, "ᶠᶦˢ\t\t Pancada corporal"); strcpy(pokemons_aliados[11-1].ataques[3-1].nome_ataque, "ˢᵗᵃ\t\t    Tambor Abdominal"); strcpy(pokemons_aliados[11-1].ataques[4-1].nome_ataque, "ˢᵖᵉ\t\t     Hidro-bomba");
                strcpy(pokemons_aliados[11-1].ataques[1-1].descricao_ataqueL1, "Ataque físico simples.");
                strcpy(pokemons_aliados[11-1].ataques[2-1].descricao_ataqueL1, "Ataque com chance de paralisar.");
                strcpy(pokemons_aliados[11-1].ataques[3-1].descricao_ataqueL1, "Maximiza o ataque ao custo de HP.");
                strcpy(pokemons_aliados[11-1].ataques[4-1].descricao_ataqueL1, "Jato de água de alta pressão.");
                    pokemons_aliados[11-1].ataques[1-1].usos_totais = 35;       pokemons_aliados[11-1].ataques[2-1].usos_totais = 15;       pokemons_aliados[11-1].ataques[3-1].usos_totais = 10;       pokemons_aliados[11-1].ataques[4-1].usos_totais = 5;
                    pokemons_aliados[11-1].ataques[1-1].poder = 40;             pokemons_aliados[11-1].ataques[2-1].poder = 85;             pokemons_aliados[11-1].ataques[3-1].poder = 0;              pokemons_aliados[11-1].ataques[4-1].poder = 110;
                    pokemons_aliados[11-1].ataques[1-1].chance_acertar = 100;   pokemons_aliados[11-1].ataques[2-1].chance_acertar = 100;   pokemons_aliados[11-1].ataques[3-1].chance_acertar = 100;   pokemons_aliados[11-1].ataques[4-1].chance_acertar = 80;
            strcpy(pokemons_aliados[12-1].ataques[1-1].nome_ataque, "ˢᵗᵃ\t\t\t   Bis"); strcpy(pokemons_aliados[12-1].ataques[2-1].nome_ataque, "ˢᵗᵃ\t\t     Capa mágica"); strcpy(pokemons_aliados[12-1].ataques[3-1].nome_ataque, "ˢᵗᵃ\t\t    Mudança psíquica"); strcpy(pokemons_aliados[12-1].ataques[4-1].nome_ataque, "ˢᵖᵉ\t\t       Confusão");
                strcpy(pokemons_aliados[12-1].ataques[1-1].descricao_ataqueL1, "Faz o oponente repetir o último movimento por alguns turnos.");
                strcpy(pokemons_aliados[12-1].ataques[2-1].descricao_ataqueL1, "Reflete efeitos de status.");
                strcpy(pokemons_aliados[12-1].ataques[3-1].descricao_ataqueL1, "Transfere efeitos de status para o alvo.");
                strcpy(pokemons_aliados[12-1].ataques[4-1].descricao_ataqueL1, "Ataque psíquico com chance de confundir.");
                    pokemons_aliados[12-1].ataques[1-1].usos_totais = 5;        pokemons_aliados[12-1].ataques[2-1].usos_totais = 15;       pokemons_aliados[12-1].ataques[3-1].usos_totais = 10;       pokemons_aliados[12-1].ataques[4-1].usos_totais = 25;
                    pokemons_aliados[12-1].ataques[1-1].poder = 0;              pokemons_aliados[12-1].ataques[2-1].poder = 0;              pokemons_aliados[12-1].ataques[3-1].poder = 0;              pokemons_aliados[12-1].ataques[4-1].poder = 50;
                    pokemons_aliados[12-1].ataques[1-1].chance_acertar = 100;   pokemons_aliados[12-1].ataques[2-1].chance_acertar = 100;   pokemons_aliados[12-1].ataques[3-1].chance_acertar = 100;   pokemons_aliados[12-1].ataques[4-1].chance_acertar = 100;
            strcpy(pokemons_aliados[13-1].ataques[1-1].nome_ataque, "ᶠᶦˢ\t\t\t Vingança"); strcpy(pokemons_aliados[13-1].ataques[2-1].nome_ataque, "ᶠᶦˢ\t\t    Soco dinâmico"); strcpy(pokemons_aliados[13-1].ataques[3-1].nome_ataque, "ˢᵗᵃ\t\t         Aumentar"); strcpy(pokemons_aliados[13-1].ataques[4-1].nome_ataque, "ᶠᶦˢ\t\t      Dois gumes");
                strcpy(pokemons_aliados[13-1].ataques[1-1].descricao_ataqueL1, "Dobra o poder se o usuário foi atingido no mesmo turno.");
                strcpy(pokemons_aliados[13-1].ataques[2-1].descricao_ataqueL1, "Ataque que causa confusão, mas com baixa precisão.");
                strcpy(pokemons_aliados[13-1].ataques[3-1].descricao_ataqueL1, "Aumenta o ataque e a defesa.");
                strcpy(pokemons_aliados[13-1].ataques[4-1].descricao_ataqueL1, "Ataque poderoso que causa dano de recuo.");
                    pokemons_aliados[13-1].ataques[1-1].usos_totais = 10;       pokemons_aliados[13-1].ataques[2-1].usos_totais = 5;        pokemons_aliados[13-1].ataques[3-1].usos_totais = 20;       pokemons_aliados[13-1].ataques[4-1].usos_totais = 15;
                    pokemons_aliados[13-1].ataques[1-1].poder = 60;             pokemons_aliados[13-1].ataques[2-1].poder = 100;            pokemons_aliados[13-1].ataques[3-1].poder = 0;              pokemons_aliados[13-1].ataques[4-1].poder = 120;
                    pokemons_aliados[13-1].ataques[1-1].chance_acertar = 100;   pokemons_aliados[13-1].ataques[2-1].chance_acertar = 50;    pokemons_aliados[13-1].ataques[3-1].chance_acertar = 100;   pokemons_aliados[13-1].ataques[4-1].chance_acertar = 100;
            strcpy(pokemons_aliados[14-1].ataques[1-1].nome_ataque, "ᶠᶦˢ\t\t     Tumba de pedra"); strcpy(pokemons_aliados[14-1].ataques[2-1].nome_ataque, "ᶠᶦˢ\t\t\t\tMalho"); strcpy(pokemons_aliados[14-1].ataques[3-1].nome_ataque, "ˢᵗᵃ\t\t        Bloquear"); strcpy(pokemons_aliados[14-1].ataques[4-1].nome_ataque, "ˢᵖᵉ\t\t     Tapa de lama");
                strcpy(pokemons_aliados[14-1].ataques[1-1].descricao_ataqueL1, "Ataque de rocha que reduz a velocidade do oponente.");
                strcpy(pokemons_aliados[14-1].ataques[2-1].descricao_ataqueL1, "Mais forte quanto menor for o HP do usuário.");
                strcpy(pokemons_aliados[14-1].ataques[3-1].descricao_ataqueL1, "Impede o oponente de ser trocado.");
                strcpy(pokemons_aliados[14-1].ataques[4-1].descricao_ataqueL1, "Ataque de lama que reduz a precisão.");
                    pokemons_aliados[14-1].ataques[1-1].usos_totais = 15;       pokemons_aliados[14-1].ataques[2-1].usos_totais = 15;       pokemons_aliados[14-1].ataques[3-1].usos_totais = 5;        pokemons_aliados[14-1].ataques[4-1].usos_totais = 10;
                    pokemons_aliados[14-1].ataques[1-1].poder = 60;             if (pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo - 1]].vida_rest > 60) {pokemons_aliados[14-1].ataques[2-1].poder = 50;} else {pokemons_aliados[14-1].ataques[2-1].poder = 50;}     pokemons_aliados[14-1].ataques[3-1].poder = 0;              pokemons_aliados[14-1].ataques[4-1].poder = 20;
                    pokemons_aliados[14-1].ataques[1-1].chance_acertar = 95;    pokemons_aliados[14-1].ataques[2-1].chance_acertar = 100;   pokemons_aliados[14-1].ataques[3-1].chance_acertar = 100;   pokemons_aliados[14-1].ataques[4-1].chance_acertar = 100;
            strcpy(pokemons_aliados[15-1].ataques[1-1].nome_ataque, "ᶠᶦˢ\t\t\t  Atacar"); strcpy(pokemons_aliados[15-1].ataques[2-1].nome_ataque, "ᶠᶦˢ\t\t  Bola giroscópica"); strcpy(pokemons_aliados[15-1].ataques[3-1].nome_ataque, "ˢᵗᵃ\t\t      Som metálico"); strcpy(pokemons_aliados[15-1].ataques[4-1].nome_ataque, "ˢᵖᵉ\t\t  Canhão elétrico");
                strcpy(pokemons_aliados[15-1].ataques[1-1].descricao_ataqueL1, "Investida física.");
                strcpy(pokemons_aliados[15-1].ataques[2-1].descricao_ataqueL1, "Ataque que aumenta o poder quanto mais lento for o usuário.");
                strcpy(pokemons_aliados[15-1].ataques[3-1].descricao_ataqueL1, "Reduz a defesa do oponente.");
                strcpy(pokemons_aliados[15-1].ataques[4-1].descricao_ataqueL1, "Ataque elétrico poderoso com chance de paralisar.");
                    pokemons_aliados[15-1].ataques[1-1].usos_totais = 35;       pokemons_aliados[15-1].ataques[2-1].usos_totais = 5;        pokemons_aliados[15-1].ataques[3-1].usos_totais = 40;       pokemons_aliados[15-1].ataques[4-1].usos_totais = 5;
                    pokemons_aliados[15-1].ataques[1-1].poder = 40;             pokemons_aliados[15-1].ataques[2-1].poder = 25 * pokemons_aliados[15-1].velocidade / pokemons_aliados[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].velocidade;              pokemons_aliados[15-1].ataques[3-1].poder = 0;              pokemons_aliados[15-1].ataques[4-1].poder = 120;
                    pokemons_aliados[15-1].ataques[1-1].chance_acertar = 100;   pokemons_aliados[15-1].ataques[2-1].chance_acertar = 100;   pokemons_aliados[15-1].ataques[3-1].chance_acertar = 85;    pokemons_aliados[15-1].ataques[4-1].chance_acertar = 50;
            strcpy(pokemons_aliados[16-1].ataques[1-1].nome_ataque, "ᶠᶦˢ\t\t       Retalhar"); strcpy(pokemons_aliados[16-1].ataques[2-1].nome_ataque, "ᶠᶦˢ\t\t   Investida ossosa"); strcpy(pokemons_aliados[16-1].ataques[3-1].nome_ataque, "ˢᵗᵃ\t\t     Chicote caudal"); strcpy(pokemons_aliados[16-1].ataques[4-1].nome_ataque, "ˢᵖᵉ\t\t     Tapa de lama");
                strcpy(pokemons_aliados[16-1].ataques[1-1].descricao_ataqueL1, "Dobra o poder se um aliado foi derrotado no turno anterior.");
                strcpy(pokemons_aliados[16-1].ataques[2-1].descricao_ataqueL1, "Ataque múltiplo com um osso.");
                strcpy(pokemons_aliados[16-1].ataques[3-1].descricao_ataqueL1, "Reduz a defesa do oponente.");
                strcpy(pokemons_aliados[16-1].ataques[4-1].descricao_ataqueL1, "Ataque de lama que reduz a precisão.");
                    pokemons_aliados[16-1].ataques[1-1].usos_totais = 5;        pokemons_aliados[16-1].ataques[2-1].usos_totais = 10;       pokemons_aliados[16-1].ataques[3-1].usos_totais = 30;       pokemons_aliados[16-1].ataques[4-1].usos_totais = 10;
                    pokemons_aliados[16-1].ataques[1-1].poder = 70;             pokemons_aliados[16-1].ataques[2-1].poder = 25;             pokemons_aliados[16-1].ataques[3-1].poder = 0;              pokemons_aliados[16-1].ataques[4-1].poder = 20;
                    pokemons_aliados[16-1].ataques[1-1].chance_acertar = 100;   pokemons_aliados[16-1].ataques[2-1].chance_acertar = 90;    pokemons_aliados[16-1].ataques[3-1].chance_acertar = 100;   pokemons_aliados[16-1].ataques[4-1].chance_acertar = 100;
            strcpy(pokemons_aliados[17-1].ataques[1-1].nome_ataque, "ᶠᶦˢ\t\t       Enrolar"); strcpy(pokemons_aliados[17-1].ataques[2-1].nome_ataque, "ᶠᶦˢ\t\t   Cauda dracônica"); strcpy(pokemons_aliados[17-1].ataques[3-1].nome_ataque, "ˢᵗᵃ\t\t   Dança do dragão"); strcpy(pokemons_aliados[17-1].ataques[4-1].nome_ataque, "ˢᵖᵉ\t\t     Hiper feixe");
                strcpy(pokemons_aliados[17-1].ataques[1-1].descricao_ataqueL1, "Envolve o oponente, causando dano contínuo.");
                strcpy(pokemons_aliados[17-1].ataques[2-1].descricao_ataqueL1, "Ataque que força a troca do oponente.");
                strcpy(pokemons_aliados[17-1].ataques[3-1].descricao_ataqueL1, "Aumenta o ataque e a velocidade.");
                strcpy(pokemons_aliados[17-1].ataques[4-1].descricao_ataqueL1, "Ataque poderoso que requer recarga.");
                    pokemons_aliados[17-1].ataques[1-1].usos_totais = 20;       pokemons_aliados[17-1].ataques[2-1].usos_totais = 10;       pokemons_aliados[17-1].ataques[3-1].usos_totais = 20;       pokemons_aliados[17-1].ataques[4-1].usos_totais = 5;
                    pokemons_aliados[17-1].ataques[1-1].poder = 15;             pokemons_aliados[17-1].ataques[2-1].poder = 60;             pokemons_aliados[17-1].ataques[3-1].poder = 0;              pokemons_aliados[17-1].ataques[4-1].poder = 150;
                    pokemons_aliados[17-1].ataques[1-1].chance_acertar = 90;    pokemons_aliados[17-1].ataques[2-1].chance_acertar = 90;    pokemons_aliados[17-1].ataques[3-1].chance_acertar = 100;   pokemons_aliados[17-1].ataques[4-1].chance_acertar = 90;
            strcpy(pokemons_aliados[18-1].ataques[1-1].nome_ataque, "ˢᵗᵃ\t\t      Transformar"); strcpy(pokemons_aliados[18-1].ataques[2-1].nome_ataque, "\t\t\t\t "); strcpy(pokemons_aliados[18-1].ataques[3-1].nome_ataque, "\t\t\t "); strcpy(pokemons_aliados[18-1].ataques[4-1].nome_ataque, "\t\t\t\t ");
                strcpy(pokemons_aliados[18-1].ataques[1-1].descricao_ataqueL1, "Transforma-se no oponente, copiando seus movimentos e stats.");
                strcpy(pokemons_aliados[18-1].ataques[2-1].descricao_ataqueL1, " ");
                strcpy(pokemons_aliados[18-1].ataques[3-1].descricao_ataqueL1, " ");
                    strcpy(pokemons_aliados[18-1].ataques[4-1].descricao_ataqueL1, " ");
                    pokemons_aliados[18-1].ataques[1-1].usos_totais = 10;       pokemons_aliados[18-1].ataques[2-1].usos_totais = 0;        pokemons_aliados[18-1].ataques[3-1].usos_totais = 0;        pokemons_aliados[18-1].ataques[4-1].usos_totais = 0;
                    pokemons_aliados[18-1].ataques[1-1].poder = 0;              pokemons_aliados[18-1].ataques[2-1].poder = 0;              pokemons_aliados[18-1].ataques[3-1].poder = 0;              pokemons_aliados[18-1].ataques[4-1].poder = 0;
                    pokemons_aliados[18-1].ataques[1-1].chance_acertar = 100;   pokemons_aliados[18-1].ataques[2-1].chance_acertar = 0;     pokemons_aliados[18-1].ataques[3-1].chance_acertar = 0;     pokemons_aliados[18-1].ataques[4-1].chance_acertar = 0;
            strcpy(pokemons_aliados[19-1].ataques[1-1].nome_ataque, "ᶠᶦˢ\t\t\t   Brasa"); strcpy(pokemons_aliados[19-1].ataques[2-1].nome_ataque, "ˢᵗᵃ\t\t       Fogo-fátuo"); strcpy(pokemons_aliados[19-1].ataques[3-1].nome_ataque, "ˢᵗᵃ\t\t\tDesabilitar"); strcpy(pokemons_aliados[19-1].ataques[4-1].nome_ataque, "ˢᵖᵉ\t\t    Explosão de fogo");
                strcpy(pokemons_aliados[19-1].ataques[1-1].descricao_ataqueL1, "Ataque de fogo com chance de queimar.");
                strcpy(pokemons_aliados[19-1].ataques[2-1].descricao_ataqueL1, "Causa queimadura no oponente.");
                strcpy(pokemons_aliados[19-1].ataques[3-1].descricao_ataqueL1, "Impede o uso do último movimento do oponente.");
                strcpy(pokemons_aliados[19-1].ataques[4-1].descricao_ataqueL1, "Ataque de fogo muito poderoso com chance de queimar.");
                    pokemons_aliados[19-1].ataques[1-1].usos_totais = 25;       pokemons_aliados[19-1].ataques[2-1].usos_totais = 15;       pokemons_aliados[19-1].ataques[3-1].usos_totais = 20;       pokemons_aliados[19-1].ataques[4-1].usos_totais = 5;
                    pokemons_aliados[19-1].ataques[1-1].poder = 40;             pokemons_aliados[19-1].ataques[2-1].poder = 0;              pokemons_aliados[19-1].ataques[3-1].poder = 0;              pokemons_aliados[19-1].ataques[4-1].poder = 110;
                    pokemons_aliados[19-1].ataques[1-1].chance_acertar = 100;   pokemons_aliados[19-1].ataques[2-1].chance_acertar = 85;    pokemons_aliados[19-1].ataques[3-1].chance_acertar = 100;   pokemons_aliados[19-1].ataques[4-1].chance_acertar = 85;
            strcpy(pokemons_aliados[20-1].ataques[1-1].nome_ataque, "ᶠᶦˢ\t\tChicote de vinheira"); strcpy(pokemons_aliados[20-1].ataques[2-1].nome_ataque, "ˢᵗᵃ\t\t  Pó para dormir"); strcpy(pokemons_aliados[20-1].ataques[3-1].nome_ataque, "ˢᵗᵃ\t\t\t      Toxinas"); strcpy(pokemons_aliados[20-1].ataques[4-1].nome_ataque, "ˢᵖᵉ\t\t   Bomba de lodo");
                strcpy(pokemons_aliados[20-1].ataques[1-1].descricao_ataqueL1, "Chicote de vinha.");
                strcpy(pokemons_aliados[20-1].ataques[2-1].descricao_ataqueL1, "Coloca o oponente para dormir.");
                strcpy(pokemons_aliados[20-1].ataques[3-1].descricao_ataqueL1, "Envenenamento severo que piora a cada turno.");
                strcpy(pokemons_aliados[20-1].ataques[4-1].descricao_ataqueL1, "Bomba de lodo com chance de envenenar.");
                    pokemons_aliados[20-1].ataques[1-1].usos_totais = 25;       pokemons_aliados[20-1].ataques[2-1].usos_totais = 15;       pokemons_aliados[20-1].ataques[3-1].usos_totais = 10;       pokemons_aliados[20-1].ataques[4-1].usos_totais = 10;
                    pokemons_aliados[20-1].ataques[1-1].poder = 45;             pokemons_aliados[20-1].ataques[2-1].poder = 0;              pokemons_aliados[20-1].ataques[3-1].poder = 0;              pokemons_aliados[20-1].ataques[4-1].poder = 90;
                    pokemons_aliados[20-1].ataques[1-1].chance_acertar = 100;   pokemons_aliados[20-1].ataques[2-1].chance_acertar = 75;    pokemons_aliados[20-1].ataques[3-1].chance_acertar = 90;    pokemons_aliados[20-1].ataques[4-1].chance_acertar = 10;

            for (int i = 0; i <= MAX_POKEMON; i++)
                {
                    pokemons_inimigos[i] = pokemons_aliados[i];
                }

            while (menu_principal == 5)
                {
                    char opcao_jogo = ' ';
                    int ataque_usado_aliado = 0, ataque_usado_inimigo = 0, status_usado_aliado = 0, status_usado_inimigo = 0, item_usado_aliado = 0, item_usado_inimigo = 0;
                    
                    
                    while (menu_jogo == 0)
                        {
                            printf("\n\t%s (vida: %.0f / %d)\n", pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].nome, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_max);
                            ler_arte_ascii(caminho_arquivo, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] +1].linha_inicial_inimigo, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] +1].linha_final_inimigo);

                            ler_arte_ascii(caminho_arquivo, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].linha_inicial_aliado, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].linha_final_aliado);
                            printf("\t\t\t\t\t\t\t\t\t\t\t\t\t%s (vida: %.0f / %d)\n", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].nome, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_max);
                            
                            printf("\n\n\t/⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\   /⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\");
                            printf("\n\t⎸\t\t\tAtaque (A)\t\t\t  ⎹   ⎸\t\t\t\tMochila (M)\t\t\t ⎹");
                            printf("\n\t\\_________________________________________________________/   \\__________________________________________________________/");
                            printf("\n\n\t/⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\   /⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\");
                            printf("\n\t⎸\t\t\tStatus (S)\t\t\t  ⎹   ⎸\t\t\t\tPokémon (P)\t\t\t ⎹");
                            printf("\n\t\\_________________________________________________________/   \\__________________________________________________________/");
                            printf("\n\n\n\t\t\t\t\t\t\t\t Opção: "); scanf("%c", &opcao_jogo);
                            fflush(stdin);

                            switch(opcao_jogo)
                                {
                                    case 'a': case 'A': limpar_terminal(); printf("\7"); menu_jogo = 1; break;
                                    case 'm': case 'M': limpar_terminal(); printf("\7"); menu_jogo = 2; break;
                                    case 's': case 'S': limpar_terminal(); printf("\7"); menu_jogo = 3; break;
                                    case 'p': case 'P': limpar_terminal(); printf("\7"); menu_jogo = 4; break;
                                    default: limpar_terminal(); printf("\7Caractere inválido!"); break;
                                }
                        }
                    
                    /*******************************************\
                     *              Menu de Ataques            *
                    \*******************************************/

                    while (menu_jogo == 1) 
                        {
                            char opcao_jogo_ataque = ' ';

                            printf("\n\t%s (vida: %.0f / %d)\n", pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].nome, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_max);
                            ler_arte_ascii(caminho_arquivo, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] +1].linha_inicial_inimigo, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] +1].linha_final_inimigo);

                            ler_arte_ascii(caminho_arquivo, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].linha_inicial_aliado, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].linha_final_aliado);
                            printf("\t\t\t\t\t\t\t\t\t\t\t\t\t%s (vida: %.0f / %d)\n", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].nome, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_max);
                            
                            char buffer1[100], buffer2[100], buffer3[100], buffer4[100];
                            sprintf(buffer1, "%s (1)      \t      %dx", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo - 1] - 1].ataques[0].nome_ataque, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo - 1] - 1].ataques[0].usos_totais);
                            sprintf(buffer2, "%s (2)      \t      %dx", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo - 1] - 1].ataques[1].nome_ataque, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo - 1] - 1].ataques[1].usos_totais);
                            sprintf(buffer3, "%s (3)      \t      %dx", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo - 1] - 1].ataques[2].nome_ataque, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo - 1] - 1].ataques[2].usos_totais);
                            sprintf(buffer4, "%s (4)      \t      %dx", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo - 1] - 1].ataques[3].nome_ataque, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo - 1] - 1].ataques[3].usos_totais);

                            printf("\n\t/⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\   /⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\");
                            printf("\n\t⎸%-30s ⎹   ⎸%-30s ⎹", buffer1, buffer2);
                            printf("\n\t\\_________________________________________________________/   \\___________________________________________________________/");
                            printf("\n\n\t/⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\   /⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\");
                            printf("\n\t⎸%-30s ⎹   ⎸%-30s ⎹", buffer3, buffer4);
                            printf("\n\t\\_________________________________________________________/   \\___________________________________________________________/");
                            printf("\n\n\n\t\t> Voltar (V)\t\t\t\t\t Opção: "); scanf("%c", &opcao_jogo_ataque);
                            fflush(stdin);

                            switch (opcao_jogo_ataque)
                                {
                                    case '1': limpar_terminal(); 
                                                if (pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].ataques[1-1].usos_totais > 0)
                                                    {pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].ataques[1-1].usos_totais--; ataque_usado_aliado = 1; menu_jogo = 5;}
                                                else {puts("Você não tem mais esse ataque.");} printf("\7"); break;
                                    case '2': limpar_terminal();
                                                if (pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].ataques[2-1].usos_totais > 0)
                                                    {pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].ataques[2-1].usos_totais--; ataque_usado_aliado = 2; menu_jogo = 5;}
                                                else {puts("Você não tem mais esse ataque.");} printf("\7"); break;
                                    case '3': limpar_terminal();
                                                if (pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].ataques[3-1].usos_totais > 0)
                                                    {pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].ataques[3-1].usos_totais--; ataque_usado_aliado = 3; menu_jogo = 5;}
                                                else {puts("Você não tem mais esse ataque.");} printf("\7"); break;
                                    case '4': limpar_terminal();
                                                if (pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].ataques[4-1].usos_totais > 0)
                                                    {pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].ataques[4-1].usos_totais--; ataque_usado_aliado = 4; menu_jogo = 5;}
                                                else {puts("Você não tem mais esse ataque.");} printf("\7"); break;
                                    case 'v': case 'V': menu_jogo = 0; break;
                                    default: limpar_terminal(); printf("\7Caractere ou número inválido."); break;
                                }
                            limpar_terminal();
                        }
                                /*******************************************\
                                 *                 Mochila                 *
                                \*******************************************/

                            char cpy_opcao_jogo_mochila = ' ', descricao_Item_mochilaL1 [60] = "\t\t\t\t\t\t\t\t ⎹", descricao_Item_mochilaL2 [60] = "\t\t\t\t\t\t\t\t ⎹";
                            char  descricao_Item_mochilaL3 [60] = "\t\t\t\t\t\t\t\t ⎹", descricao_Item_mochilaL4 [60] = "\t\t\t\t\t\t\t\t ⎹";
                            int paginas_jogo_mochila = 1;

                            typedef struct
                                {
                                    char nome_item[20];
                                    int usos_item;
                                    char descricao_mochila_linha1[60];
                                } itens;
                                
                            itens item_aliado[8], item_inimigo[8]; 
                            strcpy(item_aliado[1-1].nome_item, "Poção"); item_aliado[1-1].usos_item = 20; strcpy(item_aliado[1-1].descricao_mochila_linha1, "\t     Recupera 20 de vida do seu pokémon.\t\t ⎹");
                            strcpy(item_aliado[2-1].nome_item, "Super poção"); item_aliado[2-1].usos_item = 5; strcpy(item_aliado[2-1].descricao_mochila_linha1, "\t     Recupera 50 de vida do seu pokémon.\t\t ⎹");
                            strcpy(item_aliado[3-1].nome_item, "Poção máxima"); item_aliado[3-1].usos_item = 1; strcpy(item_aliado[3-1].descricao_mochila_linha1, "\t     Recupera toda a vida do seu pokémon.\t\t ⎹");
                            strcpy(item_aliado[4-1].nome_item, "Cura de fogo"); item_aliado[4-1].usos_item = 5; strcpy(item_aliado[4-1].descricao_mochila_linha1, "\t  Remove a condição de \"queimado\" do seu pokémon\t ⎹");
                            strcpy(item_aliado[5-1].nome_item, "Cura de paralizia"); item_aliado[5-1].usos_item = 5; strcpy(item_aliado[5-1].descricao_mochila_linha1, "\t     Retira a paralização de seu pokémon\t\t\t ⎹");
                            strcpy(item_aliado[6-1].nome_item, "Antídodo "); item_aliado[6-1].usos_item = 5; strcpy(item_aliado[6-1].descricao_mochila_linha1, "\t     Cura seu pokémon que estiver envenenado\t\t\t ⎹");
                            strcpy(item_aliado[7-1].nome_item, "Despertador"); item_aliado[7-1].usos_item = 5; strcpy(item_aliado[7-1].descricao_mochila_linha1, "\t     Acorda seu pokémon que estiver dormindo\t\t\t ⎹");
                            strcpy(item_aliado[8-1].nome_item, "Baga"); item_aliado[8-1].usos_item = 5; strcpy(item_aliado[8-1].descricao_mochila_linha1, "\t     Recupera 10 de vida e é deliciosa!\t\t\t ⎹");

                            for (int i = 0; i < 8; i++)
                                item_inimigo[i] = item_aliado[i];

                    while (menu_jogo == 2) // Menu da mochila
                        {
                            char opcao_jogo_mochila;
                            
                            if (paginas_jogo_mochila == 2)
                                {
                                    printf("\n\t%s (vida: %.0f / %d)\n", pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].nome, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_max);
                                    ler_arte_ascii(caminho_arquivo, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] +1].linha_inicial_inimigo, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] +1].linha_final_inimigo);

                                    ler_arte_ascii(caminho_arquivo, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].linha_inicial_aliado, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].linha_final_aliado);
                                    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t%s (vida: %.0f / %d)\n", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].nome, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_max);

                                    printf("\n\t\t/⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\/⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\");
                                    printf("\n\t\t⎸\t%dx\t", item_aliado[5-1].usos_item); 
                                    if(opcao_jogo_mochila == '5')
                                        printf("> ");

                                    printf("%-17s (5)\t⎹ ", item_aliado[5-1].nome_item); printf("\t\t\tDescrição do item:\t\t\t ⎹");
                                    printf("\n\t\t⊢―――――――――――――――――――――――――――――――――――――――⊣\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⎸\t%dx\t", item_aliado[6-1].usos_item); 

                                    if(opcao_jogo_mochila == '6')
                                        printf("> ");
                                    printf("%-18s (6)\t⎹ ", item_aliado[6-1].nome_item); printf("%s", descricao_Item_mochilaL1);
                                    printf("\n\t\t⊢―――――――――――――――――――――――――――――――――――――――⊣\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⎸\t%dx\t", item_aliado[7-1].usos_item); 

                                    if(opcao_jogo_mochila == '7')
                                        printf("> ");
                                    printf("%-17s (7)\t⎹ ", item_aliado[7-1].nome_item); printf("%s", descricao_Item_mochilaL2);
                                    printf("\n\t\t⊢―――――――――――――――――――――――――――――――――――――――⊣\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⎸\t%dx\t", item_aliado[8-1].usos_item); 

                                    if(opcao_jogo_mochila == '8')
                                        printf("> ");
                                    printf("%-17s (8)\t⎹ ", item_aliado[8-1].nome_item); printf("%s", descricao_Item_mochilaL3);
                                    printf("\n\t\t⊢―――――――――――――――――――――――――――――――――――――――⊣\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⎸\t\tAnterior (A)     \t⎹ "); printf("%s", descricao_Item_mochilaL4);
                                    printf("\n\t\t\\_______________________________________/\\_______________________________________________________________/");
                                    printf("\n\n\t\t\t\t> Usar (U)\t\t\t> Voltar (V)\t\t\t Opção: ");
                                    opcao_jogo_mochila = getchar(); getchar();
                                    opcao_jogo_mochila = tolower(opcao_jogo_mochila);
                                    switch (opcao_jogo_mochila)
                                        {
                                            case 'u': if (cpy_opcao_jogo_mochila == '5')
                                                            {   limpar_terminal(); if (item_aliado[5-1].usos_item != 0) {item_aliado[5-1].usos_item--; item_usado_aliado = 5; menu_jogo = 5;}
                                                            else {printf("Você não tem mais esse item.");} printf("\7"); break;   }
                                                    else if (cpy_opcao_jogo_mochila == '6')
                                                            {   limpar_terminal(); if (item_aliado[6-1].usos_item != 0) {item_aliado[6-1].usos_item--; item_usado_aliado = 6; menu_jogo = 5;}
                                                            else {printf("Você não tem mais esse item.");} printf("\7"); break;   }
                                                    else if (cpy_opcao_jogo_mochila == '7')
                                                            {   limpar_terminal(); if (item_aliado[7-1].usos_item != 0) {item_aliado[7-1].usos_item--; item_usado_aliado = 7; menu_jogo = 5;}
                                                            else {printf("Você não tem mais esse item.");} printf("\7"); break;   }
                                                    else if (cpy_opcao_jogo_mochila == '8')
                                                            {   limpar_terminal(); if (item_aliado[8-1].usos_item != 0) {item_aliado[8-1].usos_item--; item_usado_aliado = 8; menu_jogo = 5;}
                                                            else {printf("Você não tem mais esse item.");} printf("\7"); break;   }
                                                    break;
                                            case '5': limpar_terminal(); cpy_opcao_jogo_mochila = opcao_jogo_mochila;
                                                        strcpy(descricao_Item_mochilaL1, item_aliado[5-1].descricao_mochila_linha1); strcpy(descricao_Item_mochilaL2, "\t\t\t\t\t\t\t\t ⎹"); printf("\7"); break;
                                            case '6': limpar_terminal(); cpy_opcao_jogo_mochila = opcao_jogo_mochila;
                                                        strcpy(descricao_Item_mochilaL1, item_aliado[6-1].descricao_mochila_linha1); strcpy(descricao_Item_mochilaL2, "\t\t\t\t\t\t\t\t ⎹"); printf("\7"); break;
                                            case '7': limpar_terminal(); cpy_opcao_jogo_mochila = opcao_jogo_mochila;
                                                        strcpy(descricao_Item_mochilaL1, item_aliado[7-1].descricao_mochila_linha1); strcpy(descricao_Item_mochilaL2, "\t\t\t\t\t\t\t\t ⎹"); printf("\7"); break;
                                            case '8': limpar_terminal(); cpy_opcao_jogo_mochila = opcao_jogo_mochila;
                                                        strcpy(descricao_Item_mochilaL1, item_aliado[8-1].descricao_mochila_linha1); strcpy(descricao_Item_mochilaL2, "\t\t\t\t\t\t\t\t ⎹"); printf("\7"); break;
                                            case 'a': limpar_terminal(); paginas_jogo_mochila = 1; printf("\7"); break;
                                            case 'v': limpar_terminal(); menu_jogo = 0; printf("\7"); break;
                                            default: limpar_terminal(); strcpy(descricao_Item_mochilaL1, "\t\t\t\t\t\t\t\t ⎹"); strcpy(descricao_Item_mochilaL2, "\t\t\t\t\t\t\t\t ⎹"); printf("\7Caractere inválido!"); break;
                                            
                                        }
                                }

                            if (paginas_jogo_mochila == 1)
                                {
                                    printf("\n\t%s (vida: %.0f / %d)\n", pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].nome, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_max);
                                    ler_arte_ascii(caminho_arquivo, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] +1].linha_inicial_inimigo, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] +1].linha_final_inimigo);

                                    ler_arte_ascii(caminho_arquivo, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].linha_inicial_aliado, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].linha_final_aliado);
                                    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t%s (vida: %.0f / %d)\n", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].nome, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_max);

                                    printf("\n\t\t/⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\/⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\");
                                    printf("\n\t\t⎸\t%dx\t", item_aliado[1-1].usos_item); 
                                    if(opcao_jogo_mochila == '1')
                                        printf("> ");

                                    printf("%-19s (1)\t⎹", item_aliado[1-1].nome_item); printf("\t\t\tDescrição do item:\t\t\t ⎹");
                                    printf("\n\t\t⊢―――――――――――――――――――――――――――――――――――――――⊣\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⎸\t%dx\t", item_aliado[2-1].usos_item); 
                                    if(opcao_jogo_mochila == '2')
                                        printf("> ");

                                    printf("%-19s (2)\t⎹ ", item_aliado[2-1].nome_item); printf("%s", descricao_Item_mochilaL1);
                                    printf("\n\t\t⊢―――――――――――――――――――――――――――――――――――――――⊣\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⎸\t%dx\t", item_aliado[3-1].usos_item); 
                                    if(opcao_jogo_mochila == '3')
                                        printf("> ");

                                    printf("%-20s (3)\t⎹ ", item_aliado[3-1].nome_item); printf("%s", descricao_Item_mochilaL2);
                                    printf("\n\t\t⊢―――――――――――――――――――――――――――――――――――――――⊣\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⎸\t%dx\t", item_aliado[4-1].usos_item); 
                                    if(opcao_jogo_mochila == '4')
                                        printf("> ");

                                    printf("%-17s (4)\t⎹ ", item_aliado[4-1].nome_item); printf("%s", descricao_Item_mochilaL3);
                                    printf("\n\t\t⊢―――――――――――――――――――――――――――――――――――――――⊣\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⎸\t\tPróximo (P)     \t⎹ "); printf("%s", descricao_Item_mochilaL4);
                                    printf("\n\t\t\\_______________________________________/\\_______________________________________________________________/");

                                    printf("\n\n\t\t\t\t> Usar (U)\t\t\t> Voltar (V)\t\t\t Opção: ");
                                    opcao_jogo_mochila = getchar(); getchar();
                                    opcao_jogo_mochila = tolower(opcao_jogo_mochila);
                                    switch (opcao_jogo_mochila)
                                        {
                                            case 'u': if (cpy_opcao_jogo_mochila == '1')
                                                            {   limpar_terminal(); if (item_aliado[1-1].usos_item != 0) {item_aliado[1-1].usos_item--; item_usado_aliado = 1; menu_jogo = 5;}
                                                            else {printf("Você não tem mais esse item.\n");} break;   }
                                                    else if (cpy_opcao_jogo_mochila == '2')
                                                            {   limpar_terminal(); if (item_aliado[2-1].usos_item != 0) {item_aliado[2-1].usos_item--; item_usado_aliado = 2; menu_jogo = 5;}
                                                            else {printf("Você não tem mais esse item.\n");} break;   }
                                                    else if (cpy_opcao_jogo_mochila == '3')
                                                            {   limpar_terminal(); if (item_aliado[3-1].usos_item != 0) {item_aliado[3-1].usos_item--; item_usado_aliado = 3; menu_jogo = 5;}
                                                            else {printf("Você não tem mais esse item.\n");} break;   }
                                                    else if (cpy_opcao_jogo_mochila == '4')
                                                            {   limpar_terminal(); if (item_aliado[4-1].usos_item != 0) {item_aliado[4-1].usos_item--; item_usado_aliado = 4; menu_jogo = 5;}
                                                            else {printf("Você não tem mais esse item.\n");} break;   }
                                                    break;
                                            case '1': limpar_terminal(); cpy_opcao_jogo_mochila = opcao_jogo_mochila;
                                                        strcpy(descricao_Item_mochilaL1, item_aliado[1-1].descricao_mochila_linha1); strcpy(descricao_Item_mochilaL2, "\t\t\t\t\t\t\t\t ⎹"); printf("\7"); break;
                                            case '2': limpar_terminal(); cpy_opcao_jogo_mochila = opcao_jogo_mochila;
                                                        strcpy(descricao_Item_mochilaL1, item_aliado[2-1].descricao_mochila_linha1); strcpy(descricao_Item_mochilaL2, "\t\t\t\t\t\t\t\t ⎹"); printf("\7"); break;
                                            case '3': limpar_terminal(); cpy_opcao_jogo_mochila = opcao_jogo_mochila;
                                                        strcpy(descricao_Item_mochilaL1, item_aliado[3-1].descricao_mochila_linha1); strcpy(descricao_Item_mochilaL2, "\t\t\t\t\t\t\t\t ⎹"); printf("\7"); break;
                                            case '4': limpar_terminal(); cpy_opcao_jogo_mochila = opcao_jogo_mochila;
                                                        strcpy(descricao_Item_mochilaL1, item_aliado[4-1].descricao_mochila_linha1); strcpy(descricao_Item_mochilaL2, "\t\t\t\t\t\t\t\t ⎹"); printf("\7"); break;
                                            case 'p': limpar_terminal(); paginas_jogo_mochila = 2; printf("\7"); break;
                                            case 'v': limpar_terminal(); menu_jogo = 0; printf("\7"); break;
                                            default: limpar_terminal(); strcpy(descricao_Item_mochilaL1, "\t\t\t\t\t\t\t\t ⎹"); strcpy(descricao_Item_mochilaL2, "\t\t\t\t\t\t\t\t ⎹"); printf("\7Caractere inválido!\n"); break;
                                            
                                        }
                                }
                        }
                    
                                /*******************************************\
                                 *             Menu dos status           *
                                \*******************************************/

                    int menu_jogo_status = 1, pokemonSelecionadoAliado_status = 0, jogo_status_ataque_selecionado = 0;

                    while (menu_jogo == 3) // Menu dos status
                        {
                            char opcao_jogo_status = ' ';
                            if (menu_jogo_status == 1)
                                {
                                    printf("\n\t%s (vida: %.0f / %d)\n", pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].nome, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_max);
                                    ler_arte_ascii(caminho_arquivo, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] +1].linha_inicial_inimigo, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] +1].linha_final_inimigo);

                                    ler_arte_ascii(caminho_arquivo, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].linha_inicial_aliado, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].linha_final_aliado);
                                    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t%s (vida: %.0f / %d)\n", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].nome, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_max);
                                    
                                    printf("\n\t\t/⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\/⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\");
                                    printf("\n\t\t⎸\t\t"); printf("Pokemons (P)    \t⎹\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⊢―――――――――――――――――――――――――――――――――――――――⊣\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⎸\t\t"); printf("Ataques (A)    \t\t⎹\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⊢―――――――――――――――――――――――――――――――――――――――⊣\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⎸\t\t\t\t\t⎹\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⊢―――――――――――――――――――――――――――――――――――――――⊣\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⎸\t\t\t\t\t⎹\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⊢―――――――――――――――――――――――――――――――――――――――⊣\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⎸\t\t\t\t\t⎹\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t\\_______________________________________/\\_______________________________________________________________/");
                                    printf("\n\n\t\t\t> Voltar (V)\t\t\t Opção: ");
                                    
                                    opcao_jogo_status = getchar(); fflush(stdin);
                                    opcao_jogo_status = tolower (opcao_jogo_status);
                                    switch (opcao_jogo_status)
                                        {
                                            case 'p': limpar_terminal(); menu_jogo_status = 2; printf("\7"); break;
                                            case 'a': limpar_terminal(); menu_jogo_status = 3; printf("\7"); break;
                                            case 'v': limpar_terminal(); menu_jogo = 0; printf("\7"); break;
                                            default: limpar_terminal(); printf("\7Caractere inválido!\n"); break;
                                        }
                                }
                                
                            if (menu_jogo_status == 2)
                                {
                                    printf("\n\t%s (vida: %.0f / %d)\n", pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].nome, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_max);
                                    ler_arte_ascii(caminho_arquivo, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] +1].linha_inicial_inimigo, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] +1].linha_final_inimigo);

                                    ler_arte_ascii(caminho_arquivo, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].linha_inicial_aliado, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].linha_final_aliado);
                                    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t%s (vida: %.0f / %d)\n", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].nome, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_max);
                                    
                                    printf("\n\t\t/⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\/⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\");
                                    printf("\n\t\t⎸\t\t"); if (pokemonSelecionadoAliado_status == 1) printf("> ");
                                    if (pokemonsSelecionadosAliados[1-1] == 10 || pokemonsSelecionadosAliados[1-1] == 12 || pokemonsSelecionadosAliados[1-1] == 18) printf("  ");
                                    printf("%s (1)      \t⎹", pokemons_aliados[pokemonsSelecionadosAliados[1-1] -1].nome); printf("\t\t\tStatus do Pokémon:\t\t\t ⎹");
                                    printf("\n\t\t⊢―――――――――――――――――――――――――――――――――――――――⊣"); printf("\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⎸\t\t"); if (pokemonSelecionadoAliado_status == 2) printf("> ");
                                    if (pokemonsSelecionadosAliados[2-1] == 10 || pokemonsSelecionadosAliados[2-1] == 12 || pokemonsSelecionadosAliados[2-1] == 18) printf("  ");
                                    printf("%s (2)      \t⎹", pokemons_aliados[pokemonsSelecionadosAliados[2-1] -1].nome); 
                                    if (pokemonSelecionadoAliado_status >= 1 && pokemonSelecionadoAliado_status <= 3)
                                        {
                                            printf("    Nome: %s", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_status -1] -1].nome);
                                            printf("         \t\tVida máxima: %d\t ⎹", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_status -1] -1].vida_max);
                                        }
                                    else
                                        printf("\t\t\t\t\t\t\t\t ⎹");

                                    printf("\n\t\t⊢―――――――――――――――――――――――――――――――――――――――⊣");
                                    if (pokemonSelecionadoAliado_status >= 1 && pokemonSelecionadoAliado_status <= 3)
                                        printf("\t\t\t\t\tDefesa: %d\t\t ⎹", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_status -1] -1].defesa);
                                    else
                                        printf("\t\t\t\t\t\t\t\t ⎹");

                                    printf("\n\t\t⎸\t\t"); if (pokemonSelecionadoAliado_status == 3) printf("> ");
                                    if (pokemonsSelecionadosAliados[3-1] == 10 || pokemonsSelecionadosAliados[3-1] == 12 || pokemonsSelecionadosAliados[3-1] == 18) printf("  ");
                                    printf("%s (3)      \t⎹", pokemons_aliados[pokemonsSelecionadosAliados[3-1] -1].nome);
                                    if (pokemonSelecionadoAliado_status >= 1 && pokemonSelecionadoAliado_status <= 3)
                                        {
                                            printf("    Número: %d", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_status -1] -1].poke_num);
                                            printf("    \t\t\tVelocidade: %d\t\t ⎹", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_status -1] -1].velocidade);
                                        }
                                    else
                                        printf("\t\t\t\t\t\t\t\t ⎹");

                                    printf("\n\t\t⊢―――――――――――――――――――――――――――――――――――――――⊣");
                                    if (pokemonSelecionadoAliado_status >= 1 && pokemonSelecionadoAliado_status <= 3)
                                        {
                                            printf("    Tipo 1: %s", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_status -1] -1].tipo1);
                                            if (pokemonsSelecionadosAliados[1-1] == 14 || pokemonsSelecionadosAliados[1-1] == 16) printf(" ");
                                            else if (pokemonsSelecionadosAliados[2-1] == 14 || pokemonsSelecionadosAliados[2-1] == 16) printf(" ");
                                            else if (pokemonsSelecionadosAliados[3-1] == 14 || pokemonsSelecionadosAliados[3-1] == 16) printf(" ");
                                            printf("       \t\tDano do ataque: %d\t ⎹", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_status -1] -1].ataque);
                                        }
                                    else
                                        printf("\t\t\t\t\t\t\t\t ⎹");

                                    printf("\n\t\t⎸\t\t\t\t\t⎹");
                                    if (pokemonSelecionadoAliado_status >= 1 && pokemonSelecionadoAliado_status <= 3)
                                        {
                                            printf("    Tipo 2: %s", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_status -1] -1].tipo2);
                                            if (pokemonsSelecionadosAliados[1-1] == 14 || pokemonsSelecionadosAliados[1-1] == 16) printf(" ");
                                            else if (pokemonsSelecionadosAliados[2-1] == 14 || pokemonsSelecionadosAliados[2-1] == 16) printf(" ");
                                            else if (pokemonsSelecionadosAliados[3-1] == 14 || pokemonsSelecionadosAliados[3-1] == 16) printf(" ");
                                            printf("          \t\tDano do especial: %d\t ⎹", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_status -1] -1].especial);
                                        }
                                    else
                                        printf("\t\t\t\t\t\t\t\t ⎹");

                                    printf("\n\t\t⊢―――――――――――――――――――――――――――――――――――――――⊣"); printf("\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⎸\t\tAnterior (A)     \t⎹ "); printf("\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t\\_______________________________________/\\_______________________________________________________________/");
                                    printf("\n\n\t\t\t> Voltar (V)\t\t\t Opção: ");

                                    opcao_jogo_status = getchar(); fflush(stdin);
                                    opcao_jogo_status = tolower (opcao_jogo_status);
                                    switch (opcao_jogo_status)
                                        {
                                            case '1': limpar_terminal(); pokemonSelecionadoAliado_status = 1; printf("\7"); break;
                                            case '2': limpar_terminal(); pokemonSelecionadoAliado_status = 2; printf("\7"); break;
                                            case '3': limpar_terminal(); pokemonSelecionadoAliado_status = 3; printf("\7"); break;
                                            case 'a': limpar_terminal(); menu_jogo_status = 1; printf("\7"); break;
                                            case 'v': limpar_terminal(); menu_jogo = 0; printf("\7"); break;
                                            default: pokemonSelecionadoAliado_status = 0; limpar_terminal(); printf("\7Caractere inválido!\n"); break;
                                        }
                                }
                            
                            if (menu_jogo_status == 3)
                                {
                                    printf("\n\t%s (vida: %.0f / %d)\n", pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].nome, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_max);
                                    ler_arte_ascii(caminho_arquivo, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] +1].linha_inicial_inimigo, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] +1].linha_final_inimigo);

                                    ler_arte_ascii(caminho_arquivo, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].linha_inicial_aliado, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].linha_final_aliado);
                                    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t%s (vida: %.0f / %d)\n", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].nome, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_max);
                                    
                                    printf("\n\t\t/⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\/⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\");
                                    printf("\n\t\t⎸\t\t"); if (jogo_status_ataque_selecionado == 1) {printf("> ");}
                                    printf("Ataque 1 (1)    \t⎹");
                                    if (jogo_status_ataque_selecionado != 0) printf("   %-52s\t ⎹", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].ataques[jogo_status_ataque_selecionado -1].nome_ataque);
                                    else printf("\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⊢―――――――――――――――――――――――――――――――――――――――⊣\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⎸\t\t"); if (jogo_status_ataque_selecionado == 2) {printf("> ");}
                                    printf("Ataque 2 (2)    \t⎹"); 
                                    if (jogo_status_ataque_selecionado != 0) printf("   %-56s\t ⎹", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].ataques[jogo_status_ataque_selecionado -1].descricao_ataqueL1);
                                    else printf("\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⊢―――――――――――――――――――――――――――――――――――――――⊣\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⎸\t\t"); if (jogo_status_ataque_selecionado == 3) {printf("> ");}
                                    printf("Ataque 3 (3)    \t⎹"); 
                                    if (jogo_status_ataque_selecionado != 0) printf("    Usos restantes: %-2d\t\t\t\tPoder: %-2d\t ⎹", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].ataques[jogo_status_ataque_selecionado -1].usos_totais, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].ataques[jogo_status_ataque_selecionado -1].poder);
                                    else printf("\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⊢―――――――――――――――――――――――――――――――――――――――⊣");
                                    if (jogo_status_ataque_selecionado != 0) printf("    Chance de acertar: %-3d%%\t\t\t\t\t ⎹", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].ataques[jogo_status_ataque_selecionado -1].chance_acertar);
                                    else printf("\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⎸\t\t"); if (jogo_status_ataque_selecionado == 4) {printf("> ");}
                                    printf("Ataque 4 (4)    \t⎹\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⊢―――――――――――――――――――――――――――――――――――――――⊣\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t⎸\t\tAnterior (A)     \t⎹\t\t\t\t\t\t\t\t ⎹");
                                    printf("\n\t\t\\_______________________________________/\\_______________________________________________________________/");
                                    printf("\n\n\t\t\t> Voltar (V)\t\t\t Opção: ");
                                    
                                    opcao_jogo_status = getchar(); fflush(stdin);
                                    opcao_jogo_status = tolower (opcao_jogo_status);
                                    switch (opcao_jogo_status)
                                        {
                                            case '1': limpar_terminal(); jogo_status_ataque_selecionado = 1; printf("\7"); break;
                                            case '2':limpar_terminal(); jogo_status_ataque_selecionado = 2; printf("\7"); break;
                                            case '3':limpar_terminal(); jogo_status_ataque_selecionado = 3; printf("\7"); break;
                                            case '4':limpar_terminal(); jogo_status_ataque_selecionado = 4; printf("\7"); break;
                                            case 'a': limpar_terminal(); jogo_status_ataque_selecionado = 0; menu_jogo_status = 1; printf("\7"); break;
                                            case 'v': limpar_terminal(); jogo_status_ataque_selecionado = 0; menu_jogo = 0; printf("\7"); break;
                                            default: limpar_terminal(); jogo_status_ataque_selecionado = 0; printf("\7Caractere inválido!\n"); break;
                                        }
                                }
                            
                        }
                    
                                /*******************************************\
                                 *             Menu dos pokémons           *
                                \*******************************************/
                    
                    char descricao_menu_pokeL1[70] = "\t\t\t\t\t\t\t\t ⎹", descricao_menu_pokeL2[70] = "\t\t\t\t\t\t\t\t ⎹", descricao_menu_pokeL3[70] = "\t\t\t\t\t\t\t\t ⎹";
                    int aux_descricao_jogo_pokemon = 0;

                    while (menu_jogo == 4) // Menu dos pokémons
                        {
                            int controle_descricao = 1;
                            char opcao_jogo_pokemon;

                            printf("\n\t%s (vida: %.0f / %d)\n", pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].nome, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_max);
                            ler_arte_ascii(caminho_arquivo, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] +1].linha_inicial_inimigo, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] +1].linha_final_inimigo);

                            ler_arte_ascii(caminho_arquivo, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].linha_inicial_aliado, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].linha_final_aliado);
                            printf("\t\t\t\t\t\t\t\t\t\t\t\t\t%s (vida: %.0f / %d)\n", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].nome, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_max);
                            
                            
                            printf("\n\t\t/⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\/⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\");
                            printf("\n\t\t⎸\t\t"); if (opcao_jogo_pokemon == '1') printf("> ");
                            if (pokemonsSelecionadosAliados[1-1] == 12) putchar(' ');
                            printf("%s (1)       \t ⎹", pokemons_aliados[pokemonsSelecionadosAliados[1-1] -1].nome); printf("\t\t\tDescrição do pokémon:\t\t\t ⎹");
                            printf("\n\t\t⊢――――――――――――――――――――――――――――――――――――――――⊣\t\t\t\t\t\t\t\t ⎹");
                            printf("\n\t\t⎸\t\t"); if (opcao_jogo_pokemon == '2') printf("> ");
                            if (pokemonsSelecionadosAliados[2-1] == 12) putchar(' ');
                            printf("%s (2)       \t ⎹", pokemons_aliados[pokemonsSelecionadosAliados[2-1] -1].nome); printf("%s", descricao_menu_pokeL1);
                            printf("\n\t\t⊢――――――――――――――――――――――――――――――――――――――――⊣"); printf("%s", descricao_menu_pokeL2);
                            printf("\n\t\t⎸\t\t"); if (opcao_jogo_pokemon == '3') printf("> ");
                            if (pokemonsSelecionadosAliados[3-1] == 12) putchar(' ');
                            printf("%s (3)       \t ⎹", pokemons_aliados[pokemonsSelecionadosAliados[3-1] -1].nome); printf("%s", descricao_menu_pokeL3);
                            printf("\n\t\t\\________________________________________/\\______________________________________________________________/");
                            printf("\n\n\t\t\t\t\t> Voltar (V) \t\t> Escolher (E) \t\tOpção: ");
                            opcao_jogo_pokemon = getchar(); fflush(stdin);
                            opcao_jogo_pokemon = tolower(opcao_jogo_pokemon);
                            switch (opcao_jogo_pokemon)
                                {
                                    case 'e': if (aux_descricao_jogo_pokemon == 1)
                                                    {
                                                        if(aux_descricao_jogo_pokemon != pokemonSelecionadoAliado_jogo)
                                                            {limpar_terminal(); pokemonSelecionadoAliado_jogo = 1; printf("\7");}
                                                        else
                                                            {
                                                                controle_descricao = 0; limpar_terminal();
                                                                printf("\7Esse Pokémon já está em campo.");
                                                            }
                                                    }
                                              if (aux_descricao_jogo_pokemon == 2)
                                                    {
                                                        if(aux_descricao_jogo_pokemon != pokemonSelecionadoAliado_jogo)
                                                            {limpar_terminal(); pokemonSelecionadoAliado_jogo = 2; printf("\7");}
                                                        else
                                                            {
                                                                controle_descricao = 0; limpar_terminal();
                                                                printf("\7Esse Pokémon já está em campo.");
                                                            }
                                                    }
                                              if (aux_descricao_jogo_pokemon == 3)
                                                    {
                                                        if(aux_descricao_jogo_pokemon != pokemonSelecionadoAliado_jogo)
                                                            {limpar_terminal(); pokemonSelecionadoAliado_jogo = 3; printf("\7");}
                                                        else
                                                            {
                                                                controle_descricao = 0; limpar_terminal();
                                                                printf("\7Esse Pokémon já está em campo.");
                                                            }
                                                    }
                                              break;
                                    case '1': limpar_terminal(); aux_descricao_jogo_pokemon = 1; printf("\7"); break;
                                    case '2': limpar_terminal(); aux_descricao_jogo_pokemon = 2; printf("\7"); break;
                                    case '3': limpar_terminal(); aux_descricao_jogo_pokemon = 3; printf("\7"); break;
                                    case 'v': limpar_terminal(); menu_jogo = 0; printf("\7"); break;
                                    default: limpar_terminal(); controle_descricao = 0; printf("\7Caractere Inválido!!!");
                                }
                            
                            if(pokemonSelecionadoAliado_jogo >= 1 && pokemonSelecionadoAliado_jogo <= 3) 
                                {
                                    if (controle_descricao == 1)
                                        {
                                            if (aux_descricao_jogo_pokemon == 1 || aux_descricao_jogo_pokemon == 2 || aux_descricao_jogo_pokemon == 3)
                                                {
                                                    strcpy(descricao_menu_pokeL1, pokemons_aliados[pokemonsSelecionadosAliados[aux_descricao_jogo_pokemon -1] -1].descricao_pokemonL1);
                                                    strcpy(descricao_menu_pokeL2, pokemons_aliados[pokemonsSelecionadosAliados[aux_descricao_jogo_pokemon -1] -1].descricao_pokemonL2);
                                                    strcpy(descricao_menu_pokeL3, pokemons_aliados[pokemonsSelecionadosAliados[aux_descricao_jogo_pokemon -1] -1].descricao_pokemonL3);
                                                }
                                        }
                                    else
                                        {strcpy(descricao_menu_pokeL1, "\t\t\t\t\t\t\t\t ⎹"); strcpy(descricao_menu_pokeL2, "\t\t\t\t\t\t\t\t ⎹"); strcpy(descricao_menu_pokeL3, "\t\t\t\t\t\t\t\t ⎹");}
                                }
                            else
                                {strcpy(descricao_menu_pokeL1, "\t\t\t\t\t\t\t\t ⎹"); strcpy(descricao_menu_pokeL2, "\t\t\t\t\t\t\t\t ⎹"); strcpy(descricao_menu_pokeL3, "\t\t\t\t\t\t\t\t ⎹");}
                        }

                                /*******************************************\
                                 *             Menu das ações           *
                                \*******************************************/

                    if (menu_jogo == 5)
                        {
                            // Inicializa o gerador de números aleatórios
                            srand(time(NULL));

                            // Decisão aleatória entre atacar, usar item ou trocar Pokémon
                            int acao = rand() % 3;  // 0 para atacar, 1 para usar item, 2 para trocar Pokémon
                            if (pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest == 0)
                                {
                                    pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].pokemon_vivo = 0;
                                    acao = 2;
                                }

                            if (acao == 1)
                            {
                                // Escolher um item aleatório (entre 1 e 8)
                                if (pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest > (pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_max * 3) / 2)
                                    {
                                        item_usado_inimigo = (rand() % 8) + 1;
                                        ataque_usado_inimigo = 0;
                                    }
                                else acao = rand() % 3;
                            }
                            else if (acao == 0)
                            {
                                // Escolher um ataque aleatório (entre 1 e 4)
                                ataque_usado_inimigo = (rand() % 4) + 1;
                                item_usado_inimigo = 0;
                            }
                            
                            else if (acao == 2)
                            {
                                // Trocar para um Pokémon aleatório (entre 1 e 3)
                                if (pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].pokemon_vivo == 0)
                                    while (pokemonSelecionadoInimigo_jogo == pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].poke_num)
                                        pokemonSelecionadoInimigo_jogo = (rand() % 3) + 1;
                                else
                                {
                                    pokemonSelecionadoInimigo_jogo = (rand() % 3) + 1;
                                    ataque_usado_inimigo = 0;
                                    item_usado_inimigo = 0;
                                }
                            }
                            
                            int numero_rodadas;
                            if (pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].ataques[ataque_usado_aliado-1].poder == 0)
                                status_usado_aliado = 1;
                            if (pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoAliado_jogo -1] -1].ataques[ataque_usado_inimigo-1].poder == 0)
                                status_usado_inimigo = 1;
                            for (numero_rodadas = 4; numero_rodadas > 0; numero_rodadas--)
                                {
                                    // calcular_dano(int AtaqueBase, int PoderAtaque, int ChanceAcerto, char Tipo1Atacante[20], char Tipo2Atacante[20], char Tipo1Alvo[20], char Tipo2Alvo[20], int ChanceCritico, int defesaAlvo)
                                    float resDanoInimigo = calcular_dano(pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].ataque, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].ataques[ataque_usado_aliado -1].poder,
                                                                pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].ataques[ataque_usado_aliado -1].chance_acertar, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].tipo1,
                                                                pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].tipo2, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].tipo1,
                                                                pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].tipo2, 20, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].defesa);
                                    
                                    float resDanoAliado = calcular_dano(pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].ataque, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].ataques[ataque_usado_aliado -1].poder,
                                                                pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].ataques[ataque_usado_inimigo -1].chance_acertar, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].tipo1,
                                                                pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].tipo2, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].tipo1,
                                                                pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].tipo2, 20, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].defesa);

                                    // pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest - resDanoInimigo;
                                    // pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest - resDanoAliado;
                                    
                                    // Limitar vida pokemons aliados entre 0 e max
                                    if (pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest <= 0)
                                        pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = 0;
                                    
                                    else if (pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest >= pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_max)
                                        pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_max;
                                    
                                    // Limitar vida pokemons inimigos entre 0 e max
                                    if (pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest <= 0)
                                        pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = 0;
                                    
                                    else if (pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest >= pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_max)
                                        pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_max;

                                    
                                    printf("\n\t%s (vida: %.0f / %d)\n", pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].nome, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_max);
                                    ler_arte_ascii(caminho_arquivo, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] +1].linha_inicial_inimigo, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] +1].linha_final_inimigo);

                                    ler_arte_ascii(caminho_arquivo, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].linha_inicial_aliado, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].linha_final_aliado);
                                    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t%s (vida: %.0f / %d)\n", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].nome, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_max);
                                    

                                    printf("\n\t\t/⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\\");
                                    printf("\n\t\t⎸\t");
                                    if (numero_rodadas == 4)
                                        {
                                            if (pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].velocidade >= pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1]].velocidade)
                                                {
                                                    if (item_usado_aliado == 0)
                                                        {
                                                            printf("%s usou %s.", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].nome, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].ataques[ataque_usado_aliado -1].nome_ataque);
                                                            pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest - resDanoInimigo;
                                                        }

                                                    else if (item_usado_aliado > 0)
                                                        {
                                                            printf("%s usou ", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].nome);
                                                            if (item_usado_aliado == 1) 
                                                                {   printf("%s.", item_aliado[1-1].nome_item); pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest + 20;     }
                                                            else if (item_usado_aliado == 2) 
                                                                {   printf("%s.", item_aliado[2-1].nome_item); pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest + 20;     }
                                                            else if (item_usado_aliado == 3) 
                                                                {   printf("%s.", item_aliado[3-1].nome_item); pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest + 50;     }
                                                            else if (item_usado_aliado == 4) 
                                                                {   printf("%s.", item_aliado[4-1].nome_item); pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest + 1000;     }
                                                            else if (item_usado_aliado == 5) 
                                                                {   printf("%s.", item_aliado[5-1].nome_item); pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest + 20;     }
                                                            else if (item_usado_aliado == 6) 
                                                                {   printf("%s.", item_aliado[6-1].nome_item); pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest + 20;     }
                                                            else if (item_usado_aliado == 7) 
                                                                {   printf("%s.", item_aliado[7-1].nome_item); pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest + 20;     }
                                                            else if (item_usado_aliado == 8) 
                                                                {   printf("%s.", item_aliado[8-1].nome_item); pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest + 10;     }
                                                        }
                                                }
                                            else
                                                {
                                                    if (item_usado_inimigo == 0)
                                                        {
                                                            printf("%s usou %s.", pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].nome, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].ataques[ataque_usado_inimigo -1].nome_ataque);
                                                            pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest - resDanoAliado;
                                                        }
                                                    else if (item_usado_inimigo > 0)
                                                        {
                                                            printf("%s usou ", pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].nome);
                                                            if (item_usado_inimigo == 1) 
                                                                {   printf("%s.", item_aliado[1-1].nome_item); pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest + 20;     }
                                                            else if (item_usado_inimigo == 2) 
                                                                {   printf("%s.", item_aliado[2-1].nome_item); pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest + 50;     }
                                                            else if (item_usado_inimigo == 3) 
                                                                {   printf("%s.", item_aliado[3-1].nome_item); pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest + 1000;     }
                                                            else if (item_usado_inimigo == 4) 
                                                                {   printf("%s.", item_aliado[4-1].nome_item); pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest + 20;     }
                                                            else if (item_usado_inimigo == 5) 
                                                                {   printf("%s.", item_aliado[5-1].nome_item); pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest + 20;     }
                                                            else if (item_usado_inimigo == 6) 
                                                                {   printf("%s.", item_aliado[6-1].nome_item); pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest + 20;     }
                                                            else if (item_usado_inimigo == 7) 
                                                                {   printf("%s.", item_aliado[7-1].nome_item); pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest + 20;     }
                                                            else if (item_usado_inimigo == 8) 
                                                                {   printf("%s.", item_aliado[8-1].nome_item); pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest + 10;     }
                                                        }
                                                }
                                        }
                                    
                                    else if (numero_rodadas == 3)
                                        {
                                            if (pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].velocidade >= pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1]].velocidade)
                                                {
                                                    if (status_usado_aliado == 0)
                                                        {
                                                            if (item_usado_aliado == 0)
                                                                {
                                                                    printf("O ataque deu %.1f em %s", resDanoInimigo, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].nome);
                                                                }
                                                        }
                                                    else
                                                        {
                                                            if (item_usado_aliado == 0)
                                                                {
                                                                    printf("O movimento ");
                                                                    if (status_usado_aliado == 1)
                                                                        {
                                                                            if (pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] == 1)
                                                                                {   if (ataque_usado_aliado == 3)
                                                                                        {printf("deixou %s paralizado.",pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].nome);}   }
                                                                            if (pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] == 2)
                                                                                {   if (ataque_usado_aliado == 2)
                                                                                        {printf("reduziu a defesa de %s.",pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].nome);}     }
                                                                            if (pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] == 3)
                                                                                {   if (ataque_usado_aliado == 2)
                                                                                        {printf("aumentou o dano do especial do seu pokémon.");}
                                                                                    if (ataque_usado_aliado == 3)
                                                                                        {if (rand() % 100 < 75) {printf("colocou %s para dormir.", pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].nome);}
                                                                                        else printf("não fez efeito.");}
                                                                                }
                                                                            if (pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] == 4)
                                                                                {   if (ataque_usado_aliado == 3)
                                                                                        {}
                                                                                }

                                                                        }
                                                                }
                                                        }
                                                }
                                        }
                                    
                                    else if (numero_rodadas == 2)
                                        {
                                            if (pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].velocidade < pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1]].velocidade)
                                                {
                                                    if (item_usado_aliado == 0)
                                                        {
                                                            printf("%s usou %s.", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].nome, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].ataques[ataque_usado_aliado -1].nome_ataque);
                                                            pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest - resDanoInimigo;
                                                        }

                                                    else if (item_usado_aliado > 0)
                                                        {
                                                            printf("%s usou ", pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].nome);
                                                            if (item_usado_aliado == 1) 
                                                                {   printf("%s.", item_aliado[1-1].nome_item); pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest + 20;     }
                                                            else if (item_usado_aliado == 2) 
                                                                {   printf("%s.", item_aliado[2-1].nome_item); pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest + 20;     }
                                                            else if (item_usado_aliado == 3) 
                                                                {   printf("%s.", item_aliado[3-1].nome_item); pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest + 50;     }
                                                            else if (item_usado_aliado == 4) 
                                                                {   printf("%s.", item_aliado[4-1].nome_item); pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest + 1000;     }
                                                            else if (item_usado_aliado == 5) 
                                                                {   printf("%s.", item_aliado[5-1].nome_item); pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest + 20;     }
                                                            else if (item_usado_aliado == 6) 
                                                                {   printf("%s.", item_aliado[6-1].nome_item); pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest + 20;     }
                                                            else if (item_usado_aliado == 7) 
                                                                {   printf("%s.", item_aliado[7-1].nome_item); pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest + 20;     }
                                                            else if (item_usado_aliado == 8) 
                                                                {   printf("%s.", item_aliado[8-1].nome_item); pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest + 10;     }
                                                        }
                                                }
                                            else
                                                {
                                                    if (item_usado_inimigo == 0)
                                                        {
                                                            printf("%s usou %s.", pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].nome, pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].ataques[ataque_usado_inimigo -1].nome_ataque);
                                                            pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest = pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].vida_rest - resDanoAliado;
                                                        }
                                                    else if (item_usado_inimigo > 0)
                                                        {
                                                            printf("%s usou ", pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].nome);
                                                            if (item_usado_inimigo == 1) 
                                                                {   printf("%s.", item_aliado[1-1].nome_item); pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest + 20;     }
                                                            else if (item_usado_inimigo == 2) 
                                                                {   printf("%s.", item_aliado[2-1].nome_item); pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest + 50;     }
                                                            else if (item_usado_inimigo == 3) 
                                                                {   printf("%s.", item_aliado[3-1].nome_item); pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest + 1000;     }
                                                            else if (item_usado_inimigo == 4) 
                                                                {   printf("%s.", item_aliado[4-1].nome_item); pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest + 20;     }
                                                            else if (item_usado_inimigo == 5) 
                                                                {   printf("%s.", item_aliado[5-1].nome_item); pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest + 20;     }
                                                            else if (item_usado_inimigo == 6) 
                                                                {   printf("%s.", item_aliado[6-1].nome_item); pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest + 20;     }
                                                            else if (item_usado_inimigo == 7) 
                                                                {   printf("%s.", item_aliado[7-1].nome_item); pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest + 20;     }
                                                            else if (item_usado_inimigo == 8) 
                                                                {   printf("%s.", item_aliado[8-1].nome_item); pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest = pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].vida_rest + 10;     }
                                                        }
                                                }
                                        }
                                    
                                    else if (numero_rodadas == 1)
                                        {
                                            if (pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1]].velocidade < pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1]].velocidade)
                                                {
                                                    if (status_usado_inimigo == 0)
                                                        {
                                                            if (item_usado_aliado == 0)
                                                                {
                                                                    printf("O ataque deu %.1f em %s", resDanoAliado, pokemons_aliados[pokemonsSelecionadosAliados[pokemonSelecionadoAliado_jogo -1] -1].nome);
                                                                }
                                                        }
                                                    else
                                                        {
                                                            if (item_usado_inimigo == 0)
                                                                {
                                                                    printf("O movimento ");
                                                                    if (status_usado_inimigo == 1)
                                                                        {
                                                                            if (pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] == 1)
                                                                                {   if (ataque_usado_inimigo == 3)
                                                                                        {printf("deixou %s paralizado.",pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].nome);}   }
                                                                            if (pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] == 2)
                                                                                {   if (ataque_usado_inimigo == 2)
                                                                                        {printf("reduziu a defesa de %s.",pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].nome);}     }
                                                                            if (pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] == 3)
                                                                                {   if (ataque_usado_inimigo == 2)
                                                                                        {printf("aumentou o dano do especial do seu pokémon.");}
                                                                                    if (ataque_usado_inimigo == 3)
                                                                                        {if (rand() % 100 < 75) {printf("colocou %s para dormir.", pokemons_inimigos[pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] -1].nome);}
                                                                                        else printf("não fez efeito.");}
                                                                                }
                                                                            if (pokemonsSelecionadosInimigos[pokemonSelecionadoInimigo_jogo -1] == 4)
                                                                                {   if (ataque_usado_inimigo == 3)
                                                                                        {}
                                                                                }

                                                                        }
                                                                }
                                                        }
                                                }
                                        }
                                    printf("\n\t\t⎸\t");
                                    printf("\n\t\t⎸\t");
                                    printf("\n\t\t\\________________________________________________________________________________________________________/");
                                    printf("\n\n\t\t\t\t\t\t\t\t\t\t\t\t> Ok <Enter> ");
                                    getchar();
                                    limpar_terminal();
                                }
                            if (numero_rodadas == 0)
                                menu_jogo = 0;
                        }
                }

            if (menu_principal == 1 || menu_principal == 3)
                {
                    continue;
                }
    }
    
    while (menu_principal == 1 || menu_principal == 3);
    
}




/********************* Estagios: *********************\
1. Fazer ○ menu do jogo                             ok
    1.1 fazer as configs dos pokemons               ok
    1.2 fazer a variação da taxa crítica
2.Fazer a mecânica do jogo
3. Fazer os botoes                                  ++-
4. fazer o aleatório                                ok
5. fazer os pokemons ficarem aleatórios             ok
6. Fazer um arquivo com as variáveis do pokemon     ok
7. Fazer uma estrutura para cada pokemon            ok ++-
8. Fazer os pokemons efetivos                       em progresso

\*****************************************************/