#!/bin/bash
test -e hn && rm hn
if gcc -O3 hn.c -o hn; then
	echo -e "\nShould sucseed and return true"
	./hn google.com 80 5 && echo Returns true || echo Returns false

	echo -e "\nShould timeout and return false"
	./hn google.com 81 5 && echo Returns true || echo Returns false

	echo -e "\nShould be refused and return false"
	./hn solprime.net 81 5 && echo Returns true || echo Returns false

	echo -e "\nShould not be able to resolve and return false"
	./hn m5NQzdYH5qFhtC9bzb5C5fU7 80 5 && echo Returns true || echo Returns false
else
	echo "Unable to compile"
fi

