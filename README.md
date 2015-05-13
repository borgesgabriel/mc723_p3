# Plataforma base para MC723 #

## O que é uma plataforma? ##

Uma *plataforma* é um software capaz de emular/simular um hardware com características reais. No nosso caso, temos um processador MIPS, como já utilizado no projeto anterior, um módulo de memória e um barramento, além do software que executará sobre esta plataforma.

## Como utilizar a plataforma base? ##

No caso geral, dois comandos são necessários para compilar e executar a plataforma:

{
make
make run
}

ou, de forma simplificada

{
make all run
}

O primeiro comando (_make all_) compila toda a plataforma, incluindo as pastas com cada um dos componentes. O segundo comando (_make run_) executa a plataforma com os parâmetros corretos para executar o software simulado. No nosso exemplo, este é um Hello World.

