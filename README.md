
# Introduction

This is the 1.0v of the C++ parser, being simple and quite niche. I made it because I needed something to count and jot down statistics of total things done and not done in my .canvas file.

I will include a sample of .canvas and the expected output of it, along with the mechanism to run it in Windows 11 continuously with the help of task schedular soon.

For now, you can manually run it by following the procedures inside the [[#Setup]].

---

# Setup

### Installation

Download the `.exe` file from the [official GitHub repo](https://github.com/Mirza-Ishan-Beg/Obsidian-Canvas-Parser).  
The executable is **standalone**, meaning no installation or external dependencies are needed.

> ⚠️ Note: It must be run via a terminal or script with **two arguments**, or it will fail silently.

### Usage

Notes:
- Only run this from a terminal or script, **not by double-clicking**.
- Ensure the paths are wrapped in quotes if they contain spaces.
- If the destination markdown file does **not** exist, it will be created (along with missing folders).
- If it **does** exist, the file will be **overwritten** with the new content.

Open the terminal at the directory level of the executable file and run:
```shell
.\ObsidianCanvasParser.exe "<PATH\TO\YOUR\CANVAS\FILE.canvas>" "<PATH\TO\MARKDOWN\FILE.md>"
```

### Video Demo
![demo.mp4](assets/demo.mp4)

---
# Credits

- Utilize JSON header file from [GitHub - nlohmann/json: JSON for Modern C++](https://github.com/nlohmann/json)
- Developed using **Qt Creator** and **CMake** on Windows 11.

---
