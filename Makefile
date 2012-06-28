all:
	gcc -DHASH_FUNCTION=HASH_SFH -o dnsqlog -lpcap -W -Wall -O2 *.c
