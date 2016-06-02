CC = gcc
CFLAGS = -Wall -g
CFLAGS1 = -I ../Types/ \
	  -I $(LIB_PATH)
OBJDIR = OBJS
LINK_FLAGS = 
EXEC = test
LIB_PATH = $(HOME)/Mehul_Geek/programs/Sys_Timer/Lib
LIB_FILE_PATH = $(LIB_PATH)/LIBS/libTimer.a
SRC_FILES = $(wildcard *.c)
OBJ_FILES = $(patsubst %.c, %.o, $(SRC_FILES))
OBJS = $(addprefix $(OBJDIR)/, $(OBJ_FILES))

$(OBJS): | create

all: $(OBJS)
	$(MAKE) -C $(LIB_PATH) all
	@echo "\033[33m"
	@echo "==============================="
	@echo "Building Source files"
	@echo "==============================="
	@echo "\033[0m"
	$(CC) -o $(EXEC) $(LINK_FLAGS) $(OBJS) $(LIB_FILE_PATH)

.PHONY: create
create: 	
	@mkdir -p $(OBJDIR)


.PHONY: clean
clean:
	$(MAKE) -C $(LIB_PATH) clean
	@echo "\033[31m"
	@echo "==============================="
	@echo "Removing all files"
	@echo "==============================="
	@echo "\033[0m"
	@rm -rf $(OBJDIR)
	@rm -f *.o $(EXEC)

$(OBJDIR)/%.o : %.c
	$(CC) -c $(CFLAGS) $(CFLAGS1) $< -o $@
