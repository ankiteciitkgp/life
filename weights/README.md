The repository contains resources of smart switch module "life".


The module has three components embedded hardware, server and android application.

This repository contains complete codes and hardware schematics for hardware and server.

The information about directories is as follows:

schematics: 
	The schematic and layouts were designed in Proteus.
	There are two versions of hardware module: 
	Version 1: It consits of 4 switched and 2 fan
	Version 2: It consists of 2 switches and one fan with display of fan level

hardware_V1: 
	Contains code for ESP, main Arduino and Regulator's Arduino
	In this version ESP is acting just as a Wifi Module which writes message to Rx of Arduino

hardware_V2:
	In this version the most of the functionality of the main Arduino is shifted regulator due to security reasons.

server:
	Contains server script of Websocket Server using Autobahn, an open source implementation of Websockets

server_Ssl:
	Contains server script supporting TLS encryption. The TLS encryption is not very stable and takes time to establish connection due to low computation power of ESP
	
