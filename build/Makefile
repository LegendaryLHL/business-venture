CXX = g++
CC = gcc

ifdef DEBUG
    CFLAGS = -std=c++17 -g -O0
    CFLAGS_C = -std=c11 -g -O0
else
    CFLAGS = -std=c++17 -O2
    CFLAGS_C = -std=c11 -O2
endif

LDFLAGS = -lglfw -lGL -ldl -lpthread -lX11 -lXrandr -lXi
SRC = ../src
INCLUDE = -I../include
TARGET = businessVenture
EXTERNAL = ../external

EFILE = glad.o stb_image.o

OBJS = main.o core/Game.o core/Shader.o core/Asset.o entity/Building.o entity/Entity.o

all: $(TARGET)

$(OBJS): %.o: $(SRC)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(EFILE): %.o: $(EXTERNAL)/%.c
	$(CC) $(CFLAGS_C) $(INCLUDE) -c $< -o $@

$(TARGET): $(OBJS) $(EFILE)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: test clean

test: $(TARGET)
	./$(TARGET)

clean:
	find . -type f ! -name 'Makefile' ! -name 'compile_commands.json' -exec rm -f {} \;
	find . -type d ! -name '.' ! -name 'exception_file_placeholder' -exec rm -rf {} \;
