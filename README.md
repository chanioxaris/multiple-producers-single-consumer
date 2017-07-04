## Overview

This is an implementation of single consumer - multiple producers problem. One main process (consumer), creates N child processes (producers) using the function fork(). Shared memory is used to exchange data between producers and consumer, protected with semaphores to protect the access to common resource. Each producer reads a random line from a text file and send it to consumer. Consumer capitalize the string sent from consumer and return it back, ready for grad from any consumer process, to print it as a result.

![Structure](https://github.com/chanioxaris/Processes-SharedMemory-Semaphores/blob/master/img/figure.png)


### Shared Memory


### Semaphores




## Compile

`./makefile`

## Usage

`./capitalize -N [number of producers] -K [number of iterations]`
