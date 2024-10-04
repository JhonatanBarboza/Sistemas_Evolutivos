#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdlib>

// Estruturas para Predador e Presa
struct Presa {
    float x, y;
    float velocidade;
    float dx, dy;  // Direção de movimento
    float alcance_visao;
    float energia;
    bool viva;
    float aptidao;  // Novo atributo para aptidão
};

struct Predador {
    float x, y;
    float velocidade;
    float dx, dy;  // Direção de movimento
    float alcance_visao;
    float energia;
    int capturas;
    bool vivo;
    float aptidao;  // Novo atributo para aptidão
};

// Variáveis Globais
const int NUM_PRESAS = 1000;
const int NUM_PREDADORES = 5;
const float MUTACAO = 0.1f;  // Taxa de mutação
Presa presas[NUM_PRESAS];
Predador predadores[NUM_PREDADORES];
int geracao = 1;  // Contador de gerações
int bordas = 600;

// Funções auxiliares
float distancia(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - y1) + (y2 - y1) * (y2 - y1));
}

void rebote_borda(float &x, float &y, float &dx, float &dy, float limite_min, float limite_max, float velocidade) {
    // Verificar colisão com bordas horizontais
    if (x <= limite_min) {
        x = limite_min;  // Corrigir posição
        dx = fabs(dx);   // Inverter para a direita (valor positivo)
    }
    if (x >= limite_max) {
        x = limite_max;
        dx = -fabs(dx);  // Inverter para a esquerda (valor negativo)
    }

    // Verificar colisão com bordas verticais
    if (y <= limite_min) {
        y = limite_min;
        dy = fabs(dy);   // Inverter para cima (valor positivo)
    }
    if (y >= limite_max) {
        y = limite_max;
        dy = -fabs(dy);  // Inverter para baixo (valor negativo)
    }

    // Atualizar a posição com a nova direção após o rebote
    x += dx * velocidade;
    y += dy * velocidade;
}

// Função para gerar um ângulo aleatório entre 0 e 360 graus
float gerar_angulo_aleatorio() {
    float angulo = (rand() % 90) * M_PI / 180.0;  // Converte para radianos
    return angulo;
}


// Função para atualizar as direções de movimento com uma variação de ângulo
void fugir(Presa &presa, const Predador &predador_mais_proximo) {
    float dx = presa.x - predador_mais_proximo.x;
    float dy = presa.y - predador_mais_proximo.y;
    float dist = distancia(presa.x, presa.y, predador_mais_proximo.x, predador_mais_proximo.y);

    if (dist > 0) {
        // Calcular a direção normalizada na direção oposta ao predador
        float direcao_x = dx / dist;
        float direcao_y = dy / dist;

        // Gerar um ângulo aleatório para variar a direção de fuga
        float angulo = gerar_angulo_aleatorio();
        
        // Aplicar o ângulo de variação à direção de fuga
        float novo_dx = cos(angulo) * direcao_x - sin(angulo) * direcao_y;
        float novo_dy = sin(angulo) * direcao_x + cos(angulo) * direcao_y;

        // Mover a presa na nova direção com a variação angular
        float mov_x = novo_dx * presa.velocidade;
        float mov_y = novo_dy * presa.velocidade;

        presa.x += mov_x;
        presa.y += mov_y;

        // Calcular a distância percorrida usando Pitágoras
        float distancia_percorrida = sqrt(mov_x * mov_x + mov_y * mov_y);

        // Consumo de energia proporcional à distância percorrida
        presa.energia -= distancia_percorrida * 0.022222f;  // Ajuste o fator 0.01 conforme necessário
        if(presa.energia == 0){
            presa.viva = false;
        }

        // Verificar bordas e rebater se necessário
        rebote_borda(presa.x, presa.y, presa.dx, presa.dy, 0.0f, bordas, presa.velocidade);
    }
}

