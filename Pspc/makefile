CC = clang
INCFLAGS = -I..
DEBUG_CC_FLAGS = -c -g -D_DEBUG $(INCFLAGS) -std=c17
RELEASE_CC_FLAGS =  -c -O3 -DNDEBUG $(INCFLAGS) -std=c17
DEBUG_TARGET_DIR = ../debug
DEBUG_TARGET = $(DEBUG_TARGET_DIR)/libPspc.a
DEBUG_OBJECT_DIR = $(DEBUG_TARGET_DIR)/o
RELEASE_TARGET_DIR = ../release
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/libPspc.a
RELEASE_OBJECT_DIR = $(RELEASE_TARGET_DIR)/o
DEBUG_OBJECTS = $(DEBUG_OBJECT_DIR)/Pvector.o $(DEBUG_OBJECT_DIR)/Pmatrix.o $(DEBUG_OBJECT_DIR)/Pcamera.o
RELEASE_OBJECTS = $(RELEASE_OBJECT_DIR)/Pvector.o $(RELEASE_OBJECT_DIR)/Pmatrix.o $(RELEASE_OBJECT_DIR)/Pcamera.o

.PHONY	:	debug
debug	:	$(DEBUG_TARGET)

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	ar r $@ $^
	ranlib $@

$(DEBUG_OBJECT_DIR)/Pvector.o	:	Pvector.c Pvector.h
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/Pmatrix.o	:	Pmatrix.c Pmatrix.h
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/Pcamera.o	:	Pcamera.c Pcamera.h
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

clean_debug	:
	rm -f $(DEBUG_TARGET)
	rm -f $(DEBUG_OBJECTS)
	