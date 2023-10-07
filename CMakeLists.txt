cmake_minimum_required(VERSION 3.16)
project(epidemic VERSION 0.1.0)

# abilita il supporto per i test, tra cui l'opzione BUILD_TESTING usata sotto
include(CTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# richiedi l'uso di C++17, senza estensioni non-standard offerte dal compilatore usato 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# richiedi SFML, versione 2.5
# le dipendenze vengono identificate automaticamente
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

# abilita warning
string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")

# abilita l'address sanitizer e l'undefined-behaviour sanitizer in debug mode
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer")

add_executable(functions main.cpp functions/functions.cpp graph/draw.cpp graph/curve/curve.cpp graph/text/text.cpp graph/grid/grid.cpp)  # cosa sono le prime cose che si scrivono senza .cpp?
target_link_libraries(functions PRIVATE sfml-graphics)

# se il testing e' abilitato...
#   per disabilitare il testing, passare -DBUILD_TESTING=OFF a cmake durante la fase di configurazione
if (BUILD_TESTING)

  # aggiungi l'eseguibile functions.t #che cos'è questo file? A cosa serve?
  add_executable(functions.t functions/functions.test.cpp functions/functions.cpp graph/draw.cpp graph/curve/curve.cpp graph/text/text.cpp graph/grid/grid.cpp)
  # aggiungi l'eseguibile functions.t alla lista dei test
  add_test(NAME functions.t COMMAND functions.t)
  target_link_libraries(functions.t PRIVATE sfml-graphics)

endif()
