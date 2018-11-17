CC := gcc
SRC := src/
INCLUDE := include/
BIN := bin/
PROJECT := TSP

clean:
	@rm -rf $(BUILD)
	@rm -rf $(BIN)

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
