include ../../config.mk

all: $(CRMS_NODE_TARGET)

$(CRMS_NODE_TARGET): main.cpp
	g++ $(DFLAGS) -o $(CRMS_NODE_TARGET) main.cpp

clean:
	rm -f $(CRMS_NODE_TARGET)

.PHONY: all clean