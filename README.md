# Sistemas Evolutivos

Bem-vindo ao meu projeto, onde a ciência dos algoritmos evolutivos se encontra com a arte da visualização gráfica em OpenGL. Aqui, você poderá acompanhar como populações de indivíduos evoluem em busca da maior pontuação de aptidão no sistema.

### Confira uma demonstração do projeto mais recente em ação!

![Screencast from 2024-10-12 12-57-54](https://github.com/user-attachments/assets/9c4286df-8ca9-4cdd-830a-1ecc71d81ffe)


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

![proj2](https://github.com/user-attachments/assets/010fcdbd-0f68-4455-92cb-8a301e3381fb)

## PROJ3

DESAFIO: Navegar por um terreno complexo de múltiplos picos e vales.

O Projeto 3 utiliza a função sin(x) * (x^2 / 45) no intervalo de -51 a 48. Esta função apresenta vários máximos e mínimos locais, com um máximo global que é o objetivo da população. O desafio é maior, pois o máximo global e o segundo maior máximo estão em lados opostos do gráfico. Além disso, foi implementada uma taxa de mutação variável. A população é gerada aleatoriamente e, ao longo das gerações, a taxa de mutação é mantida em 0,1 até que 60% da população esteja idêntica até a segunda casa decimal, o que indica que a população encontrou um máximo. Quando isso ocorre, a taxa de mutação aumenta para 10, espalhando novamente a população em busca de um máximo maior.

![proj3](https://github.com/user-attachments/assets/ab1743af-0ae9-4fab-91b7-68615dc4c7a0)

## PROJ4

DESAFIO: Expandir o horizonte e analisar a evolução com mais profundidade.

O Projeto 4 é uma melhoria do Projeto 3, utilizando a mesma função de avaliação sin(x) * (x^2 / 45), mas agora no intervalo de -400 a 400. A principal mudança está na adição de dois gráficos: um em vermelho que mostra a aptidão do melhor de todos, e outro em azul que exibe a média da população. Esses gráficos permitem análises mais detalhadas para aprimorar a estratégia evolutiva.

Outra novidade é a possibilidade de restringir o intervalo de nascimento da população, tornando o problema mais desafiador e testando a capacidade do sistema evolutivo de encontrar a solução mesmo sob essas condições. Além disso, a taxa de mutação foi ajustada: em intervalos maiores, foi necessário aumentar ainda mais a dispersão da população. Assim, a cada cinco vezes que a mutação é aumentada como no Projeto 3, é aplicada uma taxa de mutação ainda maior.

![proj4](https://github.com/user-attachments/assets/c2c2353d-562d-46a9-96c2-2fcca1290161)


## PROJ5

DESAFIO: Adaptar-se a um ambiente dinâmico e imprevisível com funções complexas e ruído em evolução.

O Projeto 5 eleva a dificuldade ao introduzir um ruído dinâmico que modifica a função de avaliação ao longo do tempo, simulando a natureza real, onde as condições ambientais mudam constantemente e impactam os indivíduos. A nova função de avaliação sin(x + Ruido) * x + cos(x * 9.3) * 9.3 é significativamente mais complexa que as anteriores, com o ruído gerando pequenas flutuações laterais na função, alterando os máximos e desafiando o sistema a encontrar o ponto ideal em meio a essa variabilidade.

![proj5 1](https://github.com/user-attachments/assets/1273f4e4-f15d-4ff0-9381-a711057155ed)

Além disso, uma versão ainda mais caótica foi introduzida: sin(x + Ruido) * x + cos(x * Ruido) * Ruido, simulando um ambiente onde as variáveis externas influenciam de forma intensa e contínua. Para lidar com essa complexidade, ajustes foram feitos na taxa de mutação. Ela permanece fixa em 5%, mas quatro vezes durante a evolução da população, a taxa aumenta temporariamente para 20%, promovendo maior diversidade e evitando que o sistema se estabilize em soluções subótimas. O número de indivíduos agora se adapta automaticamente ao tamanho do gráfico, ajustando a pressão evolutiva conforme o cenário se modifica.

![proj5 2](https://github.com/user-attachments/assets/c2bef6ac-3609-45a0-bb77-22ee09ee5e95)


## PROJ6

DESAFIO: Dominar a adaptação em um ambiente tridimensional

O Projeto 6 eleva o desafio para o espaço tridimensional, onde o objetivo é encontrar o máximo global a partir da interação entre dois parâmetros (x e y) que influenciam um terceiro (z). A função de avaliação adotada é `z = 50 - sqrt(x^2 + y^2)`, onde o valor de z é maximizado quando x e y se aproximam de zero. Isso cria um ambiente em formato de cone, onde o pico (máximo global) está no centro.

A principal dificuldade reside na adaptação do sistema evolutivo para lidar com a complexidade tridimensional, onde múltiplas variáveis interagem de maneira mais dinâmica e as estratégias anteriores precisam ser ajustadas para encontrar a solução ótima. O algoritmo genético agora deve operar simultaneamente em três eixos, maximizando o valor de z a partir da combinação ideal de x e y.

Assim como nos projetos anteriores, a taxa de mutação é ajustada ao longo das gerações, começando em 5% e aumentando temporariamente em momentos críticos para garantir maior diversidade e evitar que a população converja prematuramente em soluções subótimas. A pressão evolutiva é cuidadosamente controlada para explorar o espaço tridimensional de maneira eficaz, permitindo que o sistema evolutivo encontre o máximo global no ambiente mais complexo até agora.

![proj6](https://github.com/user-attachments/assets/8f1a8731-e2dc-461a-a584-d89de57abb08)

