# Simple cross-platform C++ starter

This project builds a single executable without CMake or any external build system.
It follows the raylib VSCode template style but keeps script-based builds (`build.ps1`/`build.sh`) instead of make tasks.

## Structure

- `src/main.cpp` contains the program entry point.
- `build.ps1` builds on Windows with `cl`, `g++`, or `clang++`.
- `build.sh` builds on Linux and macOS with `c++`, `g++`, or `clang++`.
- `.vscode/tasks.json` adds a default VS Code build task.

## Build

### Windows

Run:

```powershell
./build.ps1
```

If Raylib is installed globally at `C:\raylib`, `build.ps1` will detect it and add include/link flags automatically.
When only MinGW Raylib (`libraylib.a`) is present, `build.ps1` prefers `g++`/`clang++` over `cl`.
You can also override the location with:

```powershell
$env:RAYLIB_DIR = "D:\path\to\raylib"
./build.ps1
```

### Linux or macOS

Run:

```sh
chmod +x build.sh
./build.sh
```

## Output

The executable is written to `build/app.exe` on Windows and `build/app` on Linux or macOS.

## VS Code

Use `Terminal: Run Build Task` or press `Ctrl+Shift+B` to build from VS Code.