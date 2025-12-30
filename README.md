# Low-Power Multi-Node IoT Sensor Network

## Overview
This project implements a low-power multi-node IoT sensor network using Arduino Nano sensor nodes and an ESP8266 gateway. The system collects environmental data and uploads it to the ThingSpeak cloud platform for real-time visualization.

## System Architecture
- Node 1: Temperature & Humidity (DHT22)
- Node 2: Light Intensity (LDR)
- Gateway: ESP8266 + nRF24L01
- Cloud Platform: ThingSpeak

## Key Features
- Battery-powered sensor nodes
- Duty-cycled low-power operation
- Wireless communication using nRF24L01
- Cloud integration and visualization
- Experimental energy consumption analysis

## Hardware Components
- Arduino Nano (ATmega328P)
- ESP8266
- nRF24L01 RF modules
- DHT22 sensor
- LDR sensor
- 3.7 V Li-ion batteries (series configuration)

## Energy Analysis
Energy consumption was measured experimentally using a digital multimeter. Duty cycling increased battery lifetime by up to 4× compared to always-active operation.

## Repository Structure
