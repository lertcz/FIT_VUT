CC = gcc
override CFLAGS += -std=c11 -Wall -Wextra -Werror#-Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-unused-result -Wno-unknown-pragmas -pedantic
EXEC = figsearch

# List of test files
TEST_DIR  = Test_TestCases
VLINE_DIR  = Vline_TestCases
HLINE_DIR  = Hline_TestCases
SQUARE_DIR  = Square_TestCases

# # Default target to build the program
# all: $(EXEC)

# Build the executable from figsearch.c
$(EXEC): figsearch.c
	$(CC) $(CFLAGS) $< -o $(EXEC)
	chmod +x ./$<

# Declare phony targets (these are not files, so always run)
.PHONY: clean test vline hline square

# Run the compiled program (no need for a separate run target)
run: $(EXEC)
	./$(EXEC) test obrazek.txt
	./$(EXEC) hline obrazek.txt
	# hline  - 3 0 3 4
	./$(EXEC) vline obrazek.txt
	# vline  - 0 2 3 2
	./$(EXEC) square obrazek.txt
	# square - 0 2 2 4

# function "tests"
test: $(EXEC)
	./figsearch test $(TEST_DIR)/valid_bitmap.txt
	./figsearch test $(TEST_DIR)/valid_bitmapExtraSpace.txt
	-./figsearch test $(TEST_DIR)/invalid_bitmap.txt
	-./figsearch test $(TEST_DIR)/invalid_bitmap_header.txt
	-./figsearch test $(TEST_DIR)/invalid_bitmap_row.txt
	-./figsearch test $(TEST_DIR)/invalid_bitmap_col.txt
	-./figsearch test $(TEST_DIR)/extra_data.txt
	-./figsearch test $(TEST_DIR)/empty_file.txt
	-./figsearch test $(TEST_DIR)/empty_file_text.txt
	-./figsearch test $(TEST_DIR)/invalid_characters.txt

vline: $(EXEC)
	./figsearch vline $(VLINE_DIR)/vline_single.txt
	./figsearch vline $(VLINE_DIR)/vline_multiple.txt
	./figsearch vline $(VLINE_DIR)/vline_none.txt
	./figsearch vline $(VLINE_DIR)/vline_edge.txt
	./figsearch vline $(VLINE_DIR)/vline_mixed.txt
	./figsearch vline $(VLINE_DIR)/vline_atTheEnd.txt

hline: $(EXEC)
	./figsearch hline $(HLINE_DIR)/hline_single.txt
	./figsearch hline $(HLINE_DIR)/hline_multiple.txt
	./figsearch hline $(HLINE_DIR)/hline_none.txt
	./figsearch hline $(HLINE_DIR)/hline_edge.txt
	./figsearch hline $(HLINE_DIR)/hline_mixed.txt
	./figsearch hline $(HLINE_DIR)/hline_sparse.txt
	./figsearch vline $(VLINE_DIR)/vline_atTheEnd.txt

square: $(EXEC)
	./figsearch square $(SQUARE_DIR)/square_single.txt
	./figsearch square $(SQUARE_DIR)/square_multiple.txt
	./figsearch square $(SQUARE_DIR)/square_none.txt
	./figsearch square $(SQUARE_DIR)/square_edge.txt
	./figsearch square $(SQUARE_DIR)/square_mixed.txt
	./figsearch square $(SQUARE_DIR)/square_sparse.txt
	./figsearch square $(SQUARE_DIR)/square_non_square.txt

# Clean up generated files
clean:
	rm -f $(EXEC) *.o