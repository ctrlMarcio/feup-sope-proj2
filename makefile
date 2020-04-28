CC=gcc
BIN_DIR=bin
SRC_DIR=src

# The names of the .c files to compile
CLIENT_FILES = $(SRC_DIR)/client/client.c # separated by space
SERVER_FILES = $(SRC_DIR)/server/server.c # separated by space

make: ${FILES}
	@mkdir -p $(BIN_DIR)
	@$(CC) -Wall -o $(BIN_DIR)/U1 ${CLIENT_FILES} -lm -lrt -lpthread
	@$(CC) -Wall -o $(BIN_DIR)/Q1 ${SERVER_FILES} -lm -lrt -lpthread