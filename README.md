# Air Traffic Control System

This repository contains an implementation of an Air Traffic Control System designed using operating systems. The system simulates processes such as planes, airports, an air traffic controller (ATC), passengers, and cleanup operations using POSIX-compliant C programs.

## Problem Statement

The assignment involves developing four POSIX-compliant C programs:

### plane.c

- Creates a plane process representing a new plane instance.
- Collects user inputs for:
  - Unique plane ID
  - Type of plane (passenger or cargo)
  - Relevant details:
    - For passenger planes: number of passengers, luggage weight, and body weight
    - For cargo planes: number of cargo items and average weight
- Sends departure information to the air traffic controller upon readiness.

### ATC.c

- Manages plane arrivals, departures, and communications with airports.
- Receives messages from planes and instructs airports accordingly via a single message queue.
- Coordinates departure and landing processes.
- Logs plane journeys in `AirTrafficController.txt`.

### cleanup.c

- Facilitates orderly termination of the entire system.
- Upon user input (Y or N), requests termination from the air traffic controller via the message queue.

### airport.c

- Creates an airport process to handle plane arrivals and departures.
- Sets up runways with load capacities and manages plane arrivals/departures.
- Uses threading to handle simultaneous plane operations.
- Interacts with the air traffic controller through the message queue.

## Requirements

To run this project, ensure you have the following:

- Ubuntu 22.04 installed on your system.
- GCC compiler for compiling C programs.
- Terminal to execute commands.

## Conclusion
This repository provides a comprehensive implementation of an Air Traffic Control System. Follow the outlined steps to successfully compile and execute the program. Good luck!
