
.PHONY: test ref-test
.PRECIOUS: parser-test
test: parser-test pcapparser ref-test

# TODO set proper dependencies from headers
parser-test: parse.cpp test.cpp packet.cpp
	${CXX} -g $^ -o $@
	./$@

pcapparser: parse.cpp packet.cpp main.cpp
	${CXX} -g $^ -o $@

clean:
	rm -f parser-test pcapparser

# Test a small sample input to generate expected output 
ref-test: pcapparser
	./pcapparser < ref/first_ten.pcap | hexdump -C | diff - ref/output.hex
