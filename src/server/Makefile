include ../../config.mk

all: $(CRMS_SERVER_TARGET)

$(CRMS_SERVER_TARGET): main.cpp
	g++ $(DFLAGS) -o $(CRMS_SERVER_TARGET) main.cpp

clean:
	rm -f $(CRMS_SERVER_TARGET)

.PHONY: all clean