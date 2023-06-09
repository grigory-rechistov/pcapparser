
.PHONY: test ref-test
.PRECIOUS: parser-test
test: parser-test pcapparser ref-test simbadecoder

CFLAGS = -Wall -Werror -g

# TODO set proper dependencies from headers
parser-test: parsedheader.cpp packet.cpp order.cpp io.cpp test.cpp 
	${CXX} ${CFLAGS} $^ -o $@
	./$@

pcapparser: parsedheader.cpp packet.cpp  io.cpp pcapparser.cpp
	${CXX} ${CFLAGS} $^ -o $@

clean:
	rm -f parser-test pcapparser simbadecoder

# Test a small sample input to generate expected output 
ref-test: pcapparser simbadecoder
	./pcapparser < ref/first_ten.pcap | hexdump -C | diff - ref/output.hex
	./simbadecoder < ref/first_ten.pcap | diff - ref/obs-txes.txt

simbadecoder: parsedheader.cpp packet.cpp order.cpp io.cpp simbadecoder.cpp
	${CXX} ${CFLAGS} $^ -o $@
