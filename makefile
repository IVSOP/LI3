TARGET_EXEC := programa-principal
DEBUG_EXEC := debug
TEST_EXEC := programa-testes

BASE_BUILD_DIR := build
BUILD_DIR := $(BASE_BUILD_DIR)/obj
DEBUG_BUILD_DIR := $(BASE_BUILD_DIR)/obj_debug
SRC_DIR := src
INC_DIR := include
RES_DIR := Resultados
TEST_BUILD_DIR := $(BASE_BUILD_DIR)/testes
TEST_SRC_DIR := $(SRC_DIR)/testes

CC := gcc

GLIBFLAGS := $(shell pkg-config --cflags --libs gobject-2.0)
STD_FLAGS := -I$(INC_DIR) $(GLIBFLAGS) -Wall -Wextra -pedantic -Wno-unused-parameter
CFLAGS := -O2 $(STD_FLAGS)
DEBUG_FLAGS := -O0 -ggdb3 $(STD_FLAGS)

# get .c files, remove original path and turn into .o
SRCS := $(shell ls $(SRC_DIR) | grep '.c')
# SRCS := $(subst $(SRC_DIR)/,,$(SRCS))
SRCS := $(subst .c,.o,$(SRCS))
SRCS := $(filter-out main%,$(SRCS))
SRCS_DEBUG := $(subst .o,.debug.o,$(SRCS))

ALL_OBJS := $(SRCS:%=$(BUILD_DIR)/%)
OBJS := $(ALL_OBJS) $(BUILD_DIR)/main.o
OBJS_DEBUG := $(SRCS_DEBUG:%=$(DEBUG_BUILD_DIR)/%)
TEST_OBJS := $(ALL_OBJS) $(TEST_BUILD_DIR)/main.o

$(info $$TEST_OBJS is [${TEST_OBJS}]) # esta certo

# make .d
DEPS := $(OBJS:.o=.d)

CPPFLAGS := -MMD -MP

# final
.PHONY: all
all: $(TARGET_EXEC)

$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) $(GLIBFLAGS) -o $@ $(LDFLAGS)

# C source
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@) $(RES_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: debug
debug: $(DEBUG_EXEC)

$(DEBUG_EXEC): $(OBJS_DEBUG)
	$(CXX) $(OBJS_DEBUG) $(GLIBFLAGS) -o $@ $(LDFLAGS)

$(DEBUG_BUILD_DIR)/%.debug.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@) $(RES_DIR)
	$(CC) $(CPPFLAGS) $(DEBUG_FLAGS) -c $< -o $@

.PHONY: tests
tests: $(TEST_EXEC)

$(TEST_EXEC): $(TEST_OBJS)
	$(CXX) $(TEST_OBJS) $(GLIBFLAGS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@) $(RES_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TEST_BUILD_DIR)/%.o: $(TEST_SRC_DIR)/%.c
	mkdir -p $(dir $@) $(RES_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BASE_BUILD_DIR) $(TARGET_EXEC) $(DEBUG_EXEC) $(RES_DIR)

.PHONY: clean_res
clean_res:
	rm $(RES_DIR)/*

-include $(DEPS)
