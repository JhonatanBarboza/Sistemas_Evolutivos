#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <time.h>

// Estrutura do indivíduo
typedef struct _individuo {

    float a;
    float b;
    float c;
    float d;
    float e;
    float f;
    float g;
    float h;
    float i;
    float j;
    float z;

} individuo;

//variaveis globais ajustaveis
#define TamPop 300                        // Tamanho da população
constexpr int numger = 500;               // Número de gerações
float Busca = 500;                        // Numero a ser buscado
float TaxRuido = 500.00;                   // Intervalo de ruido 

//Variaveis estaticas
float TaxMut;                             // Taxa de mutação
int gen = 1;                              // Geração atual
float Ruido;                              // ruido da função
int grafico;                              // nome do grafico
individuo ind[TamPop + 1];                // Indivíduos
float vetmaxi[numger], vetmedia[numger];  //vetores de apitidão 


//Funções 
float fitnessFunction(float x, float y, float w);
void ruido();
void iniciapop(int tampop, individuo ind[]);
void avalia(int tampop);
void elitismo(int tampop);
void ajustaTaxaMutacao(int tampop);
void initGraphWindow();
void displayGraphWindow();
void runAlgorithm();
void keyboard(unsigned char key, int x, int y);

//função principal
int main(int argc, char** argv) {
    iniciapop(TamPop, ind);

    // Inicializa o GLUT e o OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // Criação da janela do gráfico de vetmaxi e vetmedia
    glutInitWindowSize(1300, 600);
    glutInitWindowPosition(300, 50);
    grafico = glutCreateWindow("Grafico do Fitness Maximo e Media por Geracao");
    initGraphWindow();
    glutDisplayFunc(displayGraphWindow);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return 0;
}


//***************************************** sistema evolutivo *****************************************

// Função de aptidão com vários máximos e mínimos locais e um máximo global
float fitnessFunction(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j) {
    //return a + b + c + d + e - f - g - h - i - j + Ruido;  
    float resultado = 0.0;

    // Soma os primeiros cinco números e multiplica pelo valor absoluto do sexto
    resultado += ((a + b + c + d + e + Ruido) * fabs(f));

    // Subtrai o quadrado do sétimo e eleva o oitavo ao cubo
    resultado -= (pow(g, 2) + b * e) - Ruido / (sin (b * 67));
    resultado += (pow(h, 3) - i) + h * Ruido + sqrt (j + a)-sin (a);

    // Aplica seno ao nono e cosseno ao décimo e soma ambos
    resultado += (sin(i) + cos(j * a)) - Ruido + pow(e, 2)+ cos (b-i*j);

    // Verifica condições lógicas e faz um ajuste no resultado
    if (a > b && c < d)        resultado *= (2 + b) / 5 + Ruido;  // Se a > b e c < d, dobra o valor
    else if (e == f || g != h) resultado /= 3 + cos (d + sin (g * Ruido));  // Se e é igual a f ou g é diferente de h, divide por 3
    else if (b > a || c < i)   resultado += sin(c + Ruido) * f + cos(i * Ruido) * Ruido;

    // Função dos proj anteriores
    resultado += sin(c+Ruido)*f+cos(i*Ruido)*Ruido;

    // Realiza um ajuste de raiz quadrada baseado em alguns valores
    resultado += (sqrt(fabs(a * g - i * j) + Ruido))+5;

    return resultado;
}

//Função que gera um ruido a cada x gerações
void ruido(){
    if (gen%(numger/20) == 0){   
        // TaxRuido = rand() % 100;
        srand(static_cast<unsigned int>(time(0))); // Inicializa o gerador de números aleatórios
        float menor = -TaxRuido;
        float maior = TaxRuido;
        Ruido = menor + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maior - menor)));
        //printf("ruido %f | taxruido %f\n", Ruido, TaxRuido);
        printf("ruido %f\n", Ruido);
    }
}

// Função para iniciar a população
void iniciapop(int tampop, individuo ind[]) {
    srand(time(NULL));
    for (int i = 1; i <= tampop; i++) {
        ind[i].a = rand() % 200 ;
        ind[i].b = rand() % 200 ;
        ind[i].c = rand() % 200 ;
        ind[i].d = rand() % 200 ;
        ind[i].e = rand() % 200 ;
        ind[i].f = rand() % 200 ;
        ind[i].g = rand() % 200 ;
        ind[i].h = rand() % 200 ;
        ind[i].i = rand() % 200 ;
        ind[i].j = rand() % 200 ;
        ind[i].z = rand() % 200 ;
    }
}

// Função de avaliação
void avalia(int tampop) {
    //printf("Geração %d\n", gen);
    ruido();
    for (int i = 1; i <= tampop; i++) {
        ind[i].z = fitnessFunction(ind[i].a, ind[i].b, ind[i].c, ind[i].d, ind[i].e, ind[i].f, ind[i].g, ind[i].h, ind[i].i, ind[i].j); // Função de aptidão
    }
}

void melhor_media(float maxfit,int maxi){
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

    printf("Geração %d - Indivíduo %d - Melhor: %f, Média: %f\n", gen, maxi, vetmaxi[gen-1], vetmedia[gen-1]);
    //printf("\tIndivíduo %d | a %.2f | b %.2f | c %.2f | d %.2f | e %.2f | f %.2f | g %.2f | h %.2f | i %.2f | j %.2f\n", maxi, ind[maxi].a, ind[maxi].b, ind[maxi].c, ind[maxi].d, ind[maxi].e, ind[maxi].f, ind[maxi].g, ind[maxi].h, ind[maxi].i, ind[maxi].j);

}

