# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wvla -Wextra -Werror -D_GNU_SOURCE

LINKS = lib/code.c lib/handler.c lib/semaphore.c lib/shared_memory.c lib/conf.c 

# Source files
MASTER_SRC = src/master.c $(LINKS)
ALIMENTAZIONE_SRC = src/alimentazione.c $(LINKS)
ATOMO_SRC = src/atomo.c $(LINKS)
ATTIVATORE_SRC = src/attivatore.c $(LINKS)
INIBITORE_SRC = src/inibitore.c $(LINKS)

# Output executables in bin directory
BIN_DIR = bin
MAIN_TARGET = $(BIN_DIR)/master
ALIMENTAZIONE_TARGET = $(BIN_DIR)/alimentazione
ATOMO_TARGET = $(BIN_DIR)/atomo
ATTIVATORE_TARGET = $(BIN_DIR)/attivatore
INIBITORE_TARGET = $(BIN_DIR)/inibitore



# Default target
all: $(MAIN_TARGET) $(ALIMENTAZIONE_TARGET) $(ATOMO_TARGET) $(ATTIVATORE_TARGET) $(INIBITORE_TARGET)

# Ensure the bin directory exists before building executables
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Build the main executable
$(MAIN_TARGET): $(MASTER_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(MAIN_TARGET) $(MASTER_SRC)

# Build executables for alimentazione, atomo, and attivatore
$(ALIMENTAZIONE_TARGET): $(ALIMENTAZIONE_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(ALIMENTAZIONE_TARGET) $(ALIMENTAZIONE_SRC)

$(ATOMO_TARGET): $(ATOMO_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(ATOMO_TARGET) $(ATOMO_SRC)

$(ATTIVATORE_TARGET): $(ATTIVATORE_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(ATTIVATORE_TARGET) $(ATTIVATORE_SRC)

$(INIBITORE_TARGET): $(INIBITORE_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(INIBITORE_TARGET) $(INIBITORE_SRC)

# Clean target
clean:
	rm -f $(MAIN_TARGET) $(ALIMENTAZIONE_TARGET) $(ATOMO_TARGET) $(ATTIVATORE_TARGET) $(INIBITORE_TARGET) 
	ipcrm -a 

# Run targets
run: $(MAIN_TARGET)
	./$(MAIN_TARGET)



# Phony targets
.PHONY: all clean run run_no_inibitore run_inibitore
