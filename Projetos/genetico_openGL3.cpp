#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <cmath>     // Para usar a função sin()
#include <algorithm> // Para usar std::max e std::min

#define TamPop 20    // Tamanho da população
#define maxx 48   // Valor máximo do domínio da função em X
#define minx -51       // Valor mínimo do domínio da função em X
#define maxy 56   // Valor máximo do domínio da função em Y
#define miny -50       // Valor mínimo do domínio da função em Y

float TaxMut = 0.1;   // Taxa de mutação (ajustada)
float ind[TamPop + 1];    // Indivíduos
float fit[TamPop + 1];  // Aptidão dos indivíduos
int gen = 1;            // Geração
int numger = 50;        // Número de gerações

// Função de aptidão com vários máximos e mínimos locais e um máximo global
float fitnessFunction(float x) {
    return sin(x)*((x*x)/(45));
}

// Função para desenhar o gráfico
void display() {
    glClear(GL_COLOR_BUFFER_BIT);  // Limpa a tela

    // Desenha a curva da função de aptidão
    glColor3f(0.0, 0.0, 0.0); // Cor preta para a linha da função
    glBegin(GL_LINE_STRIP);
    for (float x = minx; x <= maxx; x += 0.1) {
        float y = fitnessFunction(x);
        glVertex2f(x, y);
    }
    glEnd();

    // Plota os pontos da população atual sobre a curva
    glPointSize(5.0);
    glBegin(GL_POINTS);
    for (int i = 1; i <= TamPop; i++) {
        float y = fitnessFunction(ind[i]);  // Calcula y para garantir que o ponto esteja na curva
        glVertex2f(ind[i], y);
    }
    glEnd();

    glFlush();  // Atualiza o buffer para exibir o desenho
}

// Função de inicialização do OpenGL
void initOpenGL() {
    glClearColor(1.0, 1.0, 1.0, 1.0);  // Define a cor de fundo (branco)
    glMatrixMode(GL_PROJECTION);       // Muda para a matriz de projeção
    glLoadIdentity();                  // Reseta a matriz de projeção
    gluOrtho2D(minx, maxx, miny, maxy);  // Define o sistema de coordenadas
}

// Função para iniciar a população
void iniciapop(int tampop, float ind[]) {
    srand(time(NULL));
    for (int i = 1; i <= tampop; i++) {
        ind[i] = minx + rand() % (maxx - minx + 1);
    }
}

// Função para avaliar a aptidão da população
void avalia(int tampop) {
    printf("Geração %d\n", gen);
    for (int i = 1; i <= tampop; i++) {
        float x = ind[i];
        float y = fitnessFunction(x);  // Função de aptidão
        fit[i] = y;
        printf("\tIndivíduo %d (%f) = %f\n", i, ind[i], fit[i]);
    }
}

// Função de elitismo para gerar nova geração
void elitismo(int tampop) {
    int maxi = 1;
    float maxfit = fit[1];
    
    // Busca pelo melhor indivíduo
    for (int i = 2; i <= tampop; i++) {
        if (fit[i] > maxfit) {
            maxfit = fit[i];
            maxi = i;
        }
    }

    // Criação de nova geração por elitismo
    for (int i = 1; i <= tampop; i++) {
        if (i == maxi) continue;  // Protege o melhor indivíduo

        // Crossover: o melhor indivíduo "transfere" parte de seus genes para os demais
        ind[i] = (ind[i] + ind[maxi]) / 2;

        // Mutação: muda a posição do indivíduo de forma controlada dentro do domínio
        int mutacao = (int)(((rand() % maxx - (maxx / 2.0)) / 100.0) * TaxMut * maxx);
        ind[i] = std::max(static_cast<float>(minx), std::min(static_cast<float>(maxx), static_cast<float>(ind[i] + mutacao)));  // Garantir que permaneça no domínio
    }
}

// Função para ajustar a taxa de mutação ao longo das gerações
void ajustaTaxaMutacao(int tampop) {
    int cont=0;

    for (int i = 0; i < tampop; i++) {
        for (int j = 0; j < tampop; j++) {
            // Arredondar para duas casas decimais
            float fit_i_rounded = std::round(fit[i] * 100.0) / 100.0;
            float fit_j_rounded = std::round(fit[j] * 100.0) / 100.0;

            if (fit_i_rounded == fit_j_rounded)
                cont++;
        }
    }
        // Se 80% ou mais da população for igual, aumentar a taxa de mutação
    if (cont >= (tampop * (tampop - 1) / 2) * 0.6) {
        TaxMut = 10.0; // Aumenta a taxa de mutação
    } else {    
        TaxMut = 0.01; // Mantém a taxa de mutação baixa
    }
   //TaxMut = 0.3;
}

// Função para executar o algoritmo genético e atualizar o gráfico
void runAlgorithm() {
    ajustaTaxaMutacao(TamPop);  // Ajusta a taxa de mutação
    avalia(TamPop);  // Avalia a população atual
    display();       // Atualiza o gráfico
    glutPostRedisplay();  // Solicita uma atualização da tela
    elitismo(TamPop);  // Gera nova população
    gen++;
}

// Função chamada ao pressionar uma tecla
void keyboard(unsigned char key, int x, int y) {
    if (key == 13) {  // Tecla Enter (ASCII 13)
        if (gen <= numger) {
            runAlgorithm();  // Executa o algoritmo para a próxima geração
        } else {
            printf("Número máximo de gerações atingido.\n");
        }
    }
}

int main(int argc, char** argv) {
    iniciapop(TamPop, ind);  // Inicializa a população

    // Inicializa o GLUT e o OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1850, 500);
    glutInitWindowPosition(90, 90);
    glutCreateWindow("Grafico da População e Função de Aptidão");
    initOpenGL();
    glutDisplayFunc(display);   // Define a função de exibição
    glutKeyboardFunc(keyboard); // Define a função para lidar com teclado

    glutMainLoop();  // Entra no loop principal do GLUT

    return 0;
}
