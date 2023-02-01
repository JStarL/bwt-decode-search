# bwt-decode-search
An implementation of the Burrows-Wheeler Transform decoding and searching algorithms

Given a bwt endoded document in a particular format (described below), returns the ouput (also in a particular format)

Example Original File / Decoded Output:

1 A very very long line

Example BWT Endode File:

1gyyAe nvvnl  iolee  rr

In the original file, lines start with line numbers and padded zeroes, and then a single whitespace.
