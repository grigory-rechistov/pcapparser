
.PHONY: test
test: parser-test

parser-test: parse.cpp mock.cpp test.cpp
	${CXX} $^ -o $@
	./$@
