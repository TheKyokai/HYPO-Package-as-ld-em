
GPP = g++
GPP_FLAGS = -Wall -Wextra
INC_FLAGS = -Iinc
DEP_FLAGS = -MMD -MP


# DIRS
BUILD_DIR = build
BIN_DIR = bin


# SRC
ASSEMBLER_SRC = $(wildcard src/assembler/*.cpp)
LINKER_SRC = $(wildcard src/linker/*.cpp)
EMULATOR_SRC = $(wildcard src/emulator/*.cpp)


# OBJ
ASSEMBLER_OBJ = $(addprefix $(BUILD_DIR)/assembler/, $(notdir $(ASSEMBLER_SRC:.cpp=.o)))
LINKER_OBJ = $(addprefix $(BUILD_DIR)/linker/, $(notdir $(LINKER_SRC:.cpp=.o)))
EMULATOR_OBJ = $(addprefix $(BUILD_DIR)/emulator/, $(notdir $(EMULATOR_SRC:.cpp=.o)))


# DEP
DEPS = $(ASSEMBLER_OBJ:.o=.d) $(LINKER_OBJ:.o=.d) $(EMULATOR_OBJ:.o=.d)


# EXE
ASSEMBLER_EXE = $(BIN_DIR)/assembler
LINKER_EXE = $(BIN_DIR)/linker
EMULATOR_EXE = $(BIN_DIR)/emulator


# Flex and Bison
FLEX_SPEC = misc/flex.l
BISON_SPEC = misc/bison.ypp

FLEX_SRC = src/assembler/scanner.cpp
BISON_SRC = src/assembler/parser.cpp
BISON_HDR = inc/assembler/parser.hpp
BISON_LOC_HDR = inc/assembler/loc.hpp

FLEX_OBJ = $(BUILD_DIR)/assembler/scanner.o
BISON_OBJ = $(BUILD_DIR)/assembler/parser.o


all:  $(ASSEMBLER_EXE) $(LINKER_EXE) $(EMULATOR_EXE)

generate: $(BISON_SRC) $(FLEX_SRC)

# Linking
$(ASSEMBLER_EXE): $(BISON_OBJ) $(FLEX_OBJ) $(ASSEMBLER_OBJ)  | $(BIN_DIR)
	$(GPP) $^ -o $@

$(LINKER_EXE): $(LINKER_OBJ) | $(BIN_DIR)
	$(GPP) $^ -o $@

$(EMULATOR_EXE): $(EMULATOR_OBJ) | $(BIN_DIR)
	$(GPP) $^ -o $@


# Compilation
$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(GPP) $(GPP_FLAGS) $(INC_FLAGS) $(DEP_FLAGS) -c $< -o $@


# Flex and bison generation
$(FLEX_SRC): $(FLEX_SPEC) 
	flex -o $@ $<

$(BISON_SRC) $(BISON_HDR): $(BISON_SPEC)
	bison --defines=$(BISON_HDR) -o $(BISON_SRC) $<

# Flex and Bison compilation
$(FLEX_OBJ): $(FLEX_SRC) | $(BUILD_DIR)
	$(GPP) $(GPP_FLAGS) $(INC_FLAGS) -c $< -o $@

$(BISON_OBJ): $(BISON_SRC) | $(BUILD_DIR)
	$(GPP) $(GPP_FLAGS) $(INC_FLAGS) $(INC_FLAGS)/assembler -c $< -o $@



# DIR Creation
$(BIN_DIR):
	mkdir -p $@

$(BUILD_DIR):
	mkdir -p $@/assembler $@/linker $@/emulator


clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(FLEX_SRC) $(BISON_SRC) $(BISON_HDR) $(BISON_LOC_HDR)


-include $(DEPS)

.PHONY: all generate_lexer_and_parser clean