## Overview

This is an implementation of single consumer - multiple producers problem. One main process (consumer), creates N child processes (producers) using the function fork(). Shared memory is used to exchange data between producers and consumer, protected with semaphores to protect the access to common resource. Each producer reads a random line from a text file and send it to consumer. Consumer capitalize the string sent from consumer and return it back, ready for grad from any consumer process, to print it as a result.

![Structure](https://github.com/chanioxaris/multiple-producers-single-consumer/blob/master/img/figure.png)


### Shared Memory

Shared memory is a method of inter-process communication (IPC) between different processes. Moreover it's a memory that may be simultaneously accessed by multiple programs with an intent to provide communication among them or avoid redundant copies. More specific i used a shared memory which contains two different arrays, named "in-ds" and "out-ds".

- in-ds: Producers write to array and consumer reads from it.
- out-ds: Consumer writes to array and producers read from it.

### Semaphores

Α semaphore is a variable or abstract data type used to control access to a common resource by multiple processes in a concurrent system such as a multiprogramming operating system. More specific i used four binaries semaphores, two for each array in shared memory, to control and synchronize the access in it. Producers and consumer operate (up and down) on both semaphores of each array, in crossed model, so we can ensure that no process is gonna write on a full array or read from an empty array.


## Compile

`./makefile`

## Usage

`./capitalize -N [number of producers] -K [number of iterations]`
