
CC = ~/gcc-4.9.2/gcc-4.9.2/bin/g++
CFLAGS = -Wall -Wextra -Werror -Wpedantic -ggdb -Wabi -Wctor-dtor-privacy     \
	 -Wnoexcept -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual  \
	 -Wformat=2 -Wmissing-include-dirs -Wswitch-default -Wswitch-enum     \
	 -Wuninitialized -Wsuggest-attribute=const                            \
	 -Wsuggest-attribute=noreturn -Wfloat-equal -Wundef -Wshadow 	      \
	 -Wcast-qual -Wconversion -Wzero-as-null-pointer-constant             \
	 -Wuseless-cast -Wlogical-op -Wno-aggressive-loop-optimizations       \
	 -Wmissing-declarations -Wnormalized=nfc -Wpacked -Wredundant-decls   \
	 -Wsign-conversion -std=c++11
ODIR = obj
LDIR = /home/eric/gcc-4.9.2/gcc-4.9.2/lib64
LIBS =
LFLAGS = -Wl,-rpath=$(LDIR)
DEPS = polynomial.h
TARGET = test_suite
TARGET_LIB = libpoly.a
AR = ar
AR_FLAGS = -rvc

_OBJ = test_suite.o polynomial.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)
	ctags -R

$(TARGET_LIB): CFLAGS += -static-libstdc++
$(TARGET_LIB): $(ODIR)/polynomial.o
	$(AR) $(AR_FLAGS) $@ $^

.PHONY: clean
	
clean:
	rm -f $(ODIR)/*.o core $(TARGET) tags $(TARGET_LIB)

