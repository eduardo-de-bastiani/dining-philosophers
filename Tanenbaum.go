package main

import (
	"fmt"
	"sync"
	"time"
)

const (
	numPhilosophers = 5
	hungryCycles    = 10		// cada filosofo realiza 3 ciclos
)

type PhilosopherState int

const (
	Thinking PhilosopherState = iota
	Hungry
	Eating
)

var (
	mutex        sync.Mutex
	state        = make([]PhilosopherState, numPhilosophers)
	condition    = make([]*sync.Cond, numPhilosophers) // One condition variable per philosopher
	wg           sync.WaitGroup
)

type Philosopher struct {
	id int
}

func (p Philosopher) think() {
	fmt.Printf("Philosopher %d is thinking\n", p.id+1)
	time.Sleep(time.Millisecond * 100)
}

func (p Philosopher) eat() {
	fmt.Printf("Philosopher %d is eating\n", p.id+1)
	time.Sleep(time.Millisecond * 100)
}

func (p Philosopher) dine() {
	defer wg.Done()

	for i := 0; i < hungryCycles; i++ {
		p.think()

		// Philosopher is hungry
		p.pickUpForks()

		// Philosopher eats
		p.eat()

		// Philosopher puts down forks after eating
		p.putDownForks()
	}

	fmt.Printf("Philosopher %d is done dining\n", p.id+1)
}

func (p Philosopher) left() int {
	return (p.id + numPhilosophers - 1) % numPhilosophers
}

func (p Philosopher) right() int {
	return (p.id + 1) % numPhilosophers
}

func (p Philosopher) pickUpForks() {
	mutex.Lock()
	state[p.id] = Hungry 
	test(p.id)
	// Wait if the philosopher can't eat
	for state[p.id] != Eating {
		condition[p.id].Wait()
	}
	mutex.Unlock()
}

func (p Philosopher) putDownForks() {
	mutex.Lock()
	state[p.id] = Thinking
	// Test if the left and right neighbors can eat
	test(p.left())
	test(p.right())
	mutex.Unlock()
}

func test(id int) {
	// A philosopher can eat if they are hungry and both neighbors are not eating
	if state[id] == Hungry && state[left(id)] != Eating && state[right(id)] != Eating {
		state[id] = Eating
		// Signal philosopher to eat
		condition[id].Signal()
	}
}

func left(id int) int {
	return (id + numPhilosophers - 1) % numPhilosophers
}

func right(id int) int {
	return (id + 1) % numPhilosophers
}

func main() {

	startTime := time.Now()

	// Initialize condition variables for each philosopher
	for i := 0; i < numPhilosophers; i++ {
		condition[i] = sync.NewCond(&mutex)
	}

	// Create and launch philosophers
	for i := 0; i < numPhilosophers; i++{
		wg.Add(1)
		philosopher := Philosopher{id: i}
		go philosopher.dine()
	}

	
	// Wait for all philosophers to finish dining
	wg.Wait()


	endTime := time.Now()

	elapsedTime := endTime.Sub(startTime)
	fmt.Println("All philosophers are done dining.")
	fmt.Println("Concurrent Time: ", elapsedTime)
}