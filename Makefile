# Variable that points to SystemC installation path
export SYSTEMC:=/home/staff/rodolfo/mc723/systemc
export LD_LIBRARY_PATH:=/home/staff/rodolfo/mc723/systemc/lib-linux64:$(LD_LIBRARY_PATH)

# Variable that points to TLM installation path
export TLM_PATH:=/home/staff/rodolfo/mc723/systemc/include

# Variable that points to ArchC installation path
export ARCHC_PATH:=/home/staff/rodolfo/mc723/archc

export PATH:=/home/staff/rodolfo/compilers/bin:$(PATH)

TARGET=platform
EXE = $(TARGET).x

SRCS := main.cpp
OBJS := $(SRCS:.cpp=.o)
COMPONENTS := mips memory cores_controller arctan bus
HOST_OS := linux64

export LIB_DIR:=-L $(SYSTEMC)/lib-$(HOST_OS) \
		-L $(ARCHC_PATH)/lib \
		$(foreach c, $(COMPONENTS), -L $(c))

export INC_DIR:=-I $(SYSTEMC)/include \
		-I $(ARCHC_PATH)/include/archc \
		-I $(TLM_PATH) \
		$(foreach c, $(COMPONENTS), -I $(c))

export LIBS:= $(foreach c, $(COMPONENTS), -l$(c)) -lsystemc -larchc -lm

export LIBFILES:= $(foreach c, $(COMPONENTS), $(c)/lib$(c).a)

export CFLAGS:=-g

export CC:=g++ --std=c++11


# Number of intervals. The higher the number, the more precise the result is
I := 100
# Number of threads. The higher the number, the faster the computation is
T := 1

all:
	for c in $(COMPONENTS); do echo " => Making" $$c ...; \
	    cd $$c; $(MAKE); cd ..; done
	echo " => Making sw ..."
	cd sw/hello_world; $(MAKE)
	cd sw/pi; $(MAKE)
	echo " => Making platform ..."
	$(MAKE) $(EXE)

clean:
	for c in $(COMPONENTS); do echo " => Making" $$c ...; \
	    cd $$c; $(MAKE) clean; cd ..; done
	echo " => Making sw ..."
	cd sw/hello_world; $(MAKE) clean
	cd sw/pi; $(MAKE) clean
	echo " => Making platform ..."
	rm -f $(OBJS) $(EXE) *~ *.o
	rm -f core*.*

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
#all: $(EXE)
#------------------------------------------------------
# How to specify I and T parameters:
# make pi I=2 T=4
pi: clean all $(EXE)
	./$(EXE) --load=sw/pi/pi.mips $(I) $(T)
#------------------------------------------------------
hw: clean all $(EXE)
	./$(EXE) --load=sw/hello_world/hello_world.mips
#------------------------------------------------------
debug: $(EXE)
	gdb $(EXE)
#------------------------------------------------------
distclean: clean
#------------------------------------------------------
.cpp.o:
	$(CC) $(CFLAGS) $(INC_DIR) -c $<
#------------------------------------------------------
.cc.o:
	$(CC) $(CFLAGS) $(INC_DIR) -c $<