// Elitismo e crossover com mutação
void elitismo(int tampop) {

    float maxfit = 0.0f;// Melhor aptidão
    int maxi;    // Índice do melhor indivíduo

    /*
    // Encontra o melhor indivíduo para o maior z 
    maxfit = ind[1].z;
    maxi = 1;
    for (int i = 2; i <= tampop; i++) {
        if (ind[i].z > maxfit) {
            maxfit = ind[i].z;
            maxi = i;
        }
    }*/

    // Encontra o melhor indivíduo com base na proximidade a n
    float Dif = fabs(ind[1].z - Busca);  // Usa a distância absoluta entre ind[1].z e n
    maxi = 1;
    for (int i = 2; i <= tampop; i++) {
        float diff = fabs(ind[i].z - Busca);  // Calcula a diferença entre ind[i].z e n
        if (diff < Dif) {
            Dif = diff;  // Atualiza a melhor proximidade
            maxi = i;       // Atualiza o índice do melhor indivíduo
        }
    }
    maxfit = ind[maxi].z;
    if (maxfit == Busca) 
        printf("\n\n\n a %.2f | b %.2f | c %.2f | d %.2f | e %.2f | f %.2f | g %.2f | h %.2f | i %.2f | j %.2f\n\n\n", ind[maxi].a, ind[maxi].b, ind[maxi].c, ind[maxi].d, ind[maxi].e, ind[maxi].f, ind[maxi].g, ind[maxi].h, ind[maxi].i, ind[maxi].j);


    //vetores para plotar grafico do melhor de todos e da media da população
    melhor_media(maxfit, maxi);


    // Crossover e mutação
    for (int i = 1; i <= tampop; i++) {
        if (i == maxi) continue; // Pula o melhor indivíduo (elitismo)

        // Crossover: combina o indivíduo com o melhor de todos
        ind[i].a = (ind[i].a + ind[maxi].a) / 2.0f;
        ind[i].b = (ind[i].b + ind[maxi].b) / 2.0f;
        ind[i].c = (ind[i].c + ind[maxi].c) / 2.0f;
        ind[i].d = (ind[i].d + ind[maxi].d) / 2.0f;
        ind[i].e = (ind[i].e + ind[maxi].e) / 2.0f;
        ind[i].f = (ind[i].f + ind[maxi].f) / 2.0f;
        ind[i].g = (ind[i].g + ind[maxi].g) / 2.0f;
        ind[i].h = (ind[i].h + ind[maxi].h) / 2.0f;
        ind[i].i = (ind[i].i + ind[maxi].i) / 2.0f;
        ind[i].j = (ind[i].j + ind[maxi].j) / 2.0f;

        // Mutação: muda a posição do indivíduo de forma controlada dentro do domínio
        if ((rand() / (float)RAND_MAX) < TaxMut) {
            // Aplica mutação ao eixo x e y com uma pequena variação aleatória
            ind[i].a += ((rand() / (float)RAND_MAX) - 0.5) * 10.0; 
            ind[i].b += ((rand() / (float)RAND_MAX) - 0.5) * 10.0;
            ind[i].c += ((rand() / (float)RAND_MAX) - 0.5) * 10.0; 
            ind[i].d += ((rand() / (float)RAND_MAX) - 0.5) * 10.0; 
            ind[i].e += ((rand() / (float)RAND_MAX) - 0.5) * 10.0;
            ind[i].f += ((rand() / (float)RAND_MAX) - 0.5) * 10.0; 
            ind[i].g += ((rand() / (float)RAND_MAX) - 0.5) * 10.0;
            ind[i].h += ((rand() / (float)RAND_MAX) - 0.5) * 10.0; 
            ind[i].i += ((rand() / (float)RAND_MAX) - 0.5) * 10.0; 
            ind[i].j += ((rand() / (float)RAND_MAX) - 0.5) * 10.0;
        }

        // Reavalia a aptidão após o crossover e mutação
        ind[i].z = fitnessFunction(ind[i].a, ind[i].b, ind[i].c, ind[i].d, ind[i].e, ind[i].f, ind[i].g, ind[i].h, ind[i].i, ind[i].j);
    }
}

// Função para ajustar a taxa de mutação ao longo das gerações
void ajustaTaxaMutacao(int tampop) {
    //TaxMut = 0.01;
    
    if (Ruido >= 1.0f) TaxMut = Ruido/10;
    else if (Ruido < -1.0f) TaxMut = (Ruido * -1)/10;
    //else TaxMut = 0.0001;
    else{
        int cont = 0;
            
        for (int i = 0; i < tampop; i++) {
            for (int j = 0; j < tampop; j++) {
                // Arredondar para duas casas decimais
                float fit_i_rounded = std::round(ind[i].z * 100.0) / 100.0;
                float fit_j_rounded = std::round(ind[j].z * 100.0) / 100.0;

                if (fit_i_rounded == fit_j_rounded)
                    cont++;
            }
        }

        // Se 60% ou mais da população for igual, aumentar a taxa de mutação
        if (cont >= (tampop * (tampop - 1) / 2) * 0.6 && gen!=1) TaxMut = 5.0;
        else TaxMut = 0.0001;
    }
}

//***************************************** openGL *****************************************


// Função para inicializar a janela do gráfico
void initGraphWindow() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, numger, -(Busca*4), Busca*5);
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

// Função para executar o algoritmo genético e atualizar o gráfico
void runAlgorithm() {
    ajustaTaxaMutacao(TamPop);
    avalia(TamPop);

    // Atualiza a janela do gráfico
    glutSetWindow(grafico);
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


