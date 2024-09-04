#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>


typedef struct _individuo{
    float x;
    float y;
}individuo;

#define maxx 50     // Valor máximo do domínio da função em X
#define minx -50    // Valor mínimo do domínio da função em X
#define maxy 100      // Valor máximo do domínio da função em Y
#define miny -100     // Valor mínimo do domínio da função em Y
#define TamPop ((maxx +(-minx)))    // Tamanho da população

constexpr int numger = 250;    // Número de gerações
float TaxMut;                  // Taxa de mutação
individuo ind[TamPop + 1];     // Indivíduos
//float indtemp[TamPop + 1];
int gen = 1;                   // Geração
int mainWindow, graphWindow;
float vetmaxi[numger], vetmedia[numger];
float Ruido;

// Função de aptidão com vários máximos e mínimos locais e um máximo global
float fitnessFunction(float x) {
    return sin(x+Ruido)*x+cos(x*Ruido)*Ruido;
    //return sin(x+Ruido)*x+cos(x*9.3)*9.3;
    //return sin(x+Ruido) * ((x * x) / 45); 
    //return (Ruido*cos(0.039*x+Ruido) + 5*sin(0.05*x) + 0.5+Ruido*cos(0.01*x) + 10*sin(0.07*x) + 5*sin(0.1*x) + 5*sin(0.035*x))*10+1000;		
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

//Função que gera um ruido a cada x gerações
void ruido(){
    if (gen%(numger/20) == 0){   
        srand(static_cast<unsigned int>(time(0))); // Inicializa o gerador de números aleatórios
        float menor = -20.0f;
        float maior = 20.0f;
        Ruido = menor + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maior - menor)));
        printf("ruido %f\n", Ruido);
    }
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
        float y = fitnessFunction(ind[i].x);
        glVertex2f(ind[i].x, y);
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
void iniciapop(int tampop, individuo ind[]) {
    srand(time(NULL));  // Inicializa o gerador de números aleatórios com a semente baseada no tempo atual
    int minIntervalo = minx; 
    int maxIntervalo = maxx; 
    for (int i = 1; i <= tampop; i++) {
        ind[i].x = minIntervalo + rand() % (maxIntervalo - minIntervalo + 1);
    }
}


// Função para avaliar a aptidão da população
void avalia(int tampop) {
    printf("Geração %d\n", gen);
    ruido();
    for (int i = 1; i <= tampop; i++){
        ind[i].y = fitnessFunction(ind[i].x);
        printf("\tIndivíduo %d (x %f) = y %f\n", i, ind[i].x, ind[i].y);
    }
}

/*// Função de comparação para ordenar em ordem decrescente por y
bool comparaIndividuos(individuo a, individuo b) {
    return a.y > b.y;
}*/

void melhor_media(float maxfit, float media, int tampop){
    // Guarda o fitness do melhor indivíduo em vetmaxi
    vetmaxi[gen-1] = maxfit;

    // Calcula e guarda a média da população em vetmedia
    for (int i = 1; i <= tampop; i++) {
        media += ind[i].y;
    }
    media = media / tampop;
    vetmedia[gen-1] = media;
    //printf("A média da população é: %f\n", vetmedia[gen-1]);

}

// Função de elitismo para gerar nova geração
void elitismo(int tampop) {

    //std::sort(ind, ind + tampop + 1, comparaIndividuos);

    int maxi = 1;
    float media = 0.0f;
    float maxfit = ind[1].y;
    
    // Busca pelo melhor indivíduo
    for (int i = 2; i <= tampop; i++) {
        if (ind[i].y > maxfit) {
            maxfit = ind[i].y;
            maxi = i;
        }
    }

    //vetores para plotar grafico do melhor de todos e da media da população
    melhor_media(maxfit, media, tampop);

    for (int i = 1; i <= tampop; i++) {
        if (i == maxi) continue;  // Protege o melhor indivíduo

        // Crossover: o melhor indivíduo "transfere" parte de seus genes para os demais
        ind[i].x = (ind[i].x + ind[maxi].x) / 2;

        // Mutação: muda a posição do indivíduo de forma controlada dentro do domínio
        int mutacao = (int)(((rand() % maxx - (maxx / 2.0)) / 100.0) * TaxMut * maxx);
        ind[i].x = std::max(static_cast<float>(minx), std::min(static_cast<float>(maxx), static_cast<float>(ind[i].x + mutacao)));  // Garantir que permaneça no domínio
    }

}


/*
void torneio(int tampop){  // Torneio de 2

    std::sort(ind, ind + tampop + 1, comparaIndividuos);

    int a, b;
    float media = 0.0f;
    float maxfit = ind[1].y;
    
    for (int i=1 ; i<=tampop ; i++)
        indtemp[i] = ind[i].x;  // Backup dos individuos


    // Guarda o fitness do melhor indivíduo em vetmaxi
    vetmaxi[gen-1] = maxfit;

    // Calcula e guarda a média da população em vetmedia
    for (int i = 1; i <= tampop; i++) {
        media += ind[i].y;
    }
    media = media / tampop;
    vetmedia[gen-1] = media;
    printf("\tIndivíduo 1 (x %f) = y %f\n", ind[1].x, ind[1].y);
    
    // Torneio
    for (int i=2 ; i <= tampop ; i++){

        printf("\tIndivíduo %d (x %f) = y %f\n", i, ind[i].x, ind[i].y);

        // Sorteia dois individuos para 1ro torneio
        a = (rand() %tampop/2) + 1;
        b = (rand() %tampop/2) + 1;

        // Crossover
        ind[i].x = (indtemp[a] + indtemp[b])/ 2.0;  // Pegar os pais antigos, pois posso sortear os novos filhos que foram alterados

        // Mutacao                    | nr = 0-40 |    - 20    |  0,02  |  10%
        ind[i].x = ind[i].x + (float) (((rand() %maxx - (maxx/2.0))/100.0) * TaxMut);
    }
}*/


// Função para ajustar a taxa de mutação ao longo das gerações
void ajustaTaxaMutacao(int tampop) {

    //TaxMut = 5.0;

    ///*
    int cont = 0;
        
    for (int i = 0; i < tampop; i++) {
        for (int j = 0; j < tampop; j++) {
            // Arredondar para duas casas decimais
            float fit_i_rounded = std::round(ind[i].y * 100.0) / 100.0;
            float fit_j_rounded = std::round(ind[j].y * 100.0) / 100.0;

            if (fit_i_rounded == fit_j_rounded)
                cont++;
        }
    }

    if (gen%(numger/4) == 0){
        TaxMut = 20.0;
    }
    else{

    // Se 60% ou mais da população for igual, aumentar a taxa de mutação
    if (cont >= (tampop * (tampop - 1) / 2) * 0.6 && gen!=1){
        TaxMut = 5.0;

    }
        else{
            TaxMut = 5.0;
        }
    }
    //*/
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

    //torneio(TamPop);
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
