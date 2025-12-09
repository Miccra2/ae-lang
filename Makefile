CC		:= cc
CCFLAGS := -Wall -Wextra -ggdb
TARGET	:= aec
SRCDIR	:= src
OBJDIR	:= .build
SRCS	:= $(wildcard $(SRCDIR)/*.c)
OBJS	:= $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(TARGET) clean

$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CCFLAGS) -c $^ -o $@

$(OBJDIR):
	mkdir -p $@

clean:
	rm -fr $(OBJDIR)
