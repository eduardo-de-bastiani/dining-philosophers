> colocar time.Sleep(time.Millisecond * 100) para pensar
> colocar time.Sleep(time.Millisecond * 100) para comer

## Tanenbaum Otimizado:
- impelmentação por estados: "thinking", "hungry", "eating"
- usa semáforos binários (5)
- usa Sinais (Wait Groups)

A cada ciclo, os filósofos realizam os processos de pensar, pegar os garfos, comer e soltar os garfos.
### Think
O processo de pensar é concorrente mas não gera data race por ser independente.
### PickUpForks
Após pensarem, os filósofos tentam pegar os garfos (pickUpForks), entrando em uma seção crítica, onde o acesso aos garfos é exclusivo.
O filósofo que tenta pegar os garfos tem seu estado alterado para "hungry".
Após isso, verificamos se ambos adjascentes não estão comendo ("eating"). Se for verdade, o filósofo muda seu estado para "eating", sai da seção crítica e sinaliza com Singal() para que os outros filósofos possam tentar comer.
Se a verificação acima falhar (pelo menos um dos adjascentes estiver comendo), o filósofo é bloqueado e fica em uma espera passiva Wait(), "saindo de cima" dos garfos (Mutex.unlock()) para que outro filósofo possa tentar acessá-los. Ele só é desbloqueado assim que outro filósofo sinalizar (Signal()) que já comeu.
### PutDonwForks
O filósofo que estava comendo entra em uma seção crítica para evitar condição de corrida e somente ele alterar o estado dos garfos. Ele muda seu estado para thinking, soltando os garfos. Após isso, verifica os estados dos filósofos adjascentes (se estiverem "hungry" e seus adjascentes não estiverem comendo), saindo da seção crítica. Se os vizinhos puderem comer, seus estados são alterados para "eating" e sinalizam que terminaram de comer (Signal()) 
___

## Chandy/Misra

### Estado dos Garfos
Garfos com estados limpo ou sujo. O garfo começa no estado sujo, e o filósofo só pode comer quando estiver no estado limpo. 
Depois de comer, o filósofo suja o garfo.

### Toca de Mensagens
Um filósofo pede um garfo ao vizinho se o garfo do vizinho estiver sujo.
Se o vizinho tiver um garfo limpo, ele o envia de volta.
Se tiver um garfo sujo, mantém o garfo até que alguém o peça

### Protocolo de Comunicação
São usadas mensagens assíncronas
Um filósofo envia uma mensagem pedindo um garfo, o vizinho responde com o garfo quando puder, e ele é limpo automaticamente
Pedidos de garfos podem ser pendentes se o garfo ainda não estiver disponível

### Condicional Troca de Garfos
Um filósofo só envia um garfo ao vizinho se for requisitado e se não estiver comendo ou com fome
Se o filósofo está com fome e tem os dois garfos, ele não cede até que tenha comido


### Mecanismos:
Canais e mensagens
Estados para os filósofos (thinking, hungry, eating) e garfos (sujo, limpo)

### Problemas:
- overhead de mensagens
- solução de implementação complexa (coordenação assíncrona e manipulação de mensagens)


Para compilar:
> g++ -std=c++11 -o main main.cpp

Para rodar:
> ./main


___

# Questões

## Descrição Funcionamento Tanenbaum Otimizado

A Solução Otimizada de Tanenbaum utiliza estados (Thinking, Hungry e Eating), semáforos Binários, um para cada processo (filósofo), e Sinais (Wait Groups). A cada ciclo, os filósofos alteram entre os estados Thinking, Hungry e Eating, no qual Thinking é independente e sua concorrência não gera nenhum tipo de sincronismo de competição ou cooperação. Os métodos pickUpForks e putDownForks são responsáveis por gerenciar a concorrência no programa

### pickUpForks

Quando um processo (filósofo) chama o método pickUpForks, ele adentra em uma seção crítica revestida por um semáforo binário (mutex) para que somente ele modifique seu estado. Ele imediatamente altera seu estado para "Hungry" e chama a função test. A função test verifica se o estado do filósofo é "Hungry" e os estados dos filósofos à esquerda e à direita são diferentes de "Eating", podendo ser "Thinking" ou "Hungry". Se a condição falhar (o filósofo não alterar seu estado para Eating), o processo terá de esperar (Wait) que outro filósofo dispare um sinal para este tentar acessar os garfos novamente.
Se a condição tiver êxito, o processo que chamou a função muda seu estado para "Eating" e dispara um sinal (Signal) para que os demais filósofos tentem acessar os garfos. Após isso, a função eat é chamada para que o processo (filósofo) acesse os recursos (se alimente) e o método putDownForks é acionado.

### putDownForks

Novamente, o processo adentra em uma seção crítica em que somente ele pode alterar seu estado. Ele imediatamente altera seu estado para Thiking e chama o método test para seus vizinhos da esqueda e direita. Ou seja, a função putDownForks possibilita que os vizinhos do processo (filósofo) que já foi processado (já comeu) tentem acessar os recursos (comer) também.

Dessa forma, a Solução Otimizada de Tanenbaum promove um ciclo eficiente, em que todos os processos têm oportunidades iguais de acessar os recursos, sem que nenhum filósofo retenha os garfos indefinidamente. Com isso, tanto Deadlock quanto Starvation são evitados, assegurando o equilíbrio e a continuidade do sistema.


## Descrição Funcionamento Chandy/Misra

A solução de Chandy/Misra utiliza estados para os filósofos, assim como na Solução Otimizada de Tanenbaum. Além disso, os garfos também possuem os estados "limpo" e "sujo". Também há outros mecanismos para o gerenciamento das concorrências, como os Canais e as mensagens.

Nesta solução, todos os processos executam os "HungryCycles" onde os filósofos devem pensar e comer. Novamente, o estado thinking também é independente dos demais.

Nesta solução, cada filósofo é "proprietário" de um garfo (geralmente o da direita) e a alternância de acesso aos recursos (alimentação) é realiza por pedidos de empréstimo de um garfo ao vizinho. 

Além disso, todos os garfos iniciam no estado "sujo", e um filósofo só pode comer se o garfo estiver no estado "limpo". A única maneira de um garfo ser limpo é emprestando-o a outro filósofo. Dessa maneira, a Solução de Chandy/Misra assegura que quando um filósofo requisita um garfo sujo de outro filósofo, o garfo é entregue (se o dono não estiver com fome ou comendo), mas o filósofo que o cede limpa o garfo antes de entregá-lo ao vizinho. Esse ciclo de limpeza e empréstimo contribui diretamente para a prevenção de deadlock e starvation.

### Protocolo de Comunicação

Para que os pedidos de garfo ocorram, é necessário que cada garfo seja um canal entre os filósofos vizinhos. Assim, as mensagens são enviadas através dele. A natureza assíncrona das mensagens colabora para a resolução de deadlock, onde não há necessidade de esperas ativas. Assim, os processos podem continuar suas atividades sem que gerem uma espera circular.


DEVEMOS FAZER OS PONTOS 1 A 4 SEPARADOS???