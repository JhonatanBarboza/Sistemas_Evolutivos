# Nome do executável
TARGET = Ecologico

# Compilador e flags
CC = g++
CFLAGS = -Wall -g

# Bibliotecas necessárias
LIBS = -lGL -lGLU -lglut

# Arquivos fonte
SRCS = Ecologico.cpp

# Arquivos objeto
OBJS = $(SRCS:.cpp=.o)

# Regra padrão para compilar tudo
all: $(TARGET)

# Regra para compilar o programa
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Regra para compilar os arquivos objeto
%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

# Regra para executar o programa
run: $(TARGET)
	./$(TARGET)

# Regra para depurar usando gdb
debug: $(TARGET)
	gdb ./$(TARGET)

# Regra para executar o programa com Valgrind
valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

# Regra para limpar os arquivos objeto e o executável
clean:
	rm -f $(OBJS) $(TARGET)

# Evita que make interprete "clean", "run", "debug", ou "valgrind" como nomes de arquivos
.PHONY: clean run debug valgrind all











// Desenho na Tela
void exibir() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenhar as presas
    glColor3f(1.0f, 1.0f, 1.0f);  
    for (int i = 0; i < cont_presas+1; i++) {
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
    for (int i = 0; i < cont_predadores+1; i++) {
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
    glutTimerFunc(1000 / 60, timer, 0);  // 60 FPS
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