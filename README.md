# `philosophers`

![42 school project](https://img.shields.io/badge/42-Project-4285F4?style=for-the-badge&logo=42)

---

## Introduction

This project is a key part of the **42 school curriculum**. The goal of `philosophers` is to solve the classic "Dining Philosophers" problem. The project focuses on learning about concurrency, threads, and synchronization to avoid common issues like deadlocks and race conditions.

The program should simulate a number of philosophers sitting at a round table, each with a fork to their left and right. The philosophers must eat, sleep, and think, but can only eat when they hold both their left and right forks.

---

## How It Works

The simulation is built using a multi-threaded approach, where each philosopher is represented by a thread. To solve the problem of multiple philosophers trying to grab the same fork at the same time, the program uses synchronization primitives like **mutexes**.

The core logic for each philosopher thread follows these steps in a loop:

1.  **Think**: The philosopher is thinking and does not require any resources.
2.  **Take Forks**: The philosopher tries to pick up both their left and right forks. This is done with mutexes to ensure only one philosopher can pick up a specific fork at a time. The order in which forks are picked up is crucial to prevent deadlocks.
3.  **Eat**: Once both forks are acquired, the philosopher starts eating for a specific duration.
4.  **Put Forks Down**: After eating, the philosopher puts both forks back on the table, releasing the mutexes.
5.  **Sleep**: The philosopher then goes to sleep for a specified duration before repeating the cycle.

The program also includes a monitoring thread that checks if any philosopher has starved (i.e., has not eaten within a specified time), which would indicate a problem with the synchronization.

---

## Installation and Usage

To compile and run `philosophers`, you will need a `Makefile`.

### Compilation

You can compile your program using the provided `Makefile`.

```bash
make
```

### Running the Program

The program takes four arguments:
-   `number_of_philosophers`
-   `time_to_die`
-   `time_to_eat`
-   `time_to_sleep`

```bash
./philo 5 800 200 200
```
This example simulates 5 philosophers, with a time to die of 800ms, time to eat of 200ms, and time to sleep of 200ms.

---

## Project Contributor

-   **Ruben Finnerud** - [rubenfin](https://github.com/rubenfin)

---
