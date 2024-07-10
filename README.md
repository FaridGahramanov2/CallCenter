# CallCenter
Introduction
This report examines a C programming project that develops an echo communication system using client-server architecture. The system consists of a server that handles multiple client connections through socket programming and a client that sends messages to the server.

Implementation Details
Server Module: Implements a multi-threaded server using the POSIX threads library to handle concurrent client connections. It accepts client connections, echoes received messages, and handles connections based on maximum client thresholds.
Client Module: Connects to the server via a specified port and IP address, sends a predefined message, and waits to receive the same message echoed back.
Code Functionality
Client:

Creates a socket and connects to the server.
Sends a "Hello" message.
Receives the echoed message and displays it.
Server:

Listens for client connections on a specified port.
Handles each client in a separate thread.
Echoes back any received messages.
Manages client connections based on the capacity defined.
Challenges Addressed
Managing multiple client connections simultaneously without blocking.
Ensuring thread safety with mutexes during client handling.
Gracefully handling network errors and invalid connections.
Conclusion
The echo communication system efficiently demonstrates the use of socket programming and multi-threading in a networked client-server setup. It provides a robust model for handling multiple client connections concurrently while maintaining stable communication channels.

This document provides a detailed view of the project's scope, challenges, and solutions implemented in the echo communication system.






