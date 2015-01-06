#!/bin/bash
rm hn
if gcc hn.c -o hn; then
	echo "Should sucseed"
	./hn google.com 80 5 && echo Returns true || echo Returns false
	echo "Should timeout"
	./hn google.com 81 5 && echo Returns true || echo Returns false
	echo "Should be refused"
	./hn solprime.net 81 5 && echo Returns true || echo Returns false
	echo "Should not be able to resolve"
	./hn m5NQzdYH5qFhtC9bzb5C5fU7 80 5 && echo Returns true || echo Returns false
else
	echo "Unable to compile"
fi

