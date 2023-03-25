
.PHONY: test
test: parser-test pcapparser

# TODO set proper dependencies from headers
parser-test: parse.cpp test.cpp header.cpp packet.cpp
	${CXX} -g $^ -o $@
	./$@

pcapparser: parse.cpp header.cpp packet.cpp main.cpp
	${CXX} -g $^ -o $@

clean:
	rm -f parser-test
