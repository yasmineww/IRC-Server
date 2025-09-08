# IRC Server - C++ Project (42 Cursus)

## Overview

This project implements a simple **IRC (Internet Relay Chat) server** in C++ following the specifications of the 42 Cursus.  
It demonstrates **socket programming**, **network protocols**, and **multi-client handling** using `poll()` for concurrency.  

The server allows multiple clients to connect, join channels, send messages, and manage basic IRC commands.

---

## Features

- Multi-client support using **poll()** (no threads required)
- Support for basic IRC commands:  
  - `PASS` – set password for server authentication  
  - `NICK` – set nickname  
  - `USER` – set username and real name  
  - `JOIN` – join a channel  
  - `PART` – leave a channel  
  - `PRIVMSG` – send messages to a user or channel  
  - `NOTICE` – send notices  
  - `QUIT` – disconnect gracefully
- Channel management with multiple users
- Server password authentication
- Handles invalid commands with proper error messages
- Robust error handling for invalid input and connection issues

---

## Installation

1. **Clone the repository**

```bash
git clone https://github.com/yasmineww/IRC-Server
cd irc-server
```
2. **Compile the Server**
```bash
make
```
3. **Run the Server**
```bash
./ircserv <PORT> <PASSWORD>
```

3. **Connecting a Client**
-You can connect using any IRC client or simple tools like netcat

```bash
nc 127.0.0.1 6667
```

## . Technical Details

- Written in **C++** (C++98 standard)  
- Uses **BSD sockets** for network communication  
- Uses **poll()** for managing multiple simultaneous client connections  
- Implements basic IRC protocol messages and parsing  
- Handles errors gracefully and sends appropriate IRC error messages



