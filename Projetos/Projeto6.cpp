#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <time.h>

// Estrutura do indivíduo
typedef struct _individuo {
    float x, y, z;
} individuo;

#define TamPop 200    // Tamanho da população
#define TaxMut 0.00001  // Taxa de mutação
individuo ind[TamPop + 1]; // Indivíduos
int gen = 1;               // Geração atual
float maxfit;              // Melhor aptidão
int maxi;                  // Índice do melhor indivíduo
constexpr int numger = 20;           // Número de gerações
float c = 50.0f;
float vetmaxi[numger], vetmedia[numger];
int graphWindow, mainWindow;

// Função de aptidão com vários máximos e mínimos locais e um máximo global
float fitnessFunction(float x, float y) {
    return fmax(0, c - sqrt(x * x + y * y));  // Garante que o valor mínimo seja 0
}


// Função para iniciar a população
void iniciapop(int tampop, individuo ind[]) {
    srand(time(NULL));
    for (int i = 1; i <= tampop; i++) {
        ind[i].x = rand() % 100 - 50;
        ind[i].y = rand() % 100 - 50;
    }
}

// Função de avaliação
void avalia(int tampop) {
    printf("Geração %d\n", gen);
    for (int i = 1; i <= tampop; i++) {
        ind[i].z = fitnessFunction(ind[i].x, ind[i].y); // Função de aptidão
        if (ind[i].z < 0) ind[i].z = 0;
        //printf("\tIndivíduo %d - x %f - y %f - z %f\n", i, ind[i].x, ind[i].y, ind[i].z);
    }
}

void melhor_media(){
    float media = 0.0f;
    // Guarda o fitness do melhor indivíduo em vetmaxi
    vetmaxi[gen-1] = maxfit;

    // Calcula e guarda a média da população em vetmedia
    for (int i = 1; i <= TamPop; i++) {
        media += ind[i].z;
    }
    media = media / TamPop;
    vetmedia[gen-1] = media;
    //printf("A média da população é: %f\n", vetmedia[gen-1]);

    printf("Geração %d - Melhor: %f, Média: %f\n", gen, vetmaxi[gen-1], vetmedia[gen-1]);

}

// Elitismo e crossover com mutação
void elitismo(int tampop) {
    // Encontra o melhor indivíduo
    maxfit = ind[1].z;
    maxi = 1;
    for (int i = 2; i <= tampop; i++) {
        if (ind[i].z > maxfit) {
            maxfit = ind[i].z;
            maxi = i;
        }
    }

    //vetores para plotar grafico do melhor de todos e da media da população
    melhor_media();


    // Crossover e mutação
    for (int i = 1; i <= tampop; i++) {
        if (i == maxi) continue; // Pula o melhor indivíduo (elitismo)

        // Crossover: combina o indivíduo com o melhor de todos
        ind[i].x = (ind[i].x + ind[maxi].x) / 2.0f;
        ind[i].y = (ind[i].y + ind[maxi].y) / 2.0f;

        // Mutação: muda a posição do indivíduo de forma controlada dentro do domínio
        if ((rand() / (float)RAND_MAX) < TaxMut) {
            // Aplica mutação ao eixo x e y com uma pequena variação aleatória
            ind[i].x += ((rand() / (float)RAND_MAX) - 0.5) * 10.0; 
            ind[i].y += ((rand() / (float)RAND_MAX) - 0.5) * 10.0; 
        }

        // Reavalia a aptidão após o crossover e mutação
        ind[i].z = fitnessFunction(ind[i].x, ind[i].y);
        if (ind[i].z < 0) ind[i].z = 0;
    }
    
    
}


//***************************************************************************************************//

// Função para desenhar uma grade sobre a superfície do cone
void drawGridOnCone() {
    float step = 5.0f; // Define o espaçamento da grade
    glLineWidth(3.0f); // Aumenta a espessura das linhas
    glColor3f(0.5f, 0.5f, 0.7f); // Cor da grade (cinza)

    // Linhas radiais
    glBegin(GL_LINES);
    for (float theta = 0.0f; theta <= 2 * M_PI; theta += M_PI / 12) { // Passos de 15 graus
        for (float r = 0.0f; r <= 50.0f; r += step) {
            float x1 = r * cos(theta);
            float y1 = r * sin(theta);
            float z1 = fitnessFunction(x1, y1);
            if (z1 < 0) z1 = 0;

            float x2 = (r + step) * cos(theta);
            float y2 = (r + step) * sin(theta);
            float z2 = c - sqrt(x2 * x2 + y2 * y2);
            if (z2 < 0) z2 = 0;

            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y2, z2);
        }
    }
    glEnd();

    // Linhas circulares
    glBegin(GL_LINES);
    for (float r = 0.0f; r <= 50.0f; r += step) {
        for (float theta = 0.0f; theta <= 2 * M_PI; theta += M_PI / 36) { // Passos de 5 graus
            float x1 = r * cos(theta);
            float y1 = r * sin(theta);
            float z1 = fitnessFunction(x1, y1);
            if (z1 < 0) z1 = 0;

            float x2 = r * cos(theta + M_PI / 36);
            float y2 = r * sin(theta + M_PI / 36);
            float z2 = c - sqrt(x2 * x2 + y2 * y2);
            if (z2 < 0) z2 = 0;

            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y2, z2);
        }
    }
    glEnd();
}

