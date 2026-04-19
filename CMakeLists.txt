cmake_minimum_required(VERSION 3.20)
project(TrabalhoOrdenacao C)

# Define o padrão da linguagem C para C11 
set(CMAKE_C_STANDARD 11)

# 1. Inclui o diretório raiz para que o compilador ache o 'ordenacao.h' 
include_directories(${CMAKE_SOURCE_DIR})

# 2. Define o nome do seu programa e qual arquivo .c ele usa 
# Se o seu arquivo principal tiver outro nome, mude 'main.c' abaixo
add_executable(trabalho main.c)

# 3. Faz a linkagem da biblioteca estática 
# Verifique se a pasta se chama 'lib' e o arquivo 'libordenacao15.a'
target_link_libraries(trabalho ${CMAKE_SOURCE_DIR}/lib/libordenacao15.a)