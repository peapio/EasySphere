include config.mk

all: prepare release

debug:
	$(MAKE) DEBUG=1

prepare:
	mkdir -p $(PROJECT_DIR)/bin
	mkdir -p $(PROJECT_DIR)/lib
	mkdir -p $(PROJECT_DIR)/log


release:
	for target in $(CRMS_NODE_TARGET) $(CRMS_SERVER_TARGET); do \
		$(MAKE) -C src/$$target; \
	done

clean:
	for target in $(CRMS_NODE_TARGET) $(CRMS_SERVER_TARGET); do \
		$(MAKE) -C src/$$target clean; \
	done

.PHONY: debug release clean
