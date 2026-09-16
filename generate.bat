cd build
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 ^
  -DCMAKE_TOOLCHAIN_FILE=S:/Programmation/Libraries/vcpkg/scripts/buildsystems/vcpkg.cmake ^
  -DCMAKE_MAP_IMPORTED_CONFIG_MINSIZEREL=Release ^
  -DCMAKE_MAP_IMPORTED_CONFIG_RELWITHDEBINFO=Release
pause