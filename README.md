# 🎮 TTK4155 - "Bygger'n" - Pong Game Project

Welcome to our Pong Game project! This repository contains the code and documentation for a distributed, microcontroller-based ping pong game developed as part of the TTK4155 course at NTNU.

## 🔗 GitHub Repository

You can find the project on GitHub here: [GitHub Repository](https://github.com/Adriaeik/TTK4155Atmel)

## 📁 Repository Structure

- **ATmega162/**: Contains code and configurations for Node 1.
- **RosaElefant/**: Contains code and configurations for Node 2 (ATSAM3X8E).

## 🛠️ Project Overview

The project involves two main nodes:

1. **Node 1 (ATmega162)**: Manages user inputs, display interfaces, and initial game logic.
2. **Node 2 (ATSAM3X8E - "RosaElefant")**: Handles motor control, sensors, and real-time game dynamics.

These nodes communicate via CAN-bus to ensure synchronized operations.

## 🎯 Key Features

- **User Interface**: OLED display for menus, game status, and scores.
- **Control Mechanisms**: Joystick and button inputs to control game elements.
- **Real-Time Control**: Motor and servo management for game element movement.
- **Sensor Integration**: Detects motor position and speed to adjust game dynamics.

## 🧠 Game Logic

The game logic includes:

- **Initialization**: Sets up game parameters and prepares hardware components.
- **Input Handling**: Reads user inputs to control game elements.
- **State Management**: Maintains the current state of the game and transitions between states based on events.
- **Scoring System**: Updates and displays scores based on game events, storing the top 19 highest scores.
- **Feedback Mechanisms**: Provides visual feedback to enhance user experience, such as displaying images of Pikachu and Ash laughing when the player loses. 

<p align="center">
  <img src="images/pikachu_ler.png" alt="Pikachu ler" width="200" />
  <img src="images/ashOgpikachu_ler.png" alt="Ash og Pikachu ler" width="200" />
</p>


## 🚀 Getting Started

To get started with the project:

1. Clone the repository.
2. Navigate to the respective directories for Node 1 and Node 2.
3. Follow the setup instructions provided in each directory's README file.

**Note**: This project was developed using Atmel Studio on Windows.

## 🤝 Contributors

- **Adrian Eikeland**: [GitHub Profile](https://github.com/Adriaeik)
- **Ravn Erik Budde**: [GitHub Profile](https://github.com/ravnbudde)
- **Pontus Leon Ekman Vangen**: [GitHub Profile](https://github.com/poelekva)




 
