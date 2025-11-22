<<<<<<< HEAD

 C++ Chat Application (Windows)

A simple console-based TCP chat system built using C++17 and Winsock2, supporting multiple clients connected to a single server.
Designed and tested only for Windows using Visual Studio.



 📌 Features

 Multi-client chat room
 Real-time message broadcasting
 Join/leave notifications
 Lightweight and easy to run
 Pure C++ + Winsock (no external libraries)



 🛠️ Requirements

 Windows 10/11
 Visual Studio 2022
 Installed workloads:

   Desktop development with C++
   Includes MSVC v143 build tools + Windows SDK

That's all you need.



 🧩 Building Using Visual Studio

 1️⃣ Open the Project

1. Open Visual Studio
2. Go to: File → Open → Project/Solution
3. Open:

    `ChatServer.sln`
    `ChatClient.sln`

You can open them separately or combine both into one solution.



 2️⃣ Configure Build Settings

Inside Visual Studio:

 Configuration: `Debug`
 Platform: `x64`

Path:
`Build → Configuration Manager → Active solution platform → x64`



 3️⃣ Build the Binaries

Go to:

```
Build → Build Solution  (F7)
```

Visual Studio will generate:

 `ChatServer.exe`
 `ChatClient.exe`

Located in:

```
/x64/Debug/
```



 ▶️ Running the Application

 🖥️ Step 1: Start the Server

Open a terminal inside:

```
ChatServer/x64/Debug/
```

Run:

```
ChatServer.exe
```

Example output:

```
Chat server started on port 8080
Waiting for clients...
```



 🖥️ Step 2: Start a Client

Open another terminal inside:

```
ChatClient/x64/Debug/
```

Run:

```
ChatClient.exe
```

Example:

```
Connected to chat server!
Enter your name:
```



 🖥️ Step 3: Run Multiple Clients

Each new client joins the same chat room.

Server:

```
 Muskan joined the chat 
Muskan: Hello!
```

Client:

```
=== Chat Started ===
Hello!
```



 📁 Project Structure

```
chatapp/
│
├── ChatServer/
│   ├── ChatServer.cpp
│   ├── ChatServer.sln
│   └── x64/Debug/ChatServer.exe
│
├── ChatClient/
│   ├── ChatClient.cpp
│   ├── ChatClient.sln
│   └── x64/Debug/ChatClient.exe
│
└── README.md
```



 🛡️ Troubleshooting

 ❗ Port Already in Use

```
Failed to bind socket
```

Fix:

```
netstat -ano | findstr :8080
```

Stop the conflicting process.



 ❗ Client Cannot Connect

 Start server first
 Confirm port (default 8080)
 Allow Visual Studio through Windows Firewall
 If using different machines → ensure they are on the same network



 📄 License

Free to use, modify, and learn.



If you want, I can also add:
✅ Screenshot of terminal
✅ Architecture diagram
✅ Project badges
✅ GIF setup demo

Just tell me!
=======
# chatapp
>>>>>>> f929ba91dec010047f245e5d3316835b8ce9a058
