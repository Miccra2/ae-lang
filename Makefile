CC := cc
CCFLAGS := -Wall -Wextra -ggdb
TARGET := aec
SRCDIR := src
OBJDIR := build
SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
INCLUDES := -I$(SRCDIR)/include

$(info SOURCES=$(SOURCES))
$(info OBJECTS=$(OBJECTS))

all: $(TARGET)

clean:
	rm -rf $(TARGET) $(OBJDIR)

run: $(TARGET)
	./$(TARGET)

$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET): $(OBJECTS) | $(OBJDIR)
	$(CC) $(CCFLAGS) $^ -o $@
