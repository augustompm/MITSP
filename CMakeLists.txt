cmake_minimum_required(VERSION 3.10)
project(MITSP 
    VERSION 1.0
    LANGUAGES CXX
)

# Especifica o padrão C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define o diretório de saída dos executáveis
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Opções de compilação
option(BUILD_TESTS "Build tests" OFF)
option(ENABLE_WARNINGS "Enable warnings" ON)

# Configurar flags de compilação
if(ENABLE_WARNINGS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-Wall -Wextra -Wpedantic)
    elseif(MSVC)
        add_compile_options(/W4)
    endif()
endif()

# Encontrar as dependências
find_package(Threads REQUIRED)

# Lista de arquivos fonte
set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/mainMITSP.cpp
)

# Criar o executável principal
add_executable(mitsp ${SOURCES})

# Configurar includes
target_include_directories(mitsp
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Configurar links
target_link_libraries(mitsp
    PRIVATE
    Threads::Threads
    m
)

# Copiar arquivo de instâncias para o diretório de build/bin
add_custom_command(TARGET mitsp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/instances ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/instances
)

# Adicionar suporte a testes se necessário
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Instalar o executável
install(TARGETS mitsp
    RUNTIME DESTINATION bin
)

# Instalar as instâncias
install(DIRECTORY instances
    DESTINATION share/${PROJECT_NAME}
)

# Mensagem de configuração
message(STATUS "")
message(STATUS "=== Build Configuration ===")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "===========================")
message(STATUS "")