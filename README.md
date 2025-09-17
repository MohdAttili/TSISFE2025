# WRO-FUTURE-ENGINEERS-2025 - TEAM TUREAH
<p align="center">
<img src="t-photos/طورية.png" alt="Team Photo" style="width:300px; height:200px;">
</p>


---

## Vehicle Preview

<img src="v-photos/vehicle.png" alt="Vehicle Photo" width="500"/>

---
An autonomous vehicle designed for the Future Engineers category of the WRO 2025 that uses computer vision and IMU sensors to navigate complex environments and avoid obstacles intelligently.

## Content Structure

📁 **t-photos** → team photos  
📁 **v-photos** → vehicle photos  
📁 **video** → video.md with demonstration link  
📁 **schemes** → schematic diagrams  
📁 **src** → control software  
📁 **models** → 3D printing / laser cutting / CNC files  
📁 **other** → documentation and additional resources  
## Project Overview
This repository contains all the engineering materials, code, schematics, and models related to our self-driven vehicle, designed and built to participate in the WRO Future Engineers 2025 competition. The project aims to develop an autonomous vehicle capable of navigating a predefined course while demonstrating precision, stability, and effective control of electromechanical components.

Our vehicle is a compact, versatile model incorporating both mechanical and electronic systems. It is designed to efficiently demonstrate autonomous navigation using sensors, motor control, and a central microcontroller. The design and implementation of this project involved detailed planning, testing, and iterative improvements to meet the strict requirements of the competition.
## 🔍 Project Description
**🎯 Goals**  
1. Build a functional self-driven vehicle that can navigate a course autonomously.  
2. Integrate electronic and mechanical components seamlessly.  
3. Develop modular and maintainable code to control all vehicle components.  
4. Document all aspects of the vehicle's design, construction, and programming.  
5. Provide clear and detailed engineering materials to facilitate understanding and replication.  

## ⚙️ Physical Equipment 

<div style="display: flex; align-items: center; gap: 20px;">

  <div>
    
**1. 🧠 ESP32-WROOM-32 Overview**  
ESP32-WROOM-32 is a microcontroller by Espressif with Wi-Fi and Bluetooth, commonly used in smart devices and robotics.  
It features a dual-core Xtensa 32-bit LX6 processor up to 240 MHz, with 520 KB SRAM and 4 MB Flash.  
Offers around 34 GPIO pins, multiple interfaces, and supports ADC/DAC.  
Applications include robotics, IoT, and smart systems.  

  </div>

  <img src="t-photos/لوحة2.png" alt="لوحة" style="width:250px; height:160px; border-radius:10px;"/>

</div>

<!-- ## ⚙️ Physical Equipment <p align="right">
<img src="t-photos/لوحة2.png" alt="لوحة" style="width:300px; height:200px;">
</p>
**1. 🧠 ESP32-WROOM-32 Overview**  
1. ESP32-WROOM-32 is a microcontroller by Espressif with Wi-Fi and Bluetooth, commonly used in smart devices and robotics.
2. It features a dual-core Xtensa 32-bit LX6 processor up to 240 MHz, with 520 KB SRAM and 4 MB Flash.
3. Offers around 34 programmable GPIO pins and interfaces like SPI, I2C,  UART, PWM, and I2S, with ADC/DAC support.  
4. Supports dual connectivity, multiple protocols, and has a large  community with ready-to-use libraries.  
5. Common applications include robotics, smart home systems, remote   measurement/control devices, and IoT data collection.  
                    -->
**2. 🤖 L298N (Motor Driver Module) Overview**
1. The L298N is a dual H-bridge motor driver used to control the speed and direction of DC and stepper motors.
2. Supports motor voltages from 5V to 35V and currents up to 2A per channel.
3. Motor direction is controlled via IN1–IN4 pins and speed via PWM (ENA/ENB pins).
4. Features include independent dual motor control, built-in heat sink, and easy interfacing with Arduino, ESP32, and Raspberry Pi.
5. Commonly used in robotics, motorized DIY projects, and stepper motor control for small CNC machines or 3D printers.

**3. 📏 VL53LXX-V2 (Time-of-Flight Distance Sensor) Overview**     
1. The VL53L0X/VL53L1X is a Time-of-Flight (ToF) distance sensor that measures distance using laser light.
2. Measurement range: VL53L0X ~30 mm–2 m, VL53L1X ~30 mm–4 m with typical accuracy ±3%.
3. Uses I²C interface, supports multiple sensors on the same bus, with power supply 2.6–3.5V.
4. Features include fast and accurate distance measurement, non-contact sensing, low power consumption, and works in various lighting conditions.
5. Common applications: robotics obstacle detection, drones, gesture/proximity sensing, level measurement, and automated safety systems.
     
**4.🎥 Pixy 2.1 (CMUcam5) Overview**
1. The Pixy 2.1 is a smart vision sensor that detects and tracks objects by color or shape.
2. Resolution: 320×200 pixels, frame rate up to 60 fps, with interfaces SPI, I²C, UART, and USB.
3. Built-in processor allows real-time object recognition and can track multiple objects simultaneously.
4. Sends object coordinates directly to microcontrollers like Arduino or ESP32, reducing main board load.
5. Common applications include line-following robots, object detection/sorting, interactive projects, and educational AI experiments.