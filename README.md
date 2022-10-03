# in-switch-PFC

## Introduction

This is the simuation of a scheme which is an extension of **PFC** (Priority-based flow control) that aims to improve the performence of packets control system. We optimize PFC by adding **VOQ** (virtual output queue) / **VIQ** (virtual input queue) in switch and a scheme similar to PFC in the in-switch-transmission.  
This repository does not cotain the part of PFC but only contians the model of the switch with VOQ/VIQ and in-switch-transmission.  
To sumulate this scheme, we use **`NS3`** and **`C++`**.

### PFC

### VOQ/VIQ

## Enviroment

- Ubuntu 16
- NS3-3.26
- gcc5 & g++5

## Installation

***NOTE:*** To use any files in this repository, you need to put the relavant `.cc`, `.h` and `wscript` files into the coresponding path in ns3.

You should install the enviroment above first.  

- NS-3.26 simulator installation guide can be found in [here](https://www.nsnam.org/wiki/Installation)  

As provided in this repository, there is folder called `scratch` and a folder called `src`.

### scratch

This folder contains a `TOPO.cc` and many `traffic_[bandwidth].txt` files as topoloy and traffic files for testing respectively.  
If you use the the `TOPO.cc` as your topology, you must include a `traffic.txt` file in the same folder. For the `traffic.txt`, you can:

1. choose one of the `traffic_[bandwidth].txt` file in the `scratch` folder and change its name to `traffic.txt`;
2. generate a traffic file by your self:
   - **TO BE ADDED**

### src

This folder contians an `alpplication` folder and a `traffic-control` folder.

#### application

In this folder, we provide a model that can add `tags` to every packets when they are sent out. The values of the tags are the the input port the packet used to enter the switch and the output port the packet used to leave the swtich.

#### traffic-control

In this folder, we provide a `Queue` model and a `QueueDisc` model that can be used as virtual queues and impletes **RoundRobin** in **in-switch-transmission**.  
To use these models, you should change values of  
`DEFAULT_PAIR`, `DEFAULT_FIFO_N_SIZE`, `VOQ_ON`, `VOQ_OFF`, `VIQ_ON`, `VIQ_OFF` in `QueueCreae.h` and  
`DEFAULT_PAIR`, `SPEEDUP_FACTOR` in `VirtualQueue.h` as needed.

## Topology

- **TO BE ADDED**

&nbsp;

**NOTE:** This repository is still in building ... 
