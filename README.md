# NextJam

Game for https://itch.io/jam/raylib-next-gamejam

# Build

All command in ```./vscode/tasks.json``` file

As example for web:

- emcmake cmake -S . -B ./build/web -DCMAKE_BUILD_TYPE=Debug -DPLATFORM=Web -G Ninja
- cmake --build ./build/web
- emrun ./build/web/NextJam.html

But in VSCode use:
- CMake extension (as best)
- ```Ctrl-Shift-P``` -> ```Tasks: Run Build Task``` -> ```web build and run``` (as generic)

# Links

- Palette https://lospec.com/palette-list/cl8uds
- Font https://datagoblin.itch.io/monogram