// Função para verificar a proximidade das bordas e simular "predadores virtuais"
void fugir_das_bordas(Presa &presa, float limite_min, float limite_max) {
    Predador borda_virtual;

    // Verificar se a presa está perto da borda esquerda ou direita
    if (presa.x <= limite_min) {
        borda_virtual.x = limite_min - 1.0f;  // "Predador virtual" na borda esquerda
        borda_virtual.y = presa.y;  // Mesma posição vertical
    } else if (presa.x >= limite_max) {
        borda_virtual.x = limite_max + 1.0f;  // "Predador virtual" na borda direita
        borda_virtual.y = presa.y;
    }

    // Verificar se a presa está perto da borda superior ou inferior
    if (presa.y <= limite_min) {
        borda_virtual.x = presa.x;
        borda_virtual.y = limite_min - 1.0f;  // "Predador virtual" na borda inferior
    } else if (presa.y >= limite_max) {
        borda_virtual.x = presa.x;
        borda_virtual.y = limite_max + 1.0f;  // "Predador virtual" na borda superior
    }

    // Se a presa estiver perto de uma borda, fugirá dela
    fugir(presa, borda_virtual);
}


// Movimento inteligente do predador (perseguição)
void perseguir(Predador &predador, const Presa &presa_mais_proxima) {
    float dx = presa_mais_proxima.x - predador.x;
    float dy = presa_mais_proxima.y - predador.y;
    float dist = distancia(predador.x, predador.y, presa_mais_proxima.x, presa_mais_proxima.y);

    if (dist > 0) {
        // Mover em direção à presa
        predador.x += (dx / dist) * predador.velocidade;
        predador.y += (dy / dist) * predador.velocidade;
        predador.energia -= 0.002f;  // Consumo de energia ao perseguir

        // Verificar bordas e rebater se necessário
        rebote_borda(predador.x, predador.y, predador.dx, predador.dy, 0.0f, bordas, predador.velocidade);
    }
}

// Movimento aleatório
void mover_aleatorio(float &x, float &y, float velocidade) {
    x += (rand() % 1 - 1) * velocidade;
    y += (rand() % 1 - 1) * velocidade;
}

// Movimento aleatório
void mover_aleatorio_predador(float &x, float &y, float velocidade) {
    x += 0;//(rand() % 1 - 1) * velocidade;
    y += 0;//(rand() % 1 - 1) * velocidade;
}


// Atualiza a posição de uma presa (fuga ou movimento aleatório)
void mover_presa(Presa &presa) {
    if (!presa.viva) return;

    // Encontra o predador mais próximo dentro do raio de visão
    bool predador_visto = false;
    Predador predador_mais_proximo;
    float menor_distancia = presa.alcance_visao;

    for (int i = 0; i < NUM_PREDADORES; i++) {
        if (predadores[i].vivo) {
            float dist = distancia(presas->x, presas->y, predadores[i].x, predadores[i].y);
            if (dist < menor_distancia) {
                menor_distancia = dist;
                predador_mais_proximo = predadores[i];
                predador_visto = true;
            }
        }
    }

    if (predador_visto) {
        // Fugir do predador mais próximo
        fugir(presa, predador_mais_proximo);
    } else {
        // Movimento aleatório
        mover_aleatorio(presa.x, presa.y, presa.velocidade);
    }

    // Energia se esgota ao se mover
    presa.energia -= 0.00f;
    if (presa.energia <= 0) {
        presa.viva = false;  // A presa morre se a energia acabar
    }
}

// Atualiza a posição de um predador (perseguição ou movimento aleatório)
void mover_predador(Predador &predador) {
    if (!predador.vivo) return;

    // Encontra a presa mais próxima dentro do raio de visão
    bool presa_vista = false;
    Presa presa_mais_proxima;
    float menor_distancia = predador.alcance_visao;

    for (int i = 0; i < NUM_PRESAS; i++) {
        if (presas[i].viva) {
            float dist = distancia(predador.x, predador.y, presas[i].x, presas[i].y);
            if (dist < menor_distancia) {
                menor_distancia = dist;
                presa_mais_proxima = presas[i];
                presa_vista = true;
            }
        }
    }

    if (presa_vista) {
        // Perseguir a presa mais próxima
        perseguir(predador, presa_mais_proxima);

        // Verifica se a presa foi capturada
        if (distancia(predador.x, predador.y, presa_mais_proxima.x, presa_mais_proxima.y) < 1.5f) {
            presa_mais_proxima.viva = false;  // Predador captura a presa
            predador.capturas++;  // Incrementa capturas do predador
            predador.energia += 0.5f;  // O predador ganha energia ao capturar uma presa
        }
    } else {
        // Movimento aleatório
        mover_aleatorio_predador(predador.x, predador.y, predador.velocidade);
    }

    // Energia se esgota ao se mover
    predador.energia -= 0.002f;
    if (predador.energia <= 0) {
        predador.vivo = false;  // O predador morre se a energia acabar
    }
}

