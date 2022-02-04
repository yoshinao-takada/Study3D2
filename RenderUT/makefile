CC = clang
INCFLAGS = -I.. -I/usr/include/opencv4
DEBUG_CC_FLAGS = -c -g -D_DEBUG $(INCFLAGS) -std=c17
RELEASE_CC_FLAGS =  -c -O3 -DNDEBUG $(INCFLAGS) -std=c17
# OPENCV_LIB = `pkg-config --libs opencv`
OPENCV_LIBS = -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core
DEBUG_LIBS =  -L$(DEBUG_TARGET_DIR) -lRender -lPspc -lZnccC -lNLSL -lUtil $(OPENCV_LIBS) -lstdc++ -lm
DEBUG_TARGET_DIR = ../debug
DEBUG_TARGET = $(DEBUG_TARGET_DIR)/RenderUT.exe
DEBUG_OBJECT_DIR = $(DEBUG_TARGET_DIR)/o
RELEASE_TARGET_DIR = ../release
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/RenderUT.exe
RELEASE_OBJECT_DIR = $(RELEASE_TARGET_DIR)/o
DEBUG_OBJECTS = $(DEBUG_OBJECT_DIR)/RenderUT.o $(DEBUG_OBJECT_DIR)/meshUT.o $(DEBUG_OBJECT_DIR)/textureUT.o \
	$(DEBUG_OBJECT_DIR)/projectionUT.o $(DEBUG_OBJECT_DIR)/projection2UT.o
RELEASE_OBJECTS = $(RELEASE_OBJECT_DIR)/RenderUT.o $(RELEASE_OBJECT_DIR)/meshUT.o $(RELEASE_OBJECT_DIR)/textureUT.o \
	$(RELEASE_OBJECT_DIR)/projectionUT.o $(RELEASE_OBJECT_DIR)/projection2UT.o

.PHONY	:	debug
debug	:	$(DEBUG_TARGET)

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p ../Work
	$(CC) -o $@ $^ $(DEBUG_LIBS)

$(DEBUG_OBJECT_DIR)/RenderUT.o	:	RenderUT.c
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/meshUT.o	:	meshUT.c
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/textureUT.o	:	textureUT.c
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/projectionUT.o	:	projectionUT.c
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/projection2UT.o	:	projection2UT.c
	mkdir -p $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

clean_debug	:
	rm -f $(DEBUG_TARGET)
	rm -f $(DEBUG_OBJECTS)
