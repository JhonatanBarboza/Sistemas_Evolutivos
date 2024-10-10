#include <GL/glut.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include <cmath>

// Estruturas para Predador e Presa
struct Presa {
    float x, y;
    float velocidade;
    float dx, dy;  // Direção de movimento
    int alcance_visao;
    float energia;
    bool viva;
    float aptidao;  // Novo atributo para aptidão
};

struct Predador {
    float x, y;
    float velocidade;
    float dx, dy;  // Direção de movimento
    int alcance_visao;
    float energia;
    int capturas;
    bool vivo;
    float aptidao;  // Novo atributo para aptidão
};

// Variáveis Globais fixas
const int NUM_PRESAS = 30;
const int NUM_PREDADORES = 6;
Presa presas[NUM_PRESAS];
Predador predadores[NUM_PREDADORES];
int geracao = 1;  // Contador de gerações

//variaveis ajustaveis
int NUM_PRESAS_INICIAL = 15;
int NUM_PREDADORES_INICIAL = 3;
int bordas = 200;

//variaveis de controle
int cont_predadores = NUM_PREDADORES_INICIAL-1;
int cont_presas = NUM_PRESAS_INICIAL-1;

// Inicialização
void inicializar_simulacao() {
    //inicializar presas
    for (int i = 0; i < NUM_PRESAS; i++) {
        presas[i].x = rand() % bordas;
        presas[i].y = rand() % bordas;
        presas[i].velocidade = 0.0f;
        presas[i].alcance_visao = 0;
        presas[i].energia = 0.1f;
        presas[i].viva = false;
    }
    for (int i = 0; i < NUM_PRESAS_INICIAL; i++) {
        presas[i].x = rand() % bordas;
        presas[i].y = rand() % bordas;
        presas[i].velocidade = 0.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 1.0));  // Aleatoriza a velocidade inicial
        presas[i].alcance_visao = 0 + rand() % 100;  // Aleatoriza o alcance de visão
        presas[i].viva = true;
    }
    
    //inicializar predadores
    for (int i = 0; i < NUM_PREDADORES; i++) {
        predadores[i].x = rand() % bordas;
        predadores[i].y = rand() % bordas;
        predadores[i].velocidade = 0.0f;   
        predadores[i].alcance_visao = 0.0f; 
        predadores[i].energia = 30.0f; 
        predadores[i].vivo = false;
        predadores[i].capturas = 0;
    }

    for (int i = 0; i < NUM_PREDADORES_INICIAL; i++) {
        predadores[i].x = rand() % bordas;
        predadores[i].y = rand() % bordas;
        predadores[i].velocidade = 0.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f));  // Aleatoriza a velocidade inicial
        predadores[i].alcance_visao = 0 + rand() % 200;  // Aleatoriza o alcance de visão
        predadores[i].vivo = true;
        predadores[i].capturas = 0;
    }
}

