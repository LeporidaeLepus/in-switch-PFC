# in-switch-PFC

## Introduction

This is simuation of a scheme which is an extension of **PFC** (Priority-based flow control) that aims to improve the performence of packets control system. We optimize PFC by adding **VOQ** (virtual output queue) / **VIQ** (virtual input queue) in switch and a scheme similar to PFC in the in-switch-transmission.  
This repository do not cotain the part of PFC but only contian the model of the switch with VOQ/VIQ and in-switch-transmission.  
To sumulate this scheme, we use **`NS3`** and **`C++`**.

### PFC

### VOQ/VIQ

## Enviroment

- Ubuntu 16
- NS3-3.26
- gcc5 & g++5

## Installation

You should install the enviroment above first. 

As provided in this repository, there is folder called `scratch` and a folder called `src`. If you want use the files in them, you must move them to the folder with the same name in your NS3.

### scratch

This folder contains a `TOPO.cc` and many `traffic_[bandwidth].txt` files as topoloy and traffic files for testing respectively.  
If you use the the `TOPO.cc` as your topology, you must include a `traffic.txt` file in the same folder. For the `traffic.txt`, you can:

1. choose one of the `traffic_[bandwidth].txt` file in the `scratch` folder and change its name to `traffic.txt`;
2. generate a traffic file by your self:
   - **TO BE ADDING**

### src

This folder contians an `alpplication` folder and a `traffic-control` folder.

#### application
