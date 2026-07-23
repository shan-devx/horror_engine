# rlgame_engine
Game engine which is basically imgui wrapper of raylib (but everything sucks

<img width="1280" height="720" alt="image" src="https://github.com/user-attachments/assets/ef2a5bc4-a0a4-4c62-b79c-9019ec68494b" />

# Dependencies
- Cmake (v3.14+)
- C++20 compatible compiler (g++)

# How to Build ,Run
1. **Clone the repository**
```bash
git clone https://github.com/shan-devx/rlgame_engine.git
cd rlgame_engine
```
2. **Build the project using CMake**
```bash
mkdir build
cd build
cmake ..
make
```
3. **Run the engine**
```bash
./rlgame_engine
```
- Do some stuff, click on save, and close 
4. **Export game**
- Go to the src directory
- Build using cmake
```bash
cd src
mkdir build
cd build
cmake ..
make
```
5. **Run game**
```bash
./game
```
