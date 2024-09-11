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
Pedidos de garfos podem ser pendentes se o garfo ainda não estiver disponível (?)

### Condicional Troca de Garfos
Um filósofo só envia um garfo ao vizinho se for requisitado e se não estiver comendo ou com fome
Se o filósofo está com fome e tem os dois garfos, ele não cede até que tenha comido


### Mecanismos:
Canais e mensagens
Estados para os filósofos (thinking, hungry, eating) e garfos (sujo, limpo)

### Problemas:
- overhead de mensagens
- solução de implementação complexa (coordenação assíncrona e manipulação de mensagens)