CC := gcc
SRC := src/
INCLUDE := include/
BIN := bin/
PROJECT := TSP

clean:
	@rm -rf $(BUILD)
	@rm -rf $(BIN)

GA: $(SRC)GA.c $(SRC)utilities.c
	@mkdir -p $(BIN)
	@$(CC) $^ -lm -o $(BIN)GA
	@$(BIN)GA

testGA: $(SRC)GA.c $(SRC)GA_test.cpp $(SRC)utilities.c
	@mkdir -p $(BIN)
	@g++ $^ -lgtest -lpthread -std=c++0x -lm -o $(BIN)GA_TEST
	@$(BIN)GA_TEST

RRHC: $(SRC)RRHC.c $(SRC)utilities.c
	@mkdir -p $(BIN)
	@$(CC) $^ -lm -o $(BIN)RRHC
	@$(BIN)RRHC

HC: $(SRC)HC.c $(SRC)utilities.c
	@mkdir -p $(BIN)
	@$(CC) $^ -lm -o $(BIN)HC
	@$(BIN)HC

SA: $(SRC)SA.c $(SRC)utilities.c
	@mkdir -p $(BIN)
	@$(CC) $^ -lm -o $(BIN)SA
	@$(BIN)SA
