CC = clang
INCFLAGS = -I..
DEBUG_CC_FLAGS = -c -g -D_DEBUG $(INCFLAGS) -std=c17
RELEASE_CC_FLAGS =  -c -O3 -DNDEBUG $(INCFLAGS) -std=c17
DEBUG_TARGET_DIR = ../debug
DEBUG_TARGET = $(DEBUG_TARGET_DIR)/PspcUT.exe
DEBUG_OBJECT_DIR = $(DEBUG_TARGET_DIR)/o
RELEASE_TARGET_DIR = ../release
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/PspcUT.exe
RELEASE_OBJECT_DIR = $(RELEASE_TARGET_DIR)/o
DEBUG_OBJECTS = $(DEBUG_OBJECT_DIR)/PvectorUT.o $(DEBUG_OBJECT_DIR)/PmatrixUT.o $(DEBUG_OBJECT_DIR)/PspcUT.o \
	$(DEBUG_OBJECT_DIR)/solveUT.o $(DEBUG_OBJECT_DIR)/PcameraUT.o $(DEBUG_OBJECT_DIR)/homographyUT.o \
	$(DEBUG_OBJECT_DIR)/Pcamera0UT.o $(DEBUG_OBJECT_DIR)/gnsolverUT.o
RELEASE_OBJECTS = $(RELEASE_OBJECT_DIR)/Pvector.o $(RELEASE_OBJECT_DIR)/Pmatrix.o$ $(RELEASE_OBJECT_DIR)/PspcUT.o \
	$(RELEASE_OBJECT_DIR)/solveUT.o $(RELEASE_OBJECT_DIR)/PcameraUT.o $(RELEASE_OBJECT_DIR)/homographyUT.o \
	$(RELEASE_OBJECT_DIR)/Pcamera0UT.o $(RELEASE_OBJECT_DIR)/gnsolverUT.o

.PHONY	:	debug
debug	:	$(DEBUG_TARGET)

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	$(CC) -o $@ $^ -L$(DEBUG_TARGET_DIR) -lPspc -lNLSL -lm

$(DEBUG_OBJECT_DIR)/PspcUT.o	:	PspcUT.c
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/PvectorUT.o	:	PvectorUT.c
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/PmatrixUT.o	:	PmatrixUT.c
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/solveUT.o	:	solveUT.c
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/PcameraUT.o	:	PcameraUT.c
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/homographyUT.o	:	homographyUT.c
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/Pcamera2UT.o	:	Pcamera2UT.c
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/Pcamera0UT.o	:	Pcamera0UT.c
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/gnsolverUT.o	:	gnsolverUT.c
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

clean_debug	:
	rm -f $(DEBUG_TARGET)
	rm -f $(DEBUG_OBJECTS)
	