> colocar um time.Sleep(time.Millisecond * 100) em todas as soluções

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

#### Problemas
- pode ocorrer starvation em situações extremas