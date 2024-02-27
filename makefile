include mono2-before.mak

# file manager needs to know app name for calculating file paths
DEBUG_CC_FLAGS += -D cdwAppDataName=\"grey\"
RELEASE_CC_FLAGS += -D cdwAppDataName=\"grey\"

# add client components to all
all: \
	$(OUT_DIR)/debug/grey.exe \
	$(OUT_DIR)/release/grey.exe \
	mono2_all

include mono2-after.mak

.PHONY: all

# write rules for client components using those in mono2-after as examples

# ----------------------------------------------------------------------
# grey

GREYT_SRC = \
	src/grey/help.cpp \
	src/grey/main.cpp \
	src/grey/verb.inter.cpp \

GREYT_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(GREYT_SRC)))

$(OUT_DIR)/debug/grey.exe: $(GREYT_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib $(OUT_DIR)/debug/cui.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -o $@ $(GREYT_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib -lcui

$(GREYT_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/grey
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

GREYT_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(GREYT_SRC)))

$(OUT_DIR)/release/grey.exe: $(GREYT_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib $(OUT_DIR)/release/cui.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -o $@ $(GREYT_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib -lcui

$(GREYT_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/grey
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@
