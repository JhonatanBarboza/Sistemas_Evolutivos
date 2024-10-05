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
const int NUM_PRESAS = 50;
const int NUM_PREDADORES = 3;
const float MUTACAO = 0.1f;  // Taxa de mutação
Presa presas[NUM_PRESAS];
Predador predadores[NUM_PREDADORES];
int geracao = 1;  // Contador de gerações
int bordas = 300;

// Funções auxiliares
float distancia(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void rebote_borda(float &x, float &y, float &dx, float &dy, float limite_min, float limite_max, float velocidade) {
    if (x <= limite_min) {
        x = limite_min;
        dx = fabs(dx);  // Inverter para a direita
    }
    if (x >= limite_max) {
        x = limite_max;
        dx = -fabs(dx);  // Inverter para a esquerda
    }
    if (y <= limite_min) {
        y = limite_min;
        dy = fabs(dy);  // Inverter para cima
    }
    if (y >= limite_max) {
        y = limite_max;
        dy = -fabs(dy);  // Inverter para baixo
    }

    // Atualizar posição
    x += (dx * velocidade);
    y += (dy * velocidade);
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
        presa.energia -= distancia_percorrida * 0.0f;  // Ajuste o fator 0.01 conforme necessário
        if(presa.energia == 0){
            presa.viva = false;
        }

        // Verificar bordas e rebater se necessário
        rebote_borda(presa.x, presa.y, presa.dx, presa.dy, 2.0f, bordas-2, presa.velocidade);
    }
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
        
        // Consumo de energia proporcional à distância percorrida
        float distancia_percorrida = predador.velocidade;
        predador.energia -= distancia_percorrida * 0.2f;  // Ajustar o fator conforme necessário

        // Verificar bordas e rebater se necessário
        rebote_borda(predador.x, predador.y, predador.dx, predador.dy, 2.0f, bordas-2, predador.velocidade);
    }
}

/*
// Movimento aleatório
void mover_aleatorio_presa(int i) {
    presas[i].x += (rand() % 1) * presas[i].velocidade;
    presas[i].y += (rand() % 2) * presas[i].velocidade;
    //presas[i].dx += (rand() % 1 - 1) * presas[i].velocidade;
    //presas[i].dy += (rand() % 1 - 1) * presas[i].velocidade;

    rebote_borda(presas[i].x, presas[i].y, presas[i].dx, presas[i].dy, 0.0f, bordas, presas[i].velocidade);

}
*/

// Movimento aleatório
void mover_aleatorio_predador(int i) {      
    static float angulo = 0.0f;  // Variável estática para reter valor entre chamadas
    float incremento_angular = 0.0005f;  // Controle o tamanho da mudança de ângulo (ajustar conforme necessário)
    
    // Incrementar o ângulo para criar a trajetória circular
    angulo += incremento_angular;

    // Calcular a nova direção em movimento circular
    float novo_dx = cos(angulo);
    float novo_dy = sin(angulo);

    // Mover o predador na nova direção circular com sua velocidade
    float mov_x = novo_dx * predadores[i].velocidade;
    float mov_y = novo_dy * predadores[i].velocidade;

    // Atualizar posição do predador
    predadores[i].x += mov_x;
    predadores[i].y += mov_y;

        // Calcular a distância percorrida usando Pitágoras
        float distancia_percorrida = sqrt(mov_x * mov_x + mov_y * mov_y);

        // Consumo de energia proporcional à distância percorrida
        predadores[i].energia -= distancia_percorrida * 0.1f;  // Ajuste o fator 0.01 conforme necessário
        if(predadores[i].energia == 0){
            predadores[i].vivo = false;
        }

    rebote_borda(predadores[i].x, predadores[i].y, predadores[i].dx, predadores[i].dy, 2.0f, bordas-2, predadores[i].velocidade);
}

