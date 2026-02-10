# 💻 Turing machine 💽
A turing machine emulator in C. Built as a project to better learn C. 🧠

## Usage
- Write a .tring file with instructions and input memory.
- Or describe a problem and feed in the skills.md file to an AI model

## Features
* Turing complete
* Turing instruction language
* Visualize turing machine in terminal

## How it works
1. Read .tring file
2. Parse each line and extract instructions, memory, state
3. Store (state, value) as key in hashmap to value (write, direction, new_state)
4. Start at inital state and at cell 0
5. Get instruction corrosponding to current state and current cell value
6. Execute instruction and update cell based on direction
7. Repeat until no instruction found or goes in halt state
8. Turing Complete!!

## Peformance
* 

