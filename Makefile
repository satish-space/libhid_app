APP = test-mtk-hid

# list all cpp sources
SRC_CPP +=	*.c

# Add personal CPP flags. To remove one just comment the line
# Set all warning to show
CFLAGS += -Wall
# compile only
CFLAGS += -c

# Additional settings
# to build debug version type: make debug=on
ifeq ($(debug), on)
CFLAGS += -DDEBUG -g3 -O0
endif

# Rules to build all c and cpp code.
OBJScpp = $(SRC_CPP:.cpp=.o)

# Example how to add library and library path if needed
# add individual libraries, one at the time
LD_LIBS += -lhidapi-libusb

all: build

build: $(SRC_CPP) $(APP)

$(APP): $(OBJScpp)
	$(CXX) $(LDFLAGS) $(OBJScpp) $(LIBS_PATH) $(LD_LIBS) -o $@

.cpp.o:
	$(CXX) $(INC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(APP)
