#!/bin/bash

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Função para imprimir erros
error() {
    echo -e "${RED}Erro: $1${NC}"
    exit 1
}

# Verifica se make está instalado
if ! command -v make &> /dev/null; then
    error "make não está instalado"
fi

# Limpa builds anteriores
echo "Limpando builds anteriores..."
make clean || error "Falha ao limpar builds"

# Compila em modo release
echo "Compilando em modo release..."
make release || error "Falha na compilação"

# Executa o programa
echo -e "${GREEN}Executando MITSP...${NC}"
./build-release/bin/mitsp || error "Falha na execução"