# zsharpminor's SmartBell, v1!

Hi, thanks for reviewing my project! ;w; This is an ESP32-powered smart doorbell with smart AI-powered detection, that automatically captures a picture that is sent to a Discord webhook via WiFi when the doorbell is rung, which triggers an integration that can set off a custom ringtone on a mobile app. Features near-waterproof PETG/TPU case and a temperature sensor that automatically reports Temperature and Humidity to said webhook as well. Additionally, an ultrasonic sensor detects movement and wakes up the camera to take a picture and identify it through a simple ML algorithm. NOTE: This is a software-heavy project, I am aiming for this to be a Tier 4 project, and Tanook agreed that it qualifies as Tier 4 (under the condition that a CAD case exists, which it does!).

BOM CSV FILE: https://docs.google.com/spreadsheets/d/1LvQgwua6umSP1ob7BG9KYe2bF_PC_YEmnamfS4FcfZg/

(BOM and placement CSV files also placed inside the PCB folder for easy access).

# BOM:

**GRANT REQUIRED: $30.00**

Screenshots of Carts:

1x SEEED Studios

<img width="506" height="548" alt="Screenshot 2025-12-02 at 7 53 21 PM" src="https://github.com/user-attachments/assets/2110a6c3-8f84-4505-950c-da713fca6667" />

1x JLCPCB

<img width="460" height="450" alt="Screenshot 2025-12-02 at 7 56 57 PM" src="https://github.com/user-attachments/assets/69d9a35a-fe44-49d9-a4a3-68a581809c71" />

Grant:

* 1x SEEED Studios XIAO ESP32-S3 SENSE (**requires Grant**).
* 1x 2-Layer JLCPCB (**requires Grant**).

Self-Provided:

* 1x Standard Active Buzzer (Self-Provided)
* 1x 5mm Diameter Green Standard LED (Self-Provided)
* 1x 330 Ohm Resistor for LED (Self-Provided)
* 1x 12mm Tactswitch and Cover (Self-Provided)
* 4x Female Header, Right Angle (Self-Provided)
* 1x Adafruit STEMMA QT to 4-pin Male Cable (Self-Provided)
* 1x Adafruit AHT20 STEMMA I2C Temp/Humi Sensor (Self-Provided)
* 1x HC-SR04 45 x 70 mm Ultrasonic Sensor (Self-Provided)
* 1x 1500mah JST LiPo Battery (Self-Provided)

* 1x Case to be entirely 3d-printed in near-waterproof PETG with TPU seals (self-provided), iterations may occur.

(NOTE: the case intentionally has an empty space to the right; this is for the AHT20 temp/humi sensor as well as a LiPo battery (I'm experimenting with a few battery sizes!)

PCB will be glued to the back of the case, and the top will be water sealed with a TPU gasket and water-resistant sealant.

# Empty CAD case:

<img width="558" height="375" alt="Screenshot 2025-12-02 at 7 40 02 PM" src="https://github.com/user-attachments/assets/c2aaa5fb-d5cb-4636-a425-9a6b0ca29d3d" />

<img width="597" height="407" alt="Screenshot 2025-12-02 at 7 40 08 PM" src="https://github.com/user-attachments/assets/a6bcb435-6d00-4bd7-9123-00d6f8085e32" />

# Final CAD model with case, showcasing how the components fit together:

## No. 1: PCB in case without top:

<img width="699" height="348" alt="Screenshot 2025-12-02 at 7 38 25 PM" src="https://github.com/user-attachments/assets/7eb5bd8c-b27c-4bc8-89f8-96bb2c36928b" />

## No. 2: PCB in case with top:

<img width="229" height="298" alt="Screenshot 2025-12-02 at 7 38 45 PM" src="https://github.com/user-attachments/assets/2bdebb1b-7211-4e2b-9a95-04b1835edf8c" />

## No. 3: Side-View of USB-C port and tapered edges

<img width="321" height="203" alt="Screenshot 2025-12-02 at 7 39 04 PM" src="https://github.com/user-attachments/assets/9d24c411-d411-47fe-a97b-12dfed64050d" />

# Schematic:

<img width="985" height="535" alt="Screenshot 2025-12-02 at 7 36 44 PM" src="https://github.com/user-attachments/assets/25daa660-13da-4df7-b908-458eb8e5b1c6" />

# PCB:

<img width="441" height="613" alt="Screenshot 2025-12-02 at 7 36 56 PM" src="https://github.com/user-attachments/assets/ff4676c1-27a3-4913-b6b2-8f56732c9ab2" />

# PCB 3d Render:

<img width="379" height="464" alt="Screenshot 2025-12-02 at 7 37 14 PM" src="https://github.com/user-attachments/assets/c2cd57ea-d3ff-4879-b7af-5446f1ef39c5" />
