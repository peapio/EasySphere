PROJECT_NAME = crms
PROJECT_DIR := $(shell pwd)

ifeq ($(DEBUG), 1)
	DFLAGS += -g
endif

CRMS_NODE_TARGET = crms_node
CRMS_SERVER_TARGET = crms_server
