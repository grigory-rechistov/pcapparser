
test: parser-test

parser-test: parse.cpp mock.cpp
	${CXX} $^ -o $@
	./$@
