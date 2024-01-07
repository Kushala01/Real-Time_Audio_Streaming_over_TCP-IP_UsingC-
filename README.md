# Real-Time Audio Streaming over TCP/IP using C++

Welcome to the Real-Time Audio Streaming project implemented in C++. This project enables you to stream audio data from a client to a server over a TCP/IP connection, providing a seamless real-time audio communication experience. Below, you'll find information on setting up and running the project.

## Project Overview

The Real-Time Audio Streaming project consists of two main components: the client and the server. The client captures audio data using the microphone, converts it to a suitable format, and sends it to the server. The server, in turn, receives the audio data and plays it back in real-time. This project uses Winsock for network communication and the Windows Multimedia API for audio capture and playback.

## Prerequisites

Before running the project, ensure that you have the following prerequisites installed:

- **Windows Operating System:** This project is designed to run on the Windows platform.
- **C++ Compiler:** You need a C++ compiler that supports the Winsock library and the Windows Multimedia API.

## Setting Up the Project

1. **Clone the Repository:**
   Clone this repository to your local machine using the following command:
   ```bash
   git clone https://github.com/your-username/audio-streaming-cpp.git
   ```

2. **Compile the Code:**
   Use your preferred C++ compiler to build both the client and server executables.

## Running the Project

1. **Start the Server:**
   - Run the compiled server executable on the machine where you want to receive the audio stream.
   - The server will wait for incoming connections and display the number of connected clients.

2. **Start the Client:**
   - Run the compiled client executable on the machine with the microphone.
   - The client will attempt to connect to the server. Adjust the `SERVER_IP` constant in the client code to match the server's IP address.

3. **Real-Time Audio Streaming:**
   - Once the client connects to the server, it will start capturing audio from the microphone and streaming it to the server.
   - The server will play back the received audio in real-time.

4. **Stop the Application:**
   - Press `Ctrl + C` in the terminal or console to stop both the client and server applications.

## Configuration Options

- **Server Configuration:**
  - Adjust the `PORT` constant in both client and server codes to set the desired communication port.
  - The `CLIENTS` constant in the server code defines the maximum number of allowed clients.

- **Audio Configuration:**
  - Modify the `SAMPLE_RATE` and `BUFFER_SIZE` constants to match your audio settings. These values impact the quality and latency of the audio stream.

## Troubleshooting

- If you encounter issues with Winsock initialization or audio playback, ensure that the necessary libraries are available on your system.

## Acknowledgments

This project is inspired by the need for real-time audio communication and is built upon Winsock and the Windows Multimedia API. Feel free to contribute, report issues, or enhance the project further.

Enjoy real-time audio streaming with this C++ project!
