CC=g++
CFLAGS += -I. -Ilib  -O3 -std=c++17 -DNDEBUG -Wall -Wextra
LDFLAGS = 

all: test apps

%.o: %.cpp %.hpp
	${CC} -c $< -o $@ $(CFLAGS)

# 不能去掉，原因不明
%.o: %.cpp
	${CC} -c $< -o $@ $(CFLAGS)

# ---------------------------  apps  --------------------------------
apps: apps/SSPPR

apps/SSPPR: apps/SSPPR.o
	${CC} ${CFLAGS} $^ -o $@ $(LDFLAGS)


# ---------------------------  test  --------------------------------
test: test/uwudgraph/test_io test/uwudgraph/test_ssppr

test/uwudgraph/test_io: test/uwudgraph/test_io.cpp
	${CC} ${CFLAGS} $^ -o $@ $(LDFLAGS)

test/uwudgraph/test_ssppr: test/uwudgraph/test_ssppr.cpp
	${CC} ${CFLAGS} $^ -o $@ $(LDFLAGS)

run_test:
	# ./test/uwudgraph/test_io
	./test/uwudgraph/test_ssppr
	@echo "Uwudgraph Test successfully."


clean:
	rm -f *.o
	rm -f test/uwudgraph/*.o
	rm -f apps/*.o
	rm -f test/uwudgraph/test_io
	rm -f test/uwudgraph/test_ssppr
	rm -f apps/SSPPR


.PHONY: clean