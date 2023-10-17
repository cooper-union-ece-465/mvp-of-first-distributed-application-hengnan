# Server-Client Program with Boost Asio

This repository contains a simple server-client application using the Boost Asio library in C++. The program enables file upload, download, and the ability to quit the client application. Below is an overview of the components and usage of this application.

## Features

- The server listens for incoming client connections and processes client requests.
- The client connects to the server using the host and port specified as command-line arguments.
- The client can upload a file to the server, download a file from the server, or quit the application.

## Prerequisites

- C++ compiler supporting C++11 or higher.
- Boost Asio library.

## Usage

### Server

1. Compile the server program using a C++ compiler. You can use g++:

  
   g++ server.cpp -o server
   

2. Run the server, specifying the port number you want to listen on:

   
   ./server <port>
  

3. The server will start listening for incoming connections.

### Client

1. Compile the client program using a C++ compiler:

   
   g++ client.cpp -o client
   

2. Run the client, specifying the server's host and port:

   
   ./client <host> <port>
   

3. The client application will connect to the server, and you will be prompted to choose from the following options:
   - **Upload**: Upload a file to the server.
   - **Download**: Download a file from the server.
   - **Quit**: Quit the client application.

## Server-Client Interaction

- The server listens for incoming client connections and accepts multiple client connections simultaneously.
- The server receives requests from clients to upload, download, or quit.
- The client interacts with the server, sends files for upload or requests downloads, and can quit the application.

## Sample Usage

Suppose you have a server running on host `localhost` and port `9999`. Here's a sample session:

1. Run the client:

   
   ./client localhost 9999
   

2. Choose an option(Not case sensitive):
   - Enter `UPLOAD` to upload a file to the server.
   - Enter `DOWNLOAD` to download a file from the server.
   - Enter `QUIT` to exit the client.


