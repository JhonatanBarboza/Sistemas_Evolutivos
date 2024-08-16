#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <algorithm>  // Para usar std::max e std::min

#define TamPop 10    // Tamanho da população
#define maxx 20      // Valor máximo do domínio da função (ajustado para o gráfico)
#define minx 0       // Valor mínimo do domínio da função
#define TaxMut 0.1   // Taxa de mutação (ajustada)

int ind[TamPop + 1];    // Indivíduos
float fit[TamPop + 1];  // Aptidão dos indivíduos
int gen = 1;            // Geração
int numger = 10;        // Número de gerações

// Função para desenhar o gráfico
void display() {
    glClear(GL_COLOR_BUFFER_BIT);  // Limpa a tela

    // Desenha a curva da função de aptidão
    glColor3f(0.0, 0.0, 0.0); // Cor branca para a linha da função
    glBegin(GL_LINE_STRIP);
    for (float x = minx; x <= maxx; x += 0.1) {
        float y = -0.2 * (x - 10) * (x - 10) + 10;
        glVertex2f(x, y);
    }
    glEnd();

    // Plota os pontos da população atual sobre a curva
    glPointSize(5.0);
    glColor3f(0.0, 0.0, 1.0); // Cor azul para os pontos
    glBegin(GL_POINTS);
    for (int i = 1; i <= TamPop; i++) {
        float y = -0.2 * (ind[i] - 10) * (ind[i] - 10) + 10;  // Calcula y para garantir que o ponto esteja na curva
        glVertex2f(ind[i], y);
    }
    glEnd();

    glFlush();  // Atualiza o buffer para exibir o desenho
}

// Função de inicialização do OpenGL
void initOpenGL() {
    glClearColor(1.0, 1.0, 1.0, 1.0);  // Define a cor de fundo (preto)
    glMatrixMode(GL_PROJECTION);       // Muda para a matriz de projeção
    glLoadIdentity();                  // Reseta a matriz de projeção
    gluOrtho2D(minx, maxx, -10.0, 15.0);  // Define o sistema de coordenadas
}

// Função para iniciar a população
void iniciapop(int tampop, int ind[]) {
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
        float y = -0.2 * (x - 10) * (x - 10) + 10;  // Função de aptidão
        fit[i] = y;
        printf("\tIndivíduo %d (%d) = %f\n", i, ind[i], fit[i]);
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
        ind[i] = std::max(minx, std::min(maxx, ind[i] + mutacao));  // Garantir que permaneça no domínio
    }
}

// Função para executar o algoritmo genético e atualizar o gráfico
void runAlgorithm() {
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
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Grafico da População e Função de Aptidão");
    initOpenGL();
    glutDisplayFunc(display);   // Define a função de exibição
    glutKeyboardFunc(keyboard); // Define a função para lidar com teclado

    glutMainLoop();  // Entra no loop principal do GLUT

    return 0;
}
