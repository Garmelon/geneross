CC = g++ -std=c++17
# CFLAGS = -g -O0 -Wall --pedantic
CFLAGS = -O2 -Wall --pedantic
LFLAGS = -lsfml-system -lsfml-network -lsfml-window -lsfml-graphics

RM = rm -f

SOURCEFILES = cpp
SRCDIR = src
TMPDIR = build
TARGET = gross
#FILES = Chromosome Fitness Generation main
FILES = Genes Chromosome

#SOURCES = $(patsubst %,$(SRCDIR)/%.cpp,$(FILES))
OBJECTS = $(patsubst %,$(TMPDIR)/%.o,$(FILES))
DEPS    = $(patsubst %,$(TMPDIR)/%.d,$(FILES))

.PHONY: clean run remake

all: $(TARGET)

# run the target
run: $(TARGET)
	./$<

# clean up the build directory
clean:
	$(RM) $(TARGET)
	$(RM) $(OBJECTS)
	$(RM) $(DEPS)

# clean up and make again
remake: clean $(TARGET)

# link the final target
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) -o $@

# compiling to .o file
$(TMPDIR)/%.o: $(SRCDIR)/%.$(SOURCEFILES)
	$(CC) $(CFLAGS) -c $< -o $@

# generating .d dependency files
$(TMPDIR)/%.d: $(SRCDIR)/%.$(SOURCEFILES)
	$(CC) $(CFLAGS) -MM  $< -MT $(TMPDIR)/$*.o -MF $(TMPDIR)/$*.d

# using dependency files
-include $(DEPS)
