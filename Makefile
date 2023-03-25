
.PHONY: test ref-test
.PRECIOUS: parser-test
test: parser-test pcapparser ref-test simbadecoder

# TODO set proper dependencies from headers
parser-test: parsedheader.cpp packet.cpp order.cpp test.cpp 
	${CXX} -g $^ -o $@
	./$@

pcapparser: parsedheader.cpp packet.cpp main.cpp
	${CXX} -g $^ -o $@

clean:
	rm -f parser-test pcapparser

# Test a small sample input to generate expected output 
ref-test: pcapparser
	./pcapparser < ref/first_ten.pcap | hexdump -C | diff - ref/output.hex

simbadecoder: parsedheader.cpp packet.cpp order.cpp simbadecoder.cpp
	${CXX} -g $^ -o $@
