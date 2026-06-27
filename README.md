# 🌊 Energy-Aware Underwater Acoustic Communication Simulator

An adaptive underwater acoustic communication simulator developed in **C**, implementing intelligent transmission strategies based on available battery energy and underwater channel conditions.

The simulator integrates a simplified **BELLHOP-inspired underwater acoustic channel model** to estimate transmission loss, Signal-to-Noise Ratio (SNR), and packet loss probability for realistic communication behavior.

---

## 📌 Features

- Adaptive transmission strategy selection
- Energy-aware communication
- Three transmission modes
  - 📡 Periodic Transmission
  - ⚡ Event-Driven Transmission
  - 🚀 Burst Transmission
- Battery consumption simulation
- BELLHOP-inspired channel modeling
- SNR calculation
- Packet loss simulation
- Retransmission support
- Modular C implementation

---

## 🧠 Project Overview

Underwater communication systems operate under severe energy constraints.

This simulator dynamically selects the most suitable communication mode according to:

- Remaining battery energy
- Event occurrence
- Required transmission power
- Underwater acoustic channel conditions

The objective is to maximize communication reliability while minimizing battery consumption.

---

## ⚙️ Transmission Modes

### 📡 Periodic Mode

- Low transmission power (10 W)
- Sends data in chunks
- Reliable due to retransmissions
- Suitable for low-energy conditions

---

### ⚡ Event Mode

- Medium transmission power (25 W)
- Triggered only when an event occurs
- Retransmissions are skipped to conserve energy

---

### 🚀 Burst Mode

- High transmission power (50 W)
- Sends the complete message in one transmission
- Fastest communication mode
- Used when sufficient energy is available

---

## 🌊 Underwater Channel Model

The simulator uses a simplified BELLHOP-based acoustic model.

The following parameters are considered:

- Transmission Loss (TL)
- Ambient Noise
- Source Level (SL)
- Signal-to-Noise Ratio (SNR)
- Packet Loss Probability

The receiver determines whether packets are successfully received according to the computed SNR.

---

## 🔋 Adaptive Decision Logic

<img width="1433" height="697" alt="image" src="https://github.com/user-attachments/assets/d6be9f21-9821-426f-ae26-38561bcea1a8" />

---

## 📊 Energy Model

Energy consumed is calculated as

```
Energy = (Message Length × Transmission Power) / Bitrate
```

Battery energy decreases after every transmission.

---

## 📂 Project Structure

```
energy-aware-underwater-acoustic-communication/

│
├── src/
│     └── main.c
│
├── docs/
│     └── Project_Report.pdf
│
├── screenshots/
│
├── output/
│     └── sample_output.txt
│
├── README.md
├── LICENSE
└── .gitignore
```

---

## 🖥️ Compilation

### GCC

```bash
gcc main.c -o simulator -lm
```

### Run

```bash
./simulator
```

For Windows (MinGW)

```bash
gcc main.c -o simulator.exe -lm
simulator.exe
```

---

## ▶️ Sample Execution

```
Enter initial available energy:
55

Enter event:
1

Selected Mode:
EVENT

Transmission Loss : 12.86 dB
SNR : 121.12 dB

Packet Successfully Received

Remaining Battery:
51.75 J
```

---

## 📷 Sample Ouputs
### Without BELLHOP Channel Transmission
#### Event Transmission

<img width="479" height="558" alt="image" src="https://github.com/user-attachments/assets/f84f67d6-f347-46b7-ae64-4d76b748b676" />

---

#### Burst Transmission

<img width="479" height="282" alt="image" src="https://github.com/user-attachments/assets/27e0d22d-7432-4e9c-a75d-d73cb4a14ac5" />
<img width="479" height="322" alt="image" src="https://github.com/user-attachments/assets/03d5145c-2c58-4e95-adea-55428b599e02" />


---

#### Periodic Transmission

<img width="447" height="663" alt="image" src="https://github.com/user-attachments/assets/f775f51c-654d-453e-973d-36f1491395d3" />


---


## 🚀 Future Improvements

- Dynamic transmission distance
- Variable underwater noise
- Multi-node communication
- Underwater sensor network simulation
- Real BELLHOP output integration
- GUI visualization
- Energy harvesting model
- Routing protocols

---

## 👨‍💻 Technologies Used

- C Programming
- Embedded C Concepts
- BELLHOP Acoustic Channel Modeling
- Energy-Aware Communication
- Windows API (Sleep)
- GCC

---

## 📚 Applications

- Underwater Sensor Networks
- Marine Environmental Monitoring
- Autonomous Underwater Vehicles (AUVs)
- Offshore Exploration
- Oceanographic Research
- Defense Communication Systems

---

## 👩‍💻 Authors

Harshini S

VIT Chennai

---
## Contributors

Subiksha

Akshaya H



