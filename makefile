TARGET_EXEC := programa-principal

BUILD_DIR := src/obj
SRC_DIR := src
INC_DIR := include
RES_DIR := Resultados

CC := gcc

GLIBFLAGS := $(shell pkg-config --cflags --libs gobject-2.0)
CFLAGS := -I$(INC_DIR) $(GLIBFLAGS) -O2 -Wall -Wextra -pedantic -Wno-unused-parameter #-O0 -ggdb3

# get .c files, remove original path and turn into .o
SRCS := $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.c' -or -name '*.s')
SRCS := $(subst $(SRC_DIR)/,,$(SRCS))
SRCS := $(subst .c,.o,$(SRCS))

OBJS := $(SRCS:%=$(BUILD_DIR)/%)

# make .d
DEPS := $(OBJS:.o=.d)

CPPFLAGS := -MMD -MP

# final
$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) $(GLIBFLAGS) -o $@ $(LDFLAGS)

# C source
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@) $(RES_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
	rm programa-principal
	rm -r $(RES_DIR)

.PHONY: clean_res
clean_res:
	rm $(RES_DIR)/*

-include $(DEPS)