// Inicialização
void inicializar_simulacao() {
    for (int i = 0; i < NUM_PRESAS; i++) {
        presas[i].x = rand() % bordas;
        presas[i].y = rand() % bordas;
        presas[i].velocidade = 0.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 1.0f));  // Aleatoriza a velocidade inicial
        presas[i].alcance_visao = 0.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f));  // Aleatoriza o alcance de visão
        presas[i].energia = 15.0f;  // Energia inicial
        presas[i].viva = true;
    }
    for (int i = 0; i < NUM_PREDADORES; i++) {
        predadores[i].x = rand() % bordas;
        predadores[i].y = rand() % bordas;
        predadores[i].velocidade = 0.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 3.0f));  // Aleatoriza a velocidade inicial
        predadores[i].alcance_visao = 0.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 3.0f));  // Aleatoriza o alcance de visão
        predadores[i].energia = 12.5f;  // Energia inicial
        predadores[i].vivo = true;
        predadores[i].capturas = 0;
    }
}



// Função para avaliar aptidão de presas e predadores
void avaliar_aptidao() {
    // Avalia a aptidão dos predadores (baseada no número de capturas)
    for (int i = 0; i < NUM_PREDADORES; i++) {
        predadores[i].aptidao = predadores[i].capturas;  // Aptidão baseada nas capturas
    }

    // Avalia a aptidão das presas (baseada no tempo de sobrevivência, que é a energia restante)
    for (int i = 0; i < NUM_PRESAS; i++) {
        presas[i].aptidao = presas[i].energia;  // Aptidão baseada na energia restante
    }
}


// Função para gerar uma nova geração de presas e predadores
void gerar_nova_geracao() {
    void inicializar_simulacao();

    // Seleção por elitismo: preservar os melhores indivíduos
    int melhor_predador = 0, melhor_presa = 0;
    float max_aptidao_predador = 0.0f, max_aptidao_presa = 0.0f;

    // Encontrar o melhor predador
    for (int i = 0; i < NUM_PREDADORES; i++) {
        if (predadores[i].aptidao > max_aptidao_predador) {
            max_aptidao_predador = predadores[i].aptidao;
            melhor_predador = i;
        }
    }

    // Encontrar a melhor presa
    for (int i = 0; i < NUM_PRESAS; i++) {
        if (presas[i].aptidao > max_aptidao_presa) {
            max_aptidao_presa = presas[i].aptidao;
            melhor_presa = i;
        }
    }

    // Gerar nova geração de predadores com crossover e mutação
    for (int i = 0; i < NUM_PREDADORES; i++) {
        if (i != melhor_predador) {  // Preserva o melhor
            // Crossover: combina genes do melhor predador com outro
            predadores[i].velocidade = (predadores[i].velocidade + predadores[melhor_predador].velocidade) / 2.0f;
            predadores[i].alcance_visao = (predadores[i].alcance_visao + predadores[melhor_predador].alcance_visao) / 2.0f;

            // Mutação: chance de pequenas variações
            if ((rand() % 100) < 10) {  // 10% de chance de mutação
                predadores[i].velocidade += ((rand() % 100) / 100.0f - 0.5f) * 0.2f;
                predadores[i].alcance_visao += ((rand() % 100) / 100.0f - 0.5f) * 0.2f;
            }
        }
    }

    // Gerar nova geração de presas com crossover e mutação
    for (int i = 0; i < NUM_PRESAS; i++) {
        if (i != melhor_presa) {  // Preserva a melhor
            // Crossover: combina genes da melhor presa com outra
            presas[i].velocidade = (presas[i].velocidade + presas[melhor_presa].velocidade) / 2.0f;
            presas[i].alcance_visao = (presas[i].alcance_visao + presas[melhor_presa].alcance_visao) / 2.0f;

            // Mutação: chance de pequenas variações
            if ((rand() % 100) < 10) {  // 10% de chance de mutação
                presas[i].velocidade += ((rand() % 100) / 100.0f - 0.5f) * 0.2f;
                presas[i].alcance_visao += ((rand() % 100) / 100.0f - 0.5f) * 0.2f;
            }
        }
    }
}