// Função para desenhar os indivíduos
void drawIndividuals(int tampop) {
    glColor3f(0.0f, 0.0f, 0.0f); // Cor vermelha para os indivíduos
    glPointSize(7.0f);
    glBegin(GL_POINTS);
    for (int i = 1; i <= tampop; i++) {
        glVertex3f(ind[i].x, ind[i].y, ind[i].z);
    }
    glEnd();
}

// Função para desenhar a superfície do cone
void drawConeSurface() {
    float step = 0.5;
    glEnable(GL_BLEND); // Ativa o blending para a transparência
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Define a função de mistura

    glBegin(GL_QUADS);
    for (float x = -50.0; x <= 50.0; x += step) {
        for (float y = -50.0; y <= 50.0; y += step) {

            // Calcula z para os pontos
            float z = fitnessFunction(x, y);
            if (z < 0) z = 0;
            float z_nextX = c - sqrt((x + step) * (x + step) + y * y);
            if (z_nextX < 0) z_nextX = 0;
            float z_nextY = c - sqrt(x * x + (y + step) * (y + step));
            if (z_nextY < 0) z_nextY = 0;
            float z_nextXY = c - sqrt((x + step) * (x + step) + (y + step) * (y + step));
            if (z_nextXY < 0) z_nextXY = 0;

            // Define a cor baseada no valor de z (normalizando o valor de z)
            float colorFactor = z / c; // Normaliza z com relação ao valor máximo (c)
            glColor4f(0.0f, colorFactor, 1.0f - colorFactor, 0.5f); // gradiente azul para verde

            // Desenha o quadrado com gradiente
            glVertex3f(x, y, z);
            glColor4f(0.0f, (z_nextX / c), 1.0f - (z_nextX / c), 0.5f);
            glVertex3f(x + step, y, z_nextX);
            glColor4f(0.0f, (z_nextXY / c), 1.0f - (z_nextXY / c), 0.5f);
            glVertex3f(x + step, y + step, z_nextXY);
            glColor4f(0.0f, (z_nextY / c), 1.0f - (z_nextY / c), 0.5f);
            glVertex3f(x, y + step, z_nextY);
        }
    }
    glEnd();

    glDisable(GL_BLEND); // Desativa o blending após desenhar o cone
}

// Função de exibição
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(50.0, 100.0, 150.0,  // Posição da câmera
              0.0, 0.0, 0.0,        // Ponto para onde a câmera está olhando
              0.0, 0.0, 1.0);       // Vetor "para cima"

    //drawAxes();         // Desenha os eixos
    //drawGrid();         // Desenha a grade no plano XY
    drawConeSurface();  // Desenha a superfície do cone com transparência
    drawGridOnCone();   // Desenha a grade sobre o cone
    drawIndividuals(TamPop); // Desenha os indivíduos

    glutSwapBuffers();
}

// Função de inicialização
void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

// Função para inicializar a janela do gráfico
void initGraphWindow() {

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, numger, 0, 60);
}

void displayGraphWindow() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Desenha o gráfico do fitness máximo (vetmaxi)
    glColor3f(1.0, 0.0, 0.0); // Cor vermelha
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < gen - 1; i++) {  // Garante que não ultrapassa o limite
        glVertex2f(i, vetmaxi[i]);
        
    }
    glEnd();

    // Desenha o gráfico da média da população (vetmedia)
    glColor3f(0.0, 0.0, 1.0); // Cor azul
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < gen - 1 ; i++) {  // Garante que não ultrapassa o limite
        glVertex2f(i, vetmedia[i]);
    }
    glEnd();

    glutSwapBuffers();  // Se estiver usando double buffering
}

// Função para atualização
void update(int value) {
    avalia(TamPop);

    // Atualiza a janela principal
    glutSetWindow(mainWindow);
    glutPostRedisplay();

    // Atualiza a janela do gráfico
    glutSetWindow(graphWindow);
    glutPostRedisplay();

    if (gen <= numger) {
            glutTimerFunc(2000, update, 0); // Atualiza a cada segundo
        } else {
            printf("Número máximo de gerações atingido.\n");
            exit(-1);
        }
    

    elitismo(TamPop);
    gen++;// Incrementa a geração após cada ciclo
}

// Função principal
int main(int argc, char** argv) {
    iniciapop(TamPop, ind);


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(1000, 800);
    mainWindow = glutCreateWindow("Sistema Evolutivo sobre Cone");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(1000, update, 0);

    // Criação da janela do gráfico de vetmaxi e vetmedia
    glutInitWindowSize(800, 300);
    glutInitWindowPosition(1200, 600);
    graphWindow = glutCreateWindow("Grafico do Fitness Maximo e Media por Geracao");
    initGraphWindow();
    glutDisplayFunc(displayGraphWindow);

    glutMainLoop();
    return 0;
}
