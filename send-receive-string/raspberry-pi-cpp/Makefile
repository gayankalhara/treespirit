ifeq ($(wildcard Makefile.inc), )
    $(error Configuration not found. Run ./configure first)
endif

include Makefile.inc

# define all programs
PROGRAMS = audio

BINARY_PREFIX = rf24
SOURCES = $(PROGRAMS:=.cpp)

LIBS=-l$(LIB)
ifeq ($(DRIVER), LittleWire)
	LIBS+= -llittlewire-spi
endif

all: $(PROGRAMS)

$(PROGRAMS): $(SOURCES)
	$(CXX) $(CFLAGS) -I$(HEADER_DIR)/.. -I.. -L$(LIB_DIR) $@.cpp $(LIBS) -o $@

clean:
	@echo "[Cleaning]"
	rm -rf $(PROGRAMS)

install: all
	@echo "[Installing examples to $(EXAMPLES_DIR)]"
	@mkdir -p $(EXAMPLES_DIR)
	@for prog in $(PROGRAMS); do \
		install -m 0755 $${prog} $(EXAMPLES_DIR)/$(BINARY_PREFIX)-$${prog}; \
	done

upload: all
	@echo "[Uploading examples to $(REMOTE):$(REMOTE_EXAMPLES_DIR)]"
ifeq ($(REMOTE),)
	@echo "[ERROR] Remote machine not configured. Run configure with respective arguments."
	@exit 1
endif
	@ssh -q -t -p $(REMOTE_PORT) $(REMOTE) "mkdir -p $(REMOTE_EXAMPLES_DIR)"
	@ssh -q -t -p $(REMOTE_PORT) $(REMOTE) "mkdir -p /tmp/RF24_examples"
	@scp -q -P $(REMOTE_PORT) $(PROGRAMS) $(REMOTE):/tmp/RF24_examples
	@for prog in $(PROGRAMS); do \
		ssh -q -t -p $(REMOTE_PORT) $(REMOTE) "sudo install -m 0755 /tmp/RF24_examples/$${prog} $(REMOTE_EXAMPLES_DIR)/$(BINARY_PREFIX)-$${prog}"; \
	done
	@ssh -q -t -p $(REMOTE_PORT) $(REMOTE) "rm -rf /tmp/RF24_examples"

.PHONY: install upload
