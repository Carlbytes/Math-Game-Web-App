C++ Web Math Game

This is a simple web-based math game that demonstrates a modern web architecture:

- Frontend: A lightweight, responsive HTML, CSS, and JavaScript interface (served from static/index.html).

- Backend: A high-performance C++ server using the Crow C++ micro-framework.

The C++ backend is responsible for generating math problems and verifying user answers, which it provides to the frontend via a simple JSON API.

## How To set up and run this project?

This Project is Built using a build and run .sh file, as the Backend is designed to run on a server, and thus is built for linux systems.
It is done this way for simplicity, to avoid docker containers at the expense of more setup on the client side.

If this was to ever make it into production, it would be hosted properly on a server, but for now it must be ran locally.

### Windows

1. Open PowerShell or Command Prompt as an Administrator
2. Run the following command to install WSL

```
wsl --install
```
3. Restart your computer when prompted.

4. In command prompt, type the following command to verify Ubuntu version install. If an installation appears, you may skip Step 5.
```
wsl --list
```
5. Install Ubuntu, You can download any Ubuntu version from the microsoft store, it is recommended to pick an LTS version, but is not needed.
6. Now, in your start menu, search for Ubuntu. Opening it will open a linux terminal enviornment. This is where we will compile and run our program
   
8. On the first launch, it will ask you to create a UNIX username and password. These are for your new Linux environment (they don't need to match your Windows login). Something like "name" and "admin" will suffice here.
9. Inside the new Ubuntu Terminal, follow the steps inside the linux setup.

### Linux

1. Download The Project Folder, or clone the project
2. Checkout project folder
For Example: (Windows Via WSl terminal)
```
cd /mnt/c/Users/Sam/Desktop/Math_Game
```
For Example; (Linux native)
```
cd ~/Desktop/Math_Game
```

3. Update System and download Dependancies
```
sudo apt-get update
```

```
sudo apt-get install build-essential libasio-dev
```

4. (This should be done already) Make the Script Executeable. This only needs to be done once, so you may ignore this step unless the script will not run in the following step.
```
chmod +x build_and_run.sh
```
5. Run the Script
```
./build_and_run.sh
```
If successful, you will see the output: Starting server on port 18080...

6. Open a web browser, and navigate to: http://localhost:18080

### And you are done!

It is important to note that this setup is only needed once, and it is not the only way to run this program (obviously)
Running in the future just requires a repeat of step 5 and 6 above while checked out in the correct directory.
You may also manually run it. The program can be ran natively on Windows also using Visual Studio, and microsofts compiler. however crow must be set up differently here, using the vcpkg package manager, and CMake, rather than G++
This is not something i have messed around with. Feel free to update this file with instructions on how to set that up.

## Editing in Visual Studio Code on windows.

This will allow you to edit the code, while still compiling inside your linux enviornment.


1. Install VS Code, Download and install [Visual Studio Code](https://code.visualstudio.com/download) on your Windows machine.

2. Install the WSL Extension:

- Open VS Code.
- Click the Extensions icon on the left-hand sidebar (it looks like four squares).
- Search for WSL and install the official extension by Microsoft.

3. Close all VS Code instances.
4. Open your Ubuntu (WSL) terminal (from the Start Menu).
5. Navigate to your project directory:
```
cd YOUR_REPOSITORY
```
6. In that folder, type the following, and press enter.
```
code .
```

That command then should install some dependancies if it is your first time running it, and then open a VS Code window.
You'll see a box in the bottom-left corner that says "WSL: Ubuntu".
To compile and run your server, just open the built-in terminal in VS Code (View -> Terminal or `Ctrl+``). This terminal is already inside your Ubuntu environment in the correct folder.
n the VS Code terminal, just type:
```
./build_and_run.sh
```



























