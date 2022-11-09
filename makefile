TARGET_EXEC := programa-principal
DEBUG_EXEC := debug

BUILD_DIR := src/obj
DEBUG_BUILD_DIR := src/obj_debug
SRC_DIR := src
INC_DIR := include
RES_DIR := Resultados

CC := gcc

GLIBFLAGS := $(shell pkg-config --cflags --libs gobject-2.0)
STD_FLAGS := -I$(INC_DIR) $(GLIBFLAGS) -Wall -Wextra -pedantic -Wno-unused-parameter
CFLAGS := -O2 $(STD_FLAGS)
DEBUG_FLAGS := -O0 -ggdb3 $(STD_FLAGS)

# get .c files, remove original path and turn into .o
SRCS := $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.c' -or -name '*.s')
SRCS := $(subst $(SRC_DIR)/,,$(SRCS))
SRCS := $(subst .c,.o,$(SRCS))

SRCS_DEBUG := $(subst .o,.debug.o,$(SRCS))

OBJS := $(SRCS:%=$(BUILD_DIR)/%)
OBJS_DEBUG := $(SRCS_DEBUG:%=$(DEBUG_BUILD_DIR)/%)

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


.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
	rm -r $(DEBUG_BUILD_DIR)
	rm programa-principal
	rm debug
	rm -r $(RES_DIR)

.PHONY: clean_res
clean_res:
	rm $(RES_DIR)/*

-include $(DEPS)