void atualizar_simulacao() {
    
    for (int i = 0; i < NUM_PREDADORES; i++) {
        if (predadores[i].vivo) {
            Presa *presa_mais_proxima = nullptr;
            float menor_distancia = 9999.0f;

            // Encontrar a presa mais próxima
            for (int j = 0; j < NUM_PRESAS; j++) {
                if (presas[j].viva) {
                    float dist = distancia(predadores[i].x, predadores[i].y, presas[j].x, presas[j].y);
                    if (dist < menor_distancia) {
                        menor_distancia = dist;
                        presa_mais_proxima = &presas[j];
                    }
                }
            }

            // Se houver uma presa próxima, persegui-la
            if (presa_mais_proxima != nullptr) {
                perseguir(predadores[i], *presa_mais_proxima);

                // Verificar se o predador encostou na presa (colisão)
                if (menor_distancia < 2.0f) {  // Se a distância for menor que o limite
                    presa_mais_proxima->viva = false;  // Presa morre
                    predadores[i].capturas++;          // Contabilizar captura
                }
            }
        }
    }

    // Movimentar as presas restantes
    for (int i = 0; i < NUM_PRESAS; i++) {
        if (presas[i].viva) {
            Predador *predador_mais_proximo = nullptr;
            float menor_distancia = 9999.0f;

            // Encontrar o predador mais próximo
            for (int j = 0; j < NUM_PREDADORES; j++) {
                if (predadores[j].vivo) {
                    float dist = distancia(presas[i].x, presas[i].y, predadores[j].x, predadores[j].y);
                    if (dist < menor_distancia) {
                        menor_distancia = dist;
                        predador_mais_proximo = &predadores[j];
                    }
                }
            }

            // Se houver um predador próximo, fugir
            if (predador_mais_proximo != nullptr) {
                fugir(presas[i], *predador_mais_proximo);
            }
        }
    }
    /*
    // Avaliar aptidão após a simulação de interação
    avaliar_aptidao();

    if (NUM_PRESAS < 1){
        
        // Gerar nova geração após avaliar a aptidão
        gerar_nova_geracao();
    }
    if (NUM_PREDADORES < 1 || NUM_PRESAS <= 10){
        
        // Gerar nova geração após avaliar a aptidão
        gerar_nova_geracao();
    }*/
}

// Desenho na Tela
void exibir() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenhar as presas
    glColor3f(1.0f, 1.0f, 1.0f);  // Cinsa para presas
    for (int i = 0; i < NUM_PRESAS; i++) {
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
    glColor3f(1.0f, 0.5f, 0.0f);  // Laranja para predadores
    for (int i = 0; i < NUM_PREDADORES; i++) {
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
    glutTimerFunc(1000 / 100, timer, 0);  // 60 FPS
}

// Configurações do OpenGL
void inicializarOpenGL() {
    glClearColor(0.0, 0.0, 0.0, 1.0);  // Fundo preto
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, bordas, 0, bordas);  // Ambiente 2D 
}

int main(int argc, char** argv) {
    srand(time(0));  // Inicializar o gerador de números aleatórios

    // Inicializar GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Simulação Predador e Presa com Perseguição e Fuga");

    // Inicializar simulação e OpenGL
    inicializar_simulacao();
    inicializarOpenGL();

    // Registrar callbacks
    glutDisplayFunc(exibir);
    glutTimerFunc(0, timer, 0);

    // Iniciar loop principal
    glutMainLoop();
    return 0;
}
