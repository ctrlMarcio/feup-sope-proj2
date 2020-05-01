CC=gcc
BIN_DIR=bin
SRC_DIR=src

# Compiling extensions
COMPILATION_EXTENSIONS = -Wall -g -o
LINKED_LIBRARIES = -lm -lrt -lpthread

# The names of the .c files to compile
CLIENT_FILES = $(SRC_DIR)/client/client.c $(SRC_DIR)/client/client_arguments.c $(SRC_DIR)/client/client_fifo.c $(SRC_DIR)/client/client_thread.c $(SRC_DIR)/util/message.c # separated by space
SERVER_FILES = $(SRC_DIR)/server/server.c # separated by space

make: ${FILES}
	@mkdir -p $(BIN_DIR)
	@$(CC) ${COMPILATION_EXTENSIONS} $(BIN_DIR)/U1 ${CLIENT_FILES} ${LINKED_LIBRARIES}
	@$(CC) ${COMPILATION_EXTENSIONS}  $(BIN_DIR)/Q1 ${SERVER_FILES} ${LINKED_LIBRARIES}