// Funções auxiliares
float distancia(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void rebote_borda(float &x, float &y, float &dx, float &dy, float limite_min, float limite_max, float velocidade) {
    if (x <= limite_min) {
        x = limite_min;
        dx = fabs(dx);  // Inverte para a direita
    } else if (x >= limite_max) {
        x = limite_max;
        dx = -fabs(dx);  // Inverte para a esquerda
    }

    if (y <= limite_min) {
        y = limite_min;
        dy = fabs(dy);  // Inverte para cima
    } else if (y >= limite_max) {
        y = limite_max;
        dy = -fabs(dy);  // Inverte para baixo
    }

    // Atualiza a posição
    x += (dx * velocidade);
    y += (dy * velocidade);
}

/*
// Função para teletransportar o indivíduo quando ultrapassa as bordas
void envolver_borda(float &x, float &y, float limite_min, float limite_max, float velocidade) {
    // Verifica se ultrapassou a borda horizontal
    if (x < limite_min) {
        x = limite_max;  // Teletransporta para o lado direito
    } else if (x > limite_max) {
        x = limite_min;  // Teletransporta para o lado esquerdo
    }

    // Verifica se ultrapassou a borda vertical
    if (y < limite_min) {
        y = limite_max;  // Teletransporta para o lado de baixo
    } else if (y > limite_max) {
        y = limite_min;  // Teletransporta para o lado de cima
    }

    // Atualiza a posição
    x += (velocidade);
    y += (velocidade);
}*/

// Função para gerar um ângulo aleatório entre 0 e 360 graus
float gerar_angulo_aleatorio() {
    float angulo = (rand() % 30) * M_PI / 180.0;  // Gera entre 0 e 360 graus em radianos
    return angulo;
}

// Função para remover predador morto e compactar o vetor
void remover_predador_morto(int indice) {
    // Desloca todos os predadores à direita do predador morto uma posição para a esquerda
    for (int j = indice; j <= cont_predadores; j++) {
        predadores[j] = predadores[j + 1];  // Copia o próximo predador para a posição atual
    }
    
    cont_predadores--;  // Diminui o contador de predadores
}

// Função para remover predador morto e compactar o vetor
void remover_presas_mortas(int indice) {
    // Desloca todos os predadores à direita do predador morto uma posição para a esquerda
    for (int j = indice; j <= cont_presas; j++) {
        presas[j] = presas[j + 1];  // Copia o próximo predador para a posição atual
    }
    
    cont_presas--;  // Diminui o contador de predadores
}

// Função para atualizar as direções de movimento com uma variação de ângulo
void fugir(int i, int j) {
    float dx = presas[i].x - predadores[j].x;
    float dy = presas[i].y - predadores[j].y;
    float dist = distancia(presas[i].x, presas[i].y, predadores[j].x, predadores[j].y);

    if (dist > 0) {
        // Calcular a direção normalizada na direção oposta ao predador
        float direcao_x = dx / dist;
        float direcao_y = dy / dist;

        // Gerar um ângulo aleatório para variar a direção de fuga
        float angulo = gerar_angulo_aleatorio();

        // Aplicar o ângulo à direção de fuga
        float novo_dx = direcao_x * cos(angulo) - direcao_y * sin(angulo);
        float novo_dy = direcao_x * sin(angulo) + direcao_y * cos(angulo);

        // Mover a presa na nova direção com a variação angular
        float mov_x = novo_dx * presas[i].velocidade;
        float mov_y = novo_dy * presas[i].velocidade;

        presas[i].x += mov_x;
        presas[i].y += mov_y;

        // Consumo de energia proporcional à distância percorrida (energia diminui)
        presas[i].energia += 0.3f;//************************************************************************* 
        if (presas[i].energia <= 0) {
            //presas[i].viva = false;
            //remover_presas_mortas(i);
        }

        // Atualizar direção de movimento da presa
        presas[i].dx = novo_dx;
        presas[i].dy = novo_dy;

        // Verificar bordas e rebater se necessário
        rebote_borda(presas[i].x, presas[i].y, presas[i].dx, presas[i].dy, 1.0f, bordas - 1, presas[i].velocidade);
    }
}


void perseguir(int i, int j) {
    float dx = presas[j].x - predadores[i].x;
    float dy = presas[j].y - predadores[i].y;
    float dist = distancia(predadores[i].x, predadores[i].y, presas[j].x, presas[j].y);

    if (dist > 0) {
        // Mover em direção à presa
        predadores[i].x += (dx / dist) * (predadores[i].velocidade + 0.2);
        predadores[i].y += (dy / dist) * (predadores[i].velocidade + 0.2);

        predadores[i].energia -= 0.009f; //*************************************************************************

        if (predadores[i].energia < 0.01f) {
            predadores[i].vivo = false;
            remover_predador_morto(i);  
        }

        // Verificar bordas e rebater se necessário
        float limite_min = 1.0f, limite_max = bordas - 1;
        rebote_borda(predadores[i].x, predadores[i].y, predadores[i].dx, predadores[i].dy, limite_min, limite_max, predadores[i].velocidade);
    }
}

// Função para gerar um valor aleatório entre -1 e 1
float gerar_valor_aleatorio_direcao() {
    return ((rand() % 2001) - 1000) / 1000.0f;  // Gera um valor entre -1 e 1
}

// Movimento aleatório da presa
void mover_aleatorio_presa(int i) {
    // Atualizar direções de forma suave com pequenos ajustes aleatórios
    presas[i].dx += gerar_valor_aleatorio_direcao() * 0.05f;  // Pequena variação na direção x (reduzida)
    presas[i].dy += gerar_valor_aleatorio_direcao() * 0.05f;  // Pequena variação na direção y (reduzida)

    // Normalizar as direções para evitar aceleração excessiva
    float magnitude = sqrt(presas[i].dx * presas[i].dx + presas[i].dy * presas[i].dy);
    if (magnitude > 0) {
        presas[i].dx /= magnitude;
        presas[i].dy /= magnitude;
    }

    // Atualizar posição da presa com base na direção e velocidade reduzida
    float fator_velocidade_reduzido = presas[i].velocidade * 0.001f;  // Reduz a velocidade pela metade
    presas[i].x += presas[i].dx * fator_velocidade_reduzido;
    presas[i].y += presas[i].dy * fator_velocidade_reduzido;


    presas[i].energia += 0.3f;//*************************************************************************

    // Verificar bordas e ajustar direção se necessário
    rebote_borda(presas[i].x, presas[i].y, presas[i].dx, presas[i].dy, 1.0f, bordas-1, presas[i].velocidade);
}

// Movimento aleatório// Movimento aleatório do predador
void mover_aleatorio_predador(int i) {
    // Atualizar direções de forma suave com pequenos ajustes aleatórios
    predadores[i].dx += gerar_valor_aleatorio_direcao() * 0.05f;  // Pequena variação na direção x
    predadores[i].dy += gerar_valor_aleatorio_direcao() * 0.05f;  // Pequena variação na direção y

    // Normalizar as direções para evitar aceleração excessiva
    float magnitude = sqrt(predadores[i].dx * predadores[i].dx + predadores[i].dy * predadores[i].dy);
    if (magnitude > 0) {
        predadores[i].dx /= magnitude;
        predadores[i].dy /= magnitude;
    }
    // Atualizar posição da presa com base na direção e velocidade reduzida
    float fator_velocidade_reduzido = predadores[i].velocidade * 0.01f;  // Reduz a velocidade pela metade
    presas[i].x += predadores[i].dx * fator_velocidade_reduzido;
    presas[i].y += predadores[i].dy * fator_velocidade_reduzido;

    predadores[i].energia -= 0.009f; //****************************************************************************

    if (predadores[i].energia < 0.01f){
        predadores[i].vivo = false;
        remover_predador_morto(i);
    }
    // Verificar bordas e ajustar direção se necessário
    rebote_borda(predadores[i].x, predadores[i].y, predadores[i].dx, predadores[i].dy, 1.0f, bordas+1, predadores[i].velocidade);
}

// Função para gerar uma nova geração de predadores
void gerar_filho_predador(int pai, int mae, int i) {

    // Crossover: combina genes do melhor predador (pai) com a melhor predadora (mãe)
    predadores[i].velocidade = (predadores[pai].velocidade + predadores[mae].velocidade) / 2.0f;
    predadores[i].alcance_visao = (predadores[pai].alcance_visao + predadores[mae].alcance_visao) / 2;
        
    //mutação
    predadores[i].velocidade += ((rand() % 100) / 100.0f) * 1.0f - 0.5f;
    predadores[i].alcance_visao += ((rand() % 100) / 100) * 40 - 20;
            
    predadores[i].energia = 30.0f;
    predadores[i].vivo = true; 

    predadores[i].x = predadores[mae].x;
    predadores[i].y = predadores[mae].y;

    predadores[pai].energia -= 20;
    predadores[mae].energia -= 20;
}


// Função para gerar uma nova geração de predadores
void gerar_filho_presa(int pai, int mae, int i) {

    // Crossover: combina genes do melhor predador (pai) com a melhor predadora (mãe)
    presas[i].velocidade = (presas[pai].velocidade + presas[mae].velocidade) / 2.0f;
    presas[i].alcance_visao = (presas[pai].alcance_visao + presas[mae].alcance_visao) / 2;
        
    //mutação
    presas[i].velocidade += ((rand() % 100) / 100.0f) * 1.0f - 0.5f;
    presas[i].alcance_visao += ((rand() % 100) / 100) * 40 - 20;
            
    presas[i].energia = 0.1f;
    presas[i].viva = true; 

    presas[i].x = presas[mae].x;
    presas[i].y = presas[mae].y;

    presas[pai].energia = 0.1;
    presas[mae].energia = 0.1;
}


void atualizar_simulacao() {
    
    // Movimentar predadores
    for (int i = 0; i < NUM_PREDADORES; i++) {
        if (predadores[i].vivo) {
            int presa_mais_proxima = 99999;
            float menor_distancia = 9999.0f;

            // Encontrar a presa mais próxima dentro do alcance de visão do predador
            for (int j = 0; j < NUM_PRESAS; j++) {
                if (presas[j].viva) {
                    float dist = distancia(predadores[i].x, predadores[i].y, presas[j].x, presas[j].y);

                    // Verificar se a presa está dentro do alcance de visão
                    if (dist < predadores[i].alcance_visao && dist < menor_distancia) {
                        menor_distancia = dist;
                        presa_mais_proxima = j;
                    }
                }
            }

            // Se houver uma presa dentro do alcance, persegui-la
            if (presa_mais_proxima != 99999) {

                perseguir(i, presa_mais_proxima);
                // Verificar se o predador encostou na presa (colisão)
                if (menor_distancia < 3.0f) {  // Se a distância for menor que o limite
                    presas[presa_mais_proxima].viva = false;  // Presa morre
                    predadores[i].energia += 8.0f;
                    predadores[i].capturas++;          // Contabilizar captura
                    remover_presas_mortas(presa_mais_proxima);
                }

            } else {
                // Movimento aleatório
                mover_aleatorio_predador(i);
            }
        }
    }

    // Movimentar as presas restantes
    for (int i = 0; i < NUM_PRESAS; i++) {
        if (presas[i].viva) {
            int predador_mais_proximo = 99999;
            float menor_distancia = 9999.0f;

            // Encontrar o predador mais próximo dentro do alcance de visão da presa
            for (int j = 0; j < NUM_PREDADORES; j++) {
                if (predadores[j].vivo) {
                    float dist = distancia(presas[i].x, presas[i].y, predadores[j].x, predadores[j].y);

                    // Verificar se o predador está dentro do alcance de visão (360 graus)
                    if (dist < presas[i].alcance_visao) {
                        if (dist < menor_distancia) {
                            menor_distancia = dist;
                            predador_mais_proximo = j;
                        }
                    }
                }
            }

            // Se houver um predador dentro do alcance, fugir
            if (predador_mais_proximo != 99999) {
                fugir(i, predador_mais_proximo);
            } else {
               mover_aleatorio_presa(i);
            }
        }
    }

    
        int pai = 0, mae = 0;
        geracao++;
        int fleg = 0;
        for (int i=0 ; i<cont_presas+1 ; i++){
            if (cont_presas >= NUM_PRESAS){
                break;
            }
            if (presas[i].viva){
                if (fleg == 0 && presas[i].energia >= 15.0f){
                    fleg = 1;
                    pai = i;
                }
                else{ 
                    if (fleg == 1 && presas[i].energia >= 15.0f){
                    fleg = 2;
                    mae = i;
                    }
                }
                if (fleg == 2){
                    gerar_filho_presa(pai, mae, cont_presas);
                    cont_presas++;
                    fleg = 0;
                    pai = -1;
                    mae = -1;
                }
            }
        }

        fleg = 0;
        pai = 0;
        mae = 0;

        for (int i=0 ; i<cont_predadores+1 ; i++){
            if (cont_predadores >= NUM_PREDADORES){
                break;
            }
            if (predadores[i].vivo){

                if (fleg == 0 && predadores[i].energia > 50.0f){
                    fleg = 1;
                    pai = i;
                }
                else{ 
                    if (fleg == 1 && predadores[i].energia > 50.0f){
                    fleg = 2;
                    mae = i;
                    }
                }
                if (fleg == 2){
                    gerar_filho_predador(pai, mae, cont_predadores);
                    cont_predadores++;
                    fleg = 0;
                    pai = 0;
                    mae = 0;
                }
            }
        }
        fleg = 0;

        if (cont_predadores == 0 && cont_presas == 0)
            exit(1);
}



//*********************************************************************************************************

// Desenho na Tela
void exibir() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenhar as presas
    glColor3f(1.0f, 1.0f, 1.0f);  
    for (int i = 0; i < cont_presas+10; i++) {
        if (presas[i].viva) {
            glBegin(GL_QUADS);
            glVertex2f(presas[i].x - 1, presas[i].y - 1);
            glVertex2f(presas[i].x + 1, presas[i].y - 1);
            glVertex2f(presas[i].x + 1, presas[i].y + 1);
            glVertex2f(presas[i].x - 1, presas[i].y + 1);
            glEnd();
        }
    }

    // Desenhar os predadores
    glColor3f(1.0f, 0.5f, 0.0f);  
    for (int i = 0; i < cont_predadores+10; i++) {
        if (predadores[i].vivo) {
            glBegin(GL_QUADS);
            glVertex2f(predadores[i].x - 1.5f, predadores[i].y - 1.5f);
            glVertex2f(predadores[i].x + 1.5f, predadores[i].y - 1.5f);
            glVertex2f(predadores[i].x + 1.5f, predadores[i].y + 1.5f);
            glVertex2f(predadores[i].x - 1.5f, predadores[i].y + 1.5f);
            glEnd();
        }
    }

    glutSwapBuffers();
}

// Função Timer
void timer(int valor) {
    atualizar_simulacao();
    glutPostRedisplay();
    glutTimerFunc(1000 / 30, timer, 0);  // 60 FPS
}

// Configurações do OpenGL
void inicializarOpenGL() {
    glClearColor(0.0, 0.0, 0.0, 1.0);  // Fundo preto
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, bordas, 0, bordas);  // Ambiente 2D 
}

//*********************************************************************************************************

int main(int argc, char** argv) {
    srand(time(0));  // Inicializar o gerador de números aleatórios

    // Inicializar GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Simulacao Predador e Presa");

    // Inicializar simulação e OpenGL
    if (geracao == 1)
        inicializar_simulacao();
    inicializarOpenGL();

    // Registrar callbacks
    glutDisplayFunc(exibir);
    glutTimerFunc(0, timer, 0);

    // Iniciar loop principal
    glutMainLoop();
    return 0;
}
