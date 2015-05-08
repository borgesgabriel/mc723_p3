# ####################################################
# Hello World Platform
# ####################################################

# Variable that points to SystemC installation path
export SYSTEMC :=  /home/staff/rodolfo/mc723/systemc

# Variable that points to TLM installation path
export TLM_PATH := /home/staff/rodolfo/mc723/systemc/include/tlm

# Variable that points to ArchC installation path
export ARCHC_PATH = /home/staff/rodolfo/mc723/archc

export PATH:=/l/archc/compilers/bin:$(PATH)

TARGET=helloworld_mips.01
EXE = $(TARGET).x

SRCS := main.cpp
OBJS := $(SRCS:.cpp=.o)

#------------------------------------------------------
.SILENT:
#------------------------------------------------------
.SUFFIXES: .cc .cpp .o .x
#------------------------------------------------------
$(EXE): $(OBJS) $(LIBFILES)
	$(CC) $(CFLAGS) $(INC_DIR) $(LIB_DIR) -o $(EXE) $(OBJS) $(LIBS)
#------------------------------------------------------
main.o:
	$(CC) $(CFLAGS) $(INC_DIR) -c main.cpp
#------------------------------------------------------
all: $(EXE)
#------------------------------------------------------
run: all
	./$(EXE) --load=hello_world.x
#------------------------------------------------------
clean:
	rm -f $(OBJS) $(EXE) *~ *.o
#------------------------------------------------------
distclean: clean
#------------------------------------------------------
.cpp.o:
	$(CC) $(CFLAGS) $(INC_DIR) -c $<
#------------------------------------------------------
.cc.o:
	$(CC) $(CFLAGS) $(INC_DIR) -c $<





