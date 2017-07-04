## Overview

This is an implementation of single consumer - multiple producers problem. One main process (consumer), creates N child processes (producers) using the function fork(). Shared memory is used to exchange data between producers and consumer, protected with semaphores to protect the access to common resource. Each producer reads a random line from a text file and send it to consumer. Consumer capitalize the string sent from consumer and return it back, ready for grad from any consumer process, to print it as a result.

![Structure](https://github.com/chanioxaris/Processes-SharedMemory-Semaphores/blob/master/img/figure.png)


### Shared Memory

Shared memory is a method of inter-process communication (IPC) between different processes. Moreover it's a memory that may be simultaneously accessed by multiple programs with an intent to provide communication among them or avoid redundant copies. More specific i am using a shared memory which contains two different arrays, named "in-ds" and "out-ds".

- in-ds: Producers write to array and consumer reads from it.
- out-ds: Consumer writes to array and producers read from it.

### Semaphores




## Compile

`./makefile`

## Usage

`./capitalize -N [number of producers] -K [number of iterations]`