/*
// Atualiza a posição de uma presa (fuga ou movimento aleatório)
void mover_presa(int i) {
    if (!presas[i].viva) return;

    // Encontra o predador mais próximo dentro do raio de visão
    bool predador_visto = false;
    Predador predador_mais_proximo;
    float menor_distancia = presas[i].alcance_visao;

    for (int j = 0; j < NUM_PREDADORES; j++) {
        if (predadores[j].vivo) {
            float dist = distancia(presas[i].x, presas[i].y, predadores[j].x, predadores[j].y);
            if (dist < menor_distancia) {
                menor_distancia = dist;
                predador_mais_proximo = predadores[j];
                predador_visto = true;
            }
        }
    }

    if (predador_visto) {
        // Fugir do predador mais próximo
        fugir(presas[i], predador_mais_proximo);
    } else {
        // Movimento aleatório
        mover_aleatorio_presa(i);
    }

    // Energia se esgota ao se mover
    presas[i].energia -= 0.0f;  // Ajuste o consumo de energia conforme necessário
    if (presas[i].energia <= 0) {
        presas[i].viva = false;  // A presa morre se a energia acabar
    }
}

// Atualiza a posição de um predador (perseguição ou movimento aleatório)
void mover_predador(int i) {
    if (!predadores[i].vivo) return;

    // Encontra a presa mais próxima dentro do raio de visão
    bool presa_vista = false;
    int presa_mais_proxima = -1;  // Inicializar com um valor inválido
    float menor_distancia = predadores[i].alcance_visao;

    for (int j = 0; j < NUM_PRESAS; j++) {
        if (presas[j].viva) {  // Corrigir para usar j, não i
            float dist = distancia(predadores[i].x, predadores[i].y, presas[j].x, presas[j].y);

            if (dist < menor_distancia) {
                menor_distancia = dist;
                presa_mais_proxima = j;  // Usar j, não i
                presa_vista = true;
            }
        }
    }

    if (presa_vista && presa_mais_proxima != -1) {
        // Perseguir a presa mais próxima
        perseguir(predadores[i], presas[presa_mais_proxima]);

        // Verifica se a presa foi capturada
        if (distancia(predadores[i].x, predadores[i].y, presas[presa_mais_proxima].x, presas[presa_mais_proxima].y) < 1.5f) {
            presas[presa_mais_proxima].viva = false;  // Predador captura a presa
            predadores[i].capturas++;  // Incrementa capturas do predador
            predadores[i].energia += 0.05f;  // O predador ganha energia ao capturar uma presa
        }
    } else {
        // Movimento aleatório
        mover_aleatorio_predador(i);
    }

    // Energia se esgota ao se mover
    predadores[i].energia -= 0.002f;
    if (predadores[i].energia <= 0) {
        predadores[i].vivo = false;  // O predador morre se a energia acabar
    }
}*/


// Inicialização
void inicializar_simulacao() {
    for (int i = 0; i < NUM_PRESAS; i++) {
        presas[i].x = rand() % bordas;
        presas[i].y = rand() % bordas;
        presas[i].velocidade = 0.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.5));  // Aleatoriza a velocidade inicial
        presas[i].alcance_visao = 0.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 3.0f));  // Aleatoriza o alcance de visão
        presas[i].energia = 5.0f;  // Energia inicial
        presas[i].viva = true;
    }
    for (int i = 0; i < NUM_PREDADORES; i++) {
        predadores[i].x = rand() % bordas;
        predadores[i].y = rand() % bordas;
        predadores[i].velocidade = 0.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.01f));  // Aleatoriza a velocidade inicial
        predadores[i].alcance_visao = 0.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 3.0f));  // Aleatoriza o alcance de visão
        predadores[i].energia = 5.0f;  // Energia inicial
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

/*
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
}*/

void atualizar_simulacao() {
    
    for (int i = 0; i < NUM_PREDADORES; i++) {
        //mover_predador(i);

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
                if (menor_distancia < 3.0f) {  // Se a distância for menor que o limite
                    presa_mais_proxima->viva = false;  // Presa morre
                    predadores[i].capturas++;          // Contabilizar captura
                }
            }
            else {
                // Movimento aleatório
                mover_aleatorio_predador(i);
            }
        }
        
    }

    // Movimentar as presas restantes
    for (int i = 0; i < NUM_PRESAS; i++) {
        //mover_presa(i);}


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
