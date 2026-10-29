NOME = ex07
# Variáveis do compilador e flags
CC = gcc
CFLAGS = $(shell pkg-config --cflags gtk4)
LIBS = $(shell pkg-config --libs gtk4)

# Nome do executável final e arquivos fonte
TARGET = $(NOME)
SRCS = $(NOME).c

# Regra padrão (executada ao rodar apenas 'make')
all: $(TARGET)

# Regra de compilação
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

# Regra para limpar os arquivos gerados
clean:
	rm -f $(TARGET)

# Declara que 'all' e 'clean' não são arquivos reais
.PHONY: all clean
