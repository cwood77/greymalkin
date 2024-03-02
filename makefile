include mono2-before.mak

# file manager needs to know app name for calculating file paths
DEBUG_CC_FLAGS += -D cdwAppDataName=\"grey\"
RELEASE_CC_FLAGS += -D cdwAppDataName=\"grey\"

# add client components to all
all: \
	$(OUT_DIR)/debug/content.dll \
	$(OUT_DIR)/debug/grey.exe \
	$(OUT_DIR)/debug/window.dll \
	$(OUT_DIR)/release/content.dll \
	$(OUT_DIR)/release/grey.exe \
	$(OUT_DIR)/release/window.dll \
	mono2_all


include mono2-after.mak

.PHONY: all

# write rules for client components using those in mono2-after as examples

# ----------------------------------------------------------------------
# content

CONTENT_SRC = \
	src/content/api.cpp \
	src/content/command.cpp \
	src/content/main.cpp \
	src/content/vPort.cpp \

CONTENT_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(CONTENT_SRC)))

$(OUT_DIR)/debug/content.dll: $(CONTENT_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib $(OUT_DIR)/debug/cui.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -shared -o $@ $(CONTENT_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib -lcui

$(CONTENT_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/content
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

CONTENT_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(CONTENT_SRC)))

$(OUT_DIR)/release/content.dll: $(CONTENT_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib $(OUT_DIR)/release/cui.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -shared -o $@ $(CONTENT_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib -lcui

$(CONTENT_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/content
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

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

# ----------------------------------------------------------------------
# window

WINDOW_SRC = \
	src/window/api.cpp \
	src/window/main.cpp \

WINDOW_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(WINDOW_SRC)))

$(OUT_DIR)/debug/window.dll: $(WINDOW_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib $(OUT_DIR)/debug/cui.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -shared -o $@ $(WINDOW_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib -lcui

$(WINDOW_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/window
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

WINDOW_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(WINDOW_SRC)))

$(OUT_DIR)/release/window.dll: $(WINDOW_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib $(OUT_DIR)/release/cui.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -shared -o $@ $(WINDOW_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib -lcui

$(WINDOW_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/window
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@
