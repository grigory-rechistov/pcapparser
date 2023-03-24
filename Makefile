
.PHONY: test
test: parser-test

# TODO set proper dependencies from headers
parser-test: parse.cpp test.cpp header.cpp packet.cpp
	${CXX} -g $^ -o $@
	./$@

clean:
	rm -f parser-test
