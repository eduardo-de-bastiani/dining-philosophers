package main

import (
	"fmt"
	"time"
)

// Número de filósofos
const(
	numFilosofos = 5
	hungryCycles = 10
) 

// Estados dos garfos
var garfos = make([]bool, numFilosofos)

// Simular o jantar dos filósofos
func jantarDosFilosofos() {

	for j := 0; j < hungryCycles; j++{
		// Ciclo para cada filósofo pegar e soltar os garfos
		for i := 0; i < numFilosofos; i++ {
			fmt.Printf("Filósofo %d está pensando.\n", i+1)
			pegarGarfos(i)
			time.Sleep(time.Millisecond * 100)
			fmt.Printf("Filósofo %d está comendo.\n", i+1)
			soltarGarfos(i)
			fmt.Printf("Filósofo %d terminou de comer e soltou os garfos.\n", i+1)
			time.Sleep(time.Millisecond * 100)
		}
	}
}

// Função para pegar os garfos (sequencialmente, sem concorrência)
func pegarGarfos(i int) {
	esquerda := i
	direita := (i + 1) % numFilosofos

	garfos[esquerda] = true  // Pega o garfo esquerdo
	garfos[direita] = true   // Pega o garfo direito
}

// Função para soltar os garfos
func soltarGarfos(i int) {
	esquerda := i
	direita := (i + 1) % numFilosofos

	// Solta os garfos
	garfos[esquerda] = false
	garfos[direita] = false
}

func main() {

	startTime := time.Now()

	fmt.Println("Iniciando o jantar dos filósofos...")
	jantarDosFilosofos()

	endTime := time.Now()

	elapsedTime := endTime.Sub(startTime)

	fmt.Println("Jantar encerrado.")
	fmt.Println("Concurrent Time: ", elapsedTime)
}
