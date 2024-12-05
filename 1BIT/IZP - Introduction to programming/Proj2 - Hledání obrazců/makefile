CC = gcc
override CFLAGS += -std=c11 -Wall -Wextra -Werror#-Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-unused-result -Wno-unknown-pragmas -pedantic
EXEC = figsearch

# List of test files
TEST_DIR  = My_tests/Test_TestCases
VLINE_DIR  = My_tests/Vline_TestCases
HLINE_DIR  = My_tests/Hline_TestCases
SQUARE_DIR  = My_tests/Square_TestCases

# # Default target to build the program
# all: $(EXEC)

# Build the executable from figsearch.c
$(EXEC): figsearch.c
	$(CC) $(CFLAGS) $< -o $(EXEC)
	chmod +x ./$(EXEC)

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
	./figsearch vline $(VLINE_DIR)/vline_single.txt  # 0 1 4 1
	./figsearch vline $(VLINE_DIR)/vline_multiple.txt  # 0 0 5 0
	./figsearch vline $(VLINE_DIR)/vline_dot.txt # 0 1 0 1
	./figsearch vline $(VLINE_DIR)/vline_edge.txt # 0 0 3 0
	./figsearch vline $(VLINE_DIR)/vline_atTheEnd.txt # 0 4 4 4
	./figsearch vline $(VLINE_DIR)/vline_mixed.txt # 0 0 3 0

hline: $(EXEC)
	./figsearch hline $(HLINE_DIR)/hline_single.txt # 2 0 2 4
	./figsearch hline $(HLINE_DIR)/hline_multiple.txt # 0 0 0 5
	./figsearch hline $(HLINE_DIR)/hline_dot.txt # 0 1 0 1
	./figsearch hline $(HLINE_DIR)/hline_edge.txt # 0 0 0 4
	./figsearch hline $(HLINE_DIR)/hline_mixed.txt # 0 0 0 2
	./figsearch hline $(HLINE_DIR)/hline_atTheEnd.txt # 4 0 4 4

square: $(EXEC)
	./figsearch square $(SQUARE_DIR)/square_single.txt # 1 1 2 2
	./figsearch square $(SQUARE_DIR)/square_multiple.txt # 1 1 2 2
	./figsearch square $(SQUARE_DIR)/square_mixed.txt # 1 1 3 3
	./figsearch square $(SQUARE_DIR)/square_dot.txt # 0 1 0 1
	./figsearch square $(SQUARE_DIR)/square_edge.txt # 0 0 1 1
	./figsearch square $(SQUARE_DIR)/square_edge2.txt # 2 2 3 3
	./figsearch square $(SQUARE_DIR)/square_broken.txt # 1 1 1 1
	./figsearch square $(SQUARE_DIR)/square_bigEdge.txt # 0 0 4 4
	./figsearch square $(SQUARE_DIR)/square_full.txt # 0 0 4 4

debug: $(EXEC)
	valgrind --leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--verbose \
			--log-file=valgrind-out.txt \
			./figsearch square obrazek.txt

# Clean up generated files
clean:
	rm -f $(EXEC) *.o