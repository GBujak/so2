#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>


void main() {
	
	int file_desc, wyraz;

	printf("Podaj, ktory wyraz ciagu fibonacciego odczytac ze sterownika: ");
	scanf(" %d", &wyraz);	

	file_desc = open("/dev/fibdev", O_RDONLY | O_APPEND);

	if (file_desc < 0) {
		printf("Nie moge otworzyc pliku urzadzenia: /dev/fibdev\n");
		exit(-1);
	}
	if (--wyraz >= 0) {
		unsigned long long val = lseek(file_desc, wyraz, 0);

		if (val == -1)
			printf("Blad");
		else
			printf("Odczytany wyraz przez lseek(): %llu\n", val);
	} else
		printf("Blad\n");
	
	close(file_desc);
	
}
