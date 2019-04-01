TARGET := t1
OBJS := t1.o

include ../common.mk

test: $(TARGET) t1.sh
	./t1.sh && echo "test succeeded"

.PHONY: test
