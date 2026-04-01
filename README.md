# Autonomous Rover for Warehouse Rack Inventory Management System

An intelligent autonomous rover designed for **real-time warehouse inventory auditing and management**, developed for **Inter IIT Tech Meet 14.0**.

The system performs **autonomous navigation, vertical rack scanning, QR-code detection, and real-time inventory reporting**, reducing manual audit cycles from **1–2 weeks to just 1 day** with **<1% discrepancy**.

---

## Project Overview

Warehouse inventory auditing is traditionally slow, labor-intensive, and error-prone.

This project introduces an **end-to-end autonomous scanning rover** capable of:

- Autonomous warehouse navigation
- Rack-level vertical scanning
- High-accuracy QR/barcode detection
- Real-time inventory status updates
- Low discrepancy stock verification

---

## Key Performance Targets

| Metric | Target |
|---------|--------|
| Horizontal Position Accuracy | ±10 cm |
| Vertical Position Accuracy | ±2 cm |
| Image Resolution | 1080p |
| QR Detection Reliability | >95% |
| Audit Cycle Time | 1 Day |
| Inventory Discrepancy | <1% |

---

## System Architecture

The rover consists of the following major subsystems:

- **Chassis System**
- **Differential Drive Mechanism**
- **Vertical Scissor Lift**
- **Power Management System**
- **Control System**
- **Path Planning Module**
- **QR Code Scanning Module**
- **Human Machine Interface (HMI)**

---

## Hardware Specifications

### Rover Specifications

- **Dimensions:** 600 × 355 mm
- **Weight:** ~20 kg
- **Rack Coverage:** 5 shelf levels
- **Vision Height Range:** 0.14 m – 1.75 m
- **Drive:** 2-wheel differential drive
- **Support:** Front and rear casters
- **Lift Mechanism:** 4-stage scissor lift
- **Battery:** 18V rechargeable battery
- **Backup Capacity:** 22000 mAh
- **Operational Time:** Up to 4 hours

---

## Sensors & Computing

- **Intel RealSense D455** for depth sensing and navigation
- **1080p Camera** for QR-code scanning
- **Jetson AGX Xavier** for high-level control and processing
- **ESP32** for real-time low-level motor control

---

## Mechanical Design

### Chassis

- Modular T-slot architecture
- High structural rigidity
- Controlled flexibility for uneven floors
- Optimized load distribution

### Drive System

- Differential drive configuration
- Zero-radius turning
- High traction wheel placement
- Robust caster-based stability

### Vertical System

A **4-stage pantograph scissor lift** is used for vertical scanning.

### Why Scissor Lift?

- Fully collapsible
- Compact design
- High vertical precision
- Safe for narrow aisle movement
- Easy field maintenance

---

## Control Architecture

The control system follows a **two-tier hierarchical structure**:

### High-Level Control
- Jetson AGX Xavier
- ROS2 (Cyclone DDS)
- Path planning
- Object detection
- QR scanning
- System supervision

### Low-Level Control
- ESP32
- PID-based motor control
- Encoder feedback processing
- Real-time closed-loop control

---

## Software Stack

- **ROS2**
- **Python**
- **C++**
- **OpenCV**
- **ESP32 Firmware**
- **MATLAB PID tuning**
- **Jetson SDK**

---

## Features

- Autonomous navigation
- Real-time mapping
- QR code scanning
- Inventory mismatch detection
- Rack height scanning
- Live bot telemetry
- Battery diagnostics
- Wi-Fi quality monitoring
- Remote start / pause / restart controls

---

## Human Machine Interface (HMI)

The HMI dashboard provides:

- Bot live status
- Battery health
- Wi-Fi status
- Live camera feed
- Sensor telemetry
- Start / stop scan controls
- Inventory alerts
- Success / caution notifications

---

## Future Improvements

### 1. Multi-Robot Swarm Coordination
Enable multiple autonomous rovers with centralized planning and distributed execution.

### 2. Enhanced Sensor Suite
Integrate:
- LiDAR
- Ultrasonic sensors
- Radar
- Thermal imaging

### 3. Robotic Arm Integration
Add manipulation capabilities for:
- pick and place
- object repositioning
- multi-angle scanning

---

## Applications

- Smart warehouses
- Inventory automation
- Industrial logistics
- E-commerce fulfillment centers
- Manufacturing storage systems

---

## Team

**Inter IIT Tech Meet 14.0**

Project: *Autonomous Rover for Warehouse Rack Inventory Management System*

---

## Acknowledgments

Special thanks to the Inter IIT Tech Meet organizers, mentors, and team members for their support in the development of this project.
