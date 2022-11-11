TARGET_EXEC := programa-principal
DEBUG_EXEC := programa-debug
TEST_EXEC := programa-testes
TEST_DEBUG_EXEC := programa-debug-testes

BASE_BUILD_DIR := build
BUILD_DIR := $(BASE_BUILD_DIR)/obj
DEBUG_BUILD_DIR := $(BASE_BUILD_DIR)/obj_debug
SRC_DIR := src
INC_DIR := include
RES_DIR := Resultados
TEST_BUILD_DIR := $(BASE_BUILD_DIR)/testes
TEST_SRC_DIR := $(SRC_DIR)/testes
TEST_INC_DIR := $(INC_DIR)/testes
TEST_DEBUG_BUILD_DIR := $(BASE_BUILD_DIR)/testes_debug

CC := gcc

GLIBFLAGS := $(shell pkg-config --cflags --libs gobject-2.0)
STD_FLAGS := -I$(INC_DIR) $(GLIBFLAGS) -Wall -Wextra -pedantic -Wno-unused-parameter
CFLAGS := -O2 $(STD_FLAGS)
DEBUG_FLAGS := -O0 -ggdb3 $(STD_FLAGS)

# get .c files, remove original path and turn into .o
SRCS_ALL := $(shell ls $(SRC_DIR) | grep '.c')
SRCS_ALL := $(subst .c,.o,$(SRCS_ALL))

SRCS_DEBUG := $(SRCS_ALL)

# é as src de teste + as src normains sem o main
SRCS_TEST := $(shell ls $(TEST_SRC_DIR) | grep '.c') $(filter-out main%,$(ALL_SRCS))
SRCS_TEST := $(subst .c,.o,$(SRCS_TEST))

SRCS_TEST_DEBUG := $(SRCS_TEST)

OBJS_ALL := $(SRCS_ALL:%=$(BUILD_DIR)/%)

OBJS_DEBUG := $(SRCS_DEBUG:%=$(DEBUG_BUILD_DIR)/%)

OBJS_TEST := $(SRCS_TEST:%=$(TEST_BUILD_DIR)/%) #$(TEST_BUILD_DIR)/main.o $(TEST_BUILD_DIR)/test_header.o

OBJS_DEBUG_TEST := $(SRCS_TEST_DEBUG:%=$(TEST_DEBUG_BUILD_DIR)/%)

# $(info $$TEST_OBJS is [${TEST_OBJS}])

# make .d
DEPS := $(OBJS_ALL:.o=.d) $(OBJS_DEBUG:.o=.d) $(OBJS_TEST:.o=.d)

CPPFLAGS := -MMD -MP

# final
.PHONY: all
all: $(TARGET_EXEC)

$(TARGET_EXEC): $(OBJS_ALL)
	$(CXX) $(OBJS_ALL) $(GLIBFLAGS) -o $@ $(LDFLAGS)

# C source
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@) $(RES_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: debug
debug: $(DEBUG_EXEC)

$(DEBUG_EXEC): $(OBJS_DEBUG)
	$(CXX) $(OBJS_DEBUG) $(GLIBFLAGS) -o $@ $(LDFLAGS)

$(DEBUG_BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@) $(RES_DIR)
	$(CC) $(CPPFLAGS) $(DEBUG_FLAGS) -c $< -o $@

.PHONY: testes
testes: $(TEST_EXEC) all

$(TEST_EXEC): $(OBJS_TEST)
	$(CXX) $(OBJS_TEST) $(GLIBFLAGS) -o $@ $(LDFLAGS)

# $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
# 	mkdir -p $(dir $@) $(RES_DIR)
# 	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TEST_BUILD_DIR)/%.o: $(TEST_SRC_DIR)/%.c
	mkdir -p $(dir $@) $(RES_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I$(TEST_INC_DIR) -c $< -o $@

.PHONY: testes_debug
testes_debug: $(TEST_DEBUG_EXEC) debug

$(TEST_DEBUG_EXEC): $(OBJS_DEBUG_TEST)
	$(CXX) $(OBJS_DEBUG_TEST) $(GLIBFLAGS) -o $@ $(LDFLAGS)

$(TEST_DEBUG_BUILD_DIR)/%.o: $(TEST_SRC_DIR)/%.c
	mkdir -p $(dir $@) $(RES_DIR)
	$(CC) $(CPPFLAGS) $(DEBUG_FLAGS) -I$(TEST_INC_DIR) -c $< -o $@

.PHONY: clean
RM_DIRS := $(BASE_BUILD_DIR) $(TARGET_EXEC) $(DEBUG_EXEC) $(TEST_EXEC) $(TEST_DEBUG_EXEC) $(RES_DIR)
RED := \033[0;31m
NC := \033[0m
clean:
	@echo -ne '$(RED)Removing:\n$(NC) $(RM_DIRS:%=%\n)'
	-@rm -r $(RM_DIRS) 2>/dev/null || true

.PHONY: clean_res
clean_res:
	rm $(RES_DIR)/*

-include $(DEPS)
