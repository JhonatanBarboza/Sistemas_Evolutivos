# Sistemas Evolutivos com Visualização em OpenGL

Bem-vindo ao meu projeto, onde a ciência dos algoritmos evolutivos se encontra com a arte da visualização gráfica em OpenGL. Aqui, você poderá acompanhar como populações de indivíduos evoluem em busca da maior pontuação de aptidão no sistema.

## Descrição do Projeto

Este projeto apresenta a implementação de algoritmos genéticos em C++ e uma interface gráfica desenvolvida em OpenGL para visualizar a evolução da população ao longo das gerações. Diferentes funções de aptidão são utilizadas para desafiar o algoritmo a encontrar as melhores soluções possíveis.

### Recursos

- **Visualização Gráfica:** Utiliza OpenGL para desenhar a função de aptidão e os pontos da população.
- **Algoritmo Genético:** Implementa um algoritmo com mutação para evoluir a população ao longo das gerações.
- **Interatividade:** Permite a atualização da população e o gráfico com a tecla Enter.

### PROJ1

Este projeto serve como a base para um sistema evolutivo. A população inicial é gerada, e a aptidão dos indivíduos é avaliada pela função `-0.2 * (x - 10) * (x - 10) + 10`. Novas gerações são criadas através de um processo de elitismo, onde o melhor indivíduo transmite seus genes para toda a população, com uma taxa de mutação aplicada. O objetivo é encontrar o máximo global da função.

### PROJ2

O Projeto 2 é uma evolução do Projeto 1, com a adição de um gráfico da função utilizando OpenGL para visualização. 

Aqui, é possível ver os indivíduos sendo gerados aleatoriamente e, ao longo das gerações, convergindo para o máximo da função.

https://github.com/user-attachments/assets/9def0049-d5f3-4396-8da4-c6b660548612

### PROJ3

O Projeto 3 utiliza a função `sin(x) * (x^2 / 45)` no intervalo de -51 a 48. Esta função apresenta vários máximos e mínimos locais, com um máximo global que é o objetivo da população. O desafio é maior, pois o máximo global e o segundo maior máximo estão em lados opostos do gráfico.

Além disso, foi implementada uma taxa de mutação variável. A população é gerada aleatoriamente e, ao longo das gerações, a taxa de mutação é mantida em 0,1 até que 60% da população esteja idêntica até a segunda casa decimal, o que indica que a população encontrou um máximo. Quando isso ocorre, a taxa de mutação aumenta para 10, espalhando novamente a população em busca de um máximo maior.

https://github.com/user-attachments/assets/730aa40c-a7eb-4384-aedc-666e92e6a901

