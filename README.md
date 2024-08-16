# Algoritmo Genético com Visualização em OpenGL

Este projeto demonstra a aplicação de algoritmos genéticos para otimização, com visualização gráfica utilizando OpenGL. O objetivo principal é evoluir uma população de indivíduos para encontrar o máximo global de uma função de aptidão com múltiplos máximos e mínimos locais.

## Descrição do Projeto

Neste projeto, você encontrará exemplos de algoritmo genético implementado em C++ e uma interface gráfica para visualizar a evolução da população ao longo das gerações. Com diferentes funçôes de aptidão, desafiando o algoritmo a encontrar o ponto ótimo global.

### Recursos

- **Visualização Gráfica:** Utiliza OpenGL para desenhar a função de aptidão e os pontos da população. O fundo do gráfico é branco, enquanto a função e os pontos são desenhados em preto e azul, respectivamente.
- **Algoritmo Genético:** Implementa um algoritmo de elitismo com mutação para evoluir a população ao longo das gerações.
- **Interatividade:** Permite a atualização da população e o gráfico com a tecla Enter.

### Compilação e Execução

1. **Compilação:**
   - Utilize o `Makefile` incluído para compilar o código.
   - Comando: `make`

2. **Execução:**
   - Execute o programa compilado.
   - Comando: `./genetico_openGL`

3. **Limpeza:**
   - Para remover os arquivos gerados durante a compilação.
   - Comando: `make clean`

### Requisitos

- **GLUT (OpenGL Utility Toolkit):** Necessário para a criação da interface gráfica.
- **Compilador C++:** Necessário para compilar o código fonte.

## Como Contribuir

Sinta-se à vontade para contribuir com melhorias, correções ou sugestões. Para contribuir, por favor, abra uma issue ou envie um pull request com suas alterações.

