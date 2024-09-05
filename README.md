# Sistemas Evolutivos com Visualização em OpenGL

Bem-vindo ao meu projeto, onde a ciência dos algoritmos evolutivos se encontra com a arte da visualização gráfica em OpenGL. Aqui, você poderá acompanhar como populações de indivíduos evoluem em busca da maior pontuação de aptidão no sistema.

### Confira uma demonstração do projeto mais recente em ação!

https://github.com/user-attachments/assets/c9fd53fd-8444-4ffe-9404-11faf0cd83fe

## Descrição do Projeto

Este projeto apresenta a implementação de algoritmos genéticos em C++ e uma interface gráfica desenvolvida em OpenGL para visualizar a evolução da população ao longo das gerações. Diferentes funções de aptidão são utilizadas para desafiar o algoritmo a encontrar as melhores soluções possíveis.

### Visão Geral do Desafio 

Progredir na complexidade e aprimorar continuamente o sistema até que ele seja capaz de resolver problemas reais.

O objetivo destes projetos é, de forma incremental, aumentar a dificuldade das condições enfrentadas pelo sistema evolutivo. Cada etapa representa um desafio maior, onde novos obstáculos são introduzidos para testar e fortalecer a capacidade do sistema. A meta final é desenvolver um sistema robusto e eficiente, capaz de resolver problemas complexos e aplicáveis em cenários reais.

### Recursos

- **Visualização Gráfica:** Utiliza OpenGL para desenhar a função de aptidão e os pontos da população.
- **Algoritmo Genético:** Implementa um algoritmo com mutação para evoluir a população ao longo das gerações.
- **Interatividade:** Permite a atualização da população e o gráfico com a tecla Enter.

## PROJ1

DESAFIO: Construir a base sólida para sistemas evolutivos.

Este projeto serve como a base para um sistema evolutivo. A população inicial é gerada, e a aptidão dos indivíduos é avaliada pela função -0.2 * (x - 10) * (x - 10) + 10. Novas gerações são criadas através de um processo de elitismo, onde o melhor indivíduo transmite seus genes para toda a população, com uma taxa de mutação aplicada. O objetivo é encontrar o máximo global da função e estabelecer uma base sólida para os próximos projetos.

## PROJ2
DESAFIO: Visualizar a evolução em tempo real.

O Projeto 2 é uma evolução do Projeto 1, com a adição de um gráfico da função utilizando OpenGL para visualização. O desafio aqui é transformar a evolução da população em uma representação visual clara e dinâmica. Agora, é possível ver os indivíduos sendo gerados aleatoriamente e, ao longo das gerações, convergindo para o máximo da função.

https://github.com/user-attachments/assets/9def0049-d5f3-4396-8da4-c6b660548612

## PROJ3

DESAFIO: Navegar por um terreno complexo de múltiplos picos e vales.

O Projeto 3 utiliza a função sin(x) * (x^2 / 45) no intervalo de -51 a 48. Esta função apresenta vários máximos e mínimos locais, com um máximo global que é o objetivo da população. O desafio é maior, pois o máximo global e o segundo maior máximo estão em lados opostos do gráfico. Além disso, foi implementada uma taxa de mutação variável. A população é gerada aleatoriamente e, ao longo das gerações, a taxa de mutação é mantida em 0,1 até que 60% da população esteja idêntica até a segunda casa decimal, o que indica que a população encontrou um máximo. Quando isso ocorre, a taxa de mutação aumenta para 10, espalhando novamente a população em busca de um máximo maior.

https://github.com/user-attachments/assets/730aa40c-a7eb-4384-aedc-666e92e6a901

## PROJ4

DESAFIO: Expandir o horizonte e analisar a evolução com mais profundidade.

O Projeto 4 é uma melhoria do Projeto 3, utilizando a mesma função de avaliação sin(x) * (x^2 / 45), mas agora no intervalo de -400 a 400. A principal mudança está na adição de dois gráficos: um em vermelho que mostra a aptidão do melhor de todos, e outro em azul que exibe a média da população. Esses gráficos permitem análises mais detalhadas para aprimorar a estratégia evolutiva.

Outra novidade é a possibilidade de restringir o intervalo de nascimento da população, tornando o problema mais desafiador e testando a capacidade do sistema evolutivo de encontrar a solução mesmo sob essas condições. Além disso, a taxa de mutação foi ajustada: em intervalos maiores, foi necessário aumentar ainda mais a dispersão da população. Assim, a cada cinco vezes que a mutação é aumentada como no Projeto 3, é aplicada uma taxa de mutação ainda maior.

https://github.com/user-attachments/assets/0fa2609f-4327-4407-b9fb-f0e80919ee09


## PROJ5

DESAFIO: Adaptar-se a um ambiente dinâmico e imprevisível com funções complexas e ruído em evolução.

O Projeto 5 eleva a dificuldade ao introduzir um ruído dinâmico que modifica a função de avaliação ao longo do tempo, simulando a natureza real, onde as condições ambientais mudam constantemente e impactam os indivíduos. A nova função de avaliação sin(x + Ruido) * x + cos(x * 9.3) * 9.3 é significativamente mais complexa que as anteriores, com o ruído gerando pequenas flutuações laterais na função, alterando os máximos e desafiando o sistema a encontrar o ponto ideal em meio a essa variabilidade.

https://github.com/user-attachments/assets/ed065a28-3580-4c18-87e4-92b1c2dfe827

Além disso, uma versão ainda mais caótica foi introduzida: sin(x + Ruido) * x + cos(x * Ruido) * Ruido, simulando um ambiente onde as variáveis externas influenciam de forma intensa e contínua. Para lidar com essa complexidade, ajustes foram feitos na taxa de mutação. Ela permanece fixa em 5%, mas quatro vezes durante a evolução da população, a taxa aumenta temporariamente para 20%, promovendo maior diversidade e evitando que o sistema se estabilize em soluções subótimas. O número de indivíduos agora se adapta automaticamente ao tamanho do gráfico, ajustando a pressão evolutiva conforme o cenário se modifica.

https://github.com/user-attachments/assets/c9fd53fd-8444-4ffe-9404-11faf0cd83fe

