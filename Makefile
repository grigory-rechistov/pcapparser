
.PHONY: test
test: parser-test

# TODO set proper dependencies from headers
parser-test: parse.cpp test.cpp
	${CXX} $^ -o $@
	./$@

clean:
	rm -f parser-test
