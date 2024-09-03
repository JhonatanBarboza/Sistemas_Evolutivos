#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <cmath>
#include <algorithm>

#define TamPop 150    // Tamanho da população
#define maxx 400      // Valor máximo do domínio da função em X
#define minx -410     // Valor mínimo do domínio da função em X
#define maxy 4000      // Valor máximo do domínio da função em Y
#define miny -4000     // Valor mínimo do domínio da função em Y

constexpr int numger = 150;    // Número de gerações
float TaxMut = 0.001;            // Taxa de mutação
float ind[TamPop + 1];         // Indivíduos
float fit[TamPop + 1];         // Aptidão dos indivíduos
float indtemp[TamPop + 1];
int gen = 1;                   // Geração
int mainWindow, graphWindow;
float vetmaxi[numger], vetmedia[numger];
int cont2 = 0;

// Função de aptidão com vários máximos e mínimos locais e um máximo global
float fitnessFunction(float x) {
    return sin(x) * ((x * x) / 45);
}

// Função para inicializar a janela principal
void initMainWindow() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(minx, maxx, miny, maxy);
}

// Função para inicializar a janela do gráfico
void initGraphWindow() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, numger, miny, maxy);
}

// Função para exibir a janela principal
void displayMainWindow() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenha a curva da função de aptidão
    glColor3f(0.0, 0.0, 0.0);
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
        float y = fitnessFunction(ind[i]);
        glVertex2f(ind[i], y);
    }
    glEnd();

    glFlush();
}

// Função para exibir o gráfico de vetmaxi e vetmedia
void displayGraphWindow() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenha o gráfico do fitness máximo (vetmaxi)
    glColor3f(1.0, 0.0, 0.0); // Cor vermelha
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < gen - 1; i++) {
        glVertex2f(i, vetmaxi[i]);
    }
    glEnd();

    // Desenha o gráfico da média da população (vetmedia)
    glColor3f(0.0, 0.0, 1.0); // Cor azul
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < gen - 1; i++) {
        glVertex2f(i, vetmedia[i]);
    }
    glEnd();

    glFlush();
}

// Função para iniciar a população
void iniciapop(int tampop, float ind[]) {
    srand(time(NULL));  // Inicializa o gerador de números aleatórios com a semente baseada no tempo atual
    int minIntervalo = -50; 
    int maxIntervalo = 50; 

    for (int i = 1; i <= tampop; i++) {
        ind[i] = minIntervalo + rand() % (maxIntervalo - minIntervalo + 1);
    }
}


// Função para avaliar a aptidão da população
void avalia(int tampop) {
    printf("Geração %d\n", gen);
    for (int i = 1; i <= tampop; i++) {
        float x = ind[i];
        float y = fitnessFunction(x);
        fit[i] = y;
        printf("\tIndivíduo %d (%f) = %f\n", i, ind[i], fit[i]);
    }
}
void melhor_media(float maxfit, float media, int tampop){
    // Guarda o fitness do melhor indivíduo em vetmaxi
    vetmaxi[gen-1] = maxfit;

    // Calcula e guarda a média da população em vetmedia
    for (int i = 1; i <= tampop; i++) {
        media += fit[i];
    }
    media = media / tampop;
    vetmedia[gen-1] = media;
    //printf("A média da população é: %f\n", vetmedia[gen-1]);

}

// Função de elitismo para gerar nova geração
void elitismo(int tampop) {
    int maxi = 1;
    float media = 0.0f;
    float maxfit = fit[1];
    
    // Busca pelo melhor indivíduo
    for (int i = 2; i <= tampop; i++) {
        if (fit[i] > maxfit) {
            maxfit = fit[i];
            maxi = i;
        }
    }

    //vetores para plotar grafico do melhor de todos e da media da população
    melhor_media(maxfit, media, tampop);

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
    
    int cont = 0;
        
    for (int i = 0; i < tampop; i++) {
        for (int j = 0; j < tampop; j++) {
            // Arredondar para duas casas decimais
            float fit_i_rounded = std::round(fit[i] * 100.0) / 100.0;
            float fit_j_rounded = std::round(fit[j] * 100.0) / 100.0;

            if (fit_i_rounded == fit_j_rounded)
                cont++;
        }
    }

    if (cont2 == 5){
        TaxMut = 3.0;
        cont2 = 0;
    }
    else{

    // Se 60% ou mais da população for igual, aumentar a taxa de mutação
    if (cont >= (tampop * (tampop - 1) / 2) * 0.6 && gen!=1){
        TaxMut = 0.1;
        cont2++;
    }else{
            TaxMut = 0.001;
        }
    }
    
}

// Função para executar o algoritmo genético e atualizar o gráfico
void runAlgorithm() {
    ajustaTaxaMutacao(TamPop);
    avalia(TamPop);

    // Atualiza a janela principal
    glutSetWindow(mainWindow);
    glutPostRedisplay();
    
    // Atualiza a janela do gráfico
    glutSetWindow(graphWindow);
    glutPostRedisplay();

    //avaliação
    elitismo(TamPop);
    gen++;
    
}

// Função chamada ao pressionar uma tecla
void keyboard(unsigned char key, int x, int y) {
    if (key == 13) {  // Tecla Enter
        if (gen <= numger) {
            runAlgorithm();
        } else {
            printf("Número máximo de gerações atingido.\n");
        }
    }
}

int main(int argc, char** argv) {
    iniciapop(TamPop, ind);

    // Inicializa o GLUT e o OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // Criação da janela principal
    glutInitWindowSize(1850, 500);
    glutInitWindowPosition(50, 50);
    mainWindow = glutCreateWindow("Grafico da Populacao e Funcao de Aptidao");
    initMainWindow();
    glutDisplayFunc(displayMainWindow);
    glutKeyboardFunc(keyboard);

    // Criação da janela do gráfico de vetmaxi e vetmedia
    glutInitWindowSize(800, 300);
    glutInitWindowPosition(1000, 600);
    graphWindow = glutCreateWindow("Grafico do Fitness Maximo e Media por Geracao");
    initGraphWindow();
    glutDisplayFunc(displayGraphWindow);

    glutMainLoop();

    return 0;
}
