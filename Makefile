all:
	gcc -o bwtsearch bwtsearch.c index_ops.c bwt_ops.c freq_table.c
