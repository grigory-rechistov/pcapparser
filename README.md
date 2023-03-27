
## Building

Use `make` to compile the binaries and run tests. It was tested to work
on Ubuntu 22.04 x86_64 with GCC 11.3.0.
Use `make clean` to remove the binaries.

## Using binaries

- `simbadecoder` - takes a PCAP file as stdin and dumps contents of found
   OrderBookSnapshot packets.

- `pcapparser` - takes a PCAP file as stdin, strips PCAP headers and outputs
  raw packet data to stdout. It can be compiled to strip Ethernet/IPv4/UDP
  headers and/or dump the contents as hexadecimal strings.

- `parser-test` - unit tests, are run automatilaly by `make test`.
