CC = gcc

OUTDIR = build
SRCDIR = src
INCDIR = include
TSTDIR = test
EXDIR = examples
EXAMPLES = $(OUTDIR)/lemonade
TESTS = $(OUTDIR)/geo
TARGET = $(OUTDIR)/libmegiddolp.a
OBJS = $(OUTDIR)/clp_wrapper.o \
       $(OUTDIR)/geo.o
HDRS = $(INCDIR)/clp_wrapper.h \
       $(INCDIR)/geo.h

CPPFLAGS = -I$(INCDIR)
CFLAGS = -std=c89 -Wall -Werror -pedantic
ARFLAGS = rcvs

all: $(TARGET)
.PHONY: all

$(TARGET): $(OBJS)
	@mkdir -p $(OUTDIR)
	$(AR) $(ARFLAGS) $@ $^

$(OBJS): $(OUTDIR)/%.o: $(SRCDIR)/%.c $(HDRS)
	@mkdir -p $(OUTDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

examples: $(EXAMPLES)
.PHONY: examples

test: $(TESTS)
.PHONY: test

$(EXAMPLES): $(TARGET)
$(EXAMPLES): $(OUTDIR)/%: $(EXDIR)/%.c
	@mkdir -p $(OUTDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(TARGET) -o $@ -lm

$(TESTS): $(TARGET)
$(TESTS): $(OUTDIR)/%: $(TSTDIR)/%.c
	@mkdir -p $(OUTDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(TARGET) -o $@ -lm
	$@

clean:
	rm -rf $(OUTDIR)
.PHONY: clean
