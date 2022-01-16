CC = clang
INCFLAGS = -I..
DEBUG_CC_FLAGS = -std=c17 -c -g -D_DEBUG $(INCFLAGS)
RELEASE_CC_FLAGS = -std=c17 -c -O3 -DNDEBUG $(INCFLAGS)

DEBUG_TARGET_DIR = ../debug
RELEASE_TARGET_DIR = ../release
DEBUG_OBJECT_DIR = $(DEBUG_TARGET_DIR)/o
RELEASE_OBJECT_DIR = $(RELEASE_TARGET_DIR)/o
DEBUG_OBJECTS = $(DEBUG_OBJECT_DIR)/NLSLsolver.o \
	$(DEBUG_OBJECT_DIR)/NLSLvars.o $(DEBUG_OBJECT_DIR)/NLSLutils.o $(DEBUG_OBJECT_DIR)/NLSLmatrix.o \
	$(DEBUG_OBJECT_DIR)/NLSLgnsolver.o
RELEASE_OBJECTS = $(RELEASE_OBJECT_DIR)/NLSLsolver.o \
	$(RELEASE_OBJECT_DIR)/NLSLvars.o $(RELEASE_OBJECT_DIR)/NLSLutils.o $(RELEASE_OBJECT_DIR)/NLSLmatrix.o \
	$(RELEASE_OBJECT_DIR)/NLSLgnsolver.o
DEBUG_TARGET = $(DEBUG_TARGET_DIR)/libNLSL.a
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/libNLSL.a

# ----------- debug --------------------
debug	:	$(DEBUG_TARGET)
$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	$(AR) r $@ $^
	$(RANLIB) $@

$(DEBUG_OBJECT_DIR)/NLSLapp.o	:	NLSLapp.c
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/NLSLsolver.o	:	NLSLsolver.c
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/NLSLvars.o	:	NLSLvars.c
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/NLSLutils.o	:	NLSLutils.c
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/NLSLmatrix.o	:	NLSLmatrix.c
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/NLSLgnsolver.o	:	NLSLgnsolver.c NLSLgnsolver.h NLSLsolver.h
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

clean_debug	:
	rm -f $(DEBUG_OBJECTS)
	rm -f $(DEBUG_TARGET)

# ----------- release --------------------
release	:	$(RELEASE_TARGET)
$(RELEASE_TARGET)	:	$(RELEASE_OBJECTS)
	$(AR) r $@ $^
	$(RANLIB) $@

$(RELEASE_OBJECT_DIR)/NLSLapp.o	:	NLSLapp.c
	mkdir -p $(RELEASE_OBJECT_DIR)
	$(CC) $(RELEASE_CC_FLAGS) -o $@ $<

$(RELEASE_OBJECT_DIR)/NLSLsolver.o	:	NLSLsolver.c
	mkdir -p $(RELEASE_OBJECT_DIR)
	$(CC) $(RELEASE_CC_FLAGS) -o $@ $<

$(RELEASE_OBJECT_DIR)/NLSLvars.o	:	NLSLvars.c
	mkdir -p $(RELEASE_OBJECT_DIR)
	$(CC) $(RELEASE_CC_FLAGS) -o $@ $<

$(RELEASE_OBJECT_DIR)/NLSLutils.o	:	NLSLutils.c
	mkdir -p $(RELEASE_OBJECT_DIR)
	$(CC) $(RELEASE_CC_FLAGS) -o $@ $<

$(RELEASE_OBJECT_DIR)/NLSLmatrix.o	:	NLSLmatrix.c
	mkdir -p $(RELEASE_OBJECT_DIR)
	$(CC) $(RELEASE_CC_FLAGS) -o $@ $<

$(RELEASE_OBJECT_DIR)/NLSLgnsolver.o	:	NLSLgnsolver.c NLSLgnsolver.h NLSLsolver.h
	mkdir -p $(RELEASE_OBJECT_DIR)
	$(CC) $(RELEASE_CC_FLAGS) -o $@ $<

clean_release	:
	rm -f $(RELEASE_OBJECTS)
	rm -f $(RELEASE_TARGET)

clean_all	:	clean_debug clean_release