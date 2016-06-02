#==============================================================
#======================== COMPILER ============================
#==============================================================
CC = gcc
#CFLAGS = -Wall -ggdb3
CFLAGS = -Wall 
CFLAGS1 = -I .
OBJDIR = OBJS
LINK_FLAGS = 

#==============================================================
#======================== INPUT FILES =========================
#==============================================================

EXEC = test
SRC_FILES = $(wildcard *.c)
OBJ_FILES = $(patsubst %.c, %.o, $(SRC_FILES))
OBJS = $(addprefix $(OBJDIR)/, $(OBJ_FILES))

CFILES   = $(shell find . -type f -name '*.c')
CXXFILES = $(shell find . -type f -name '*.cpp')
OBJECTS  = $(SRC_FILES:.c=.o) \
           $(CXXFILES:.cpp=.o)

#==============================================================
#======================== RULES ===============================
#==============================================================

$(OBJS): | create

all: $(EXEC)

${EXEC}: $(OBJS)
	@echo "\033[33m"
	@echo "==============================="
	@echo "Building Source files"
	@echo "==============================="
	@echo "\033[0m"
	$(CC) -o $(EXEC) $(LINK_FLAGS) $^

.PHONY: create
create: 	
	@mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	@echo "\033[31m"
	@echo "==============================="
	@echo "Removing all files"
	@echo "==============================="
	@echo "\033[0m"
	@rm -rf $(OBJDIR)
	@rm -f *.o $(EXEC)

$(OBJDIR)/%.o : %.c
	$(CC) -c $(CFLAGS) $(CFLAGS1) $< -o $@
