#include <iostream>
#include <stdio.h>
#include <locale.h>
#define N 6
/*
39. В массиве слов (байтов) выбранного размера (5-6 элементов) над каждым элементом выполнить операцию:
если биты 6:5 являются результатом операции дизъюнкции битового поля 4:3 и образца, то инвертировать
старший и младший биты. Вывод на экран исходного массива и массива результатов производить в
двоичном и шестнадцатеричном представлениях.

*/
void printBits(size_t const size, void const* const ptr)
{
	unsigned char* b = (unsigned char*)ptr;
	unsigned char byte;
	int i, j;

	for (i = size - 1; i >= 0; i--) {
		for (j = 7; j >= 0; j--) {
			byte = (b[i] >> j) & 1;
			printf("%u", byte);
		}
	}
	printf(" | ");
}

//binary to decimal
int convertbtod(long long n) {
	int dec = 0, i = 0, rem;

	while (n != 0) {
		rem = n % 10;
		n /= 10;
		dec += rem * pow(2, i);
		++i;
	}

	return dec;
}


int main() {
	setlocale(LC_ALL, "rus");

	short int i = N; // i - для циклов
	int t = 1; // Режим ввода: 0=байтовый, не ноль = слова
	char OBR = 6; // (110)


	short int A[6] = { 55, 96, 77, 3, -121232, 96 };
	//short int A[6] = { 122, -96, 772121, 33, -121232, 96 };
	//short int A[6] = { 51215, -91216, 73337, 1213, -121232, -96 };
	//short int A[6] = { convertbtod(1110111), convertbtod(1100000), convertbtod(01001101), convertbtod(11), convertbtod(10011001110000), convertbtod(1100000) }; // двоичный ввод
	//short int A[6] = { 0xfff5, 0x1216, 0x73337, 0x1213, 0x1212, 0x96 };
	// 
	//char A[6] = { 55, 96, 77, 3, -32, 96 };
	//char A[6] = { 122, -96, 772121, 33, -121232, 96 };
	//char A[6] = { 51215, -91216, 73337, 1213, -121232, -96 };
	//char A[6] = { convertbtod(110111), convertbtod(1101000), convertbtod(01001101), convertbtod(11), convertbtod(1110000), convertbtod(1100000) }; // двоичный ввод
	// char A[6] = { 0xfff5, 0x91216, 0x73337, 0x1213, 0x1212, 0x96 };
	// Вывод старого массива
	printf("\n\nВведённый массив А в 10й = [ | ");
	for (i = 0; i < N; i += 1)
		printf("%d | ", A[i]);
	printf(" ]\n");

	printf("Введённый массив А в 16й = [ | ");
	for (i = 0; i < N; i += 1)
		printf("%x | ", A[i]);
	printf(" ]\n");

	printf("Введённый массив А в 2й = [ | ");
	for (i = 0; i < N; i += 1)
		printBits(sizeof(A[i]), &A[i]);
	printf("]\n");

	

	__asm {
		; // начальные установки регистров
		lea esi, A; // адрес начала массива А в si
		mov cx, i; // задать число итераций
		mov dl, OBR; // занести байт образца 
		mov dh, 0; // в dx
		cmp t, 0; // выбор обработки слова или байта
		ja STARTW; // если t != 0, то переход на обработку слова

	STARTB:  mov al, [esi]; // обработка байта, текущий элемент массива А в al
		mov bl, al; // дублировать al в bl для выделения второго поля
		and al, 01100000b; // выделение битового поля 6:5 (al изменен)
		shr al, 4; // сдвиг вправо на 4
		and bl, 00011000b; // выделение битового поля 4:3 (bl изменен)
		shr bl, 2; // сдвиг вправо на 2
		or bl, OBR; // дизъюнкция битового поля 4:3 и образца

		cmp al, bl; // сравнить al и bl
		jnz NEXTB; // если поля не совпадают, то далее

		xor [esi], 10000001b; // инвертировать старший и младший биты

	NEXTB: inc esi; // Байтовый массив
		dec cx; // счетчик цикла -1
		cmp cx, 0; // проверка наличия цикла
		ja STARTB; // запустисть цикл заново
		jna END; // конец

	STARTW:  mov ax, [esi]; // обработка байта, текущий элемент массива А в ax
		mov bx, ax; // дублировать al в bl для выделения второго поля
		and ax, 0000000001100000b; // выделение битового поля 6:5 (ax изменен)
		shr ax, 4; // сдвиг вправо на 4
		and bx, 0000000000011000b; // выделение битового поля 4:3 (bx изменен)
		shr bx, 2; // сдвиг вправо на 2
		or bx, dx; // дизъюнкция битового поля 4:3 и образца

		cmp ax, bx; // сравнить ax и bx
		jnz NEXTW; // если поля не совпадают, то далее

		mov ax, [esi]; // получить значение
		xor ax, 1000000000000001b; // инвертировать
		mov [esi], ax; // заменить значение

	NEXTW: add esi, 2; // short int массив
		dec cx; // счетчик цикла -1
		cmp cx, 0; // проверка наличия цикла
		ja STARTW; // запустисть цикл заново
	END:nop
	}


	// Вывод нового массива
	printf("\n\n    Новый массив А в 10й = [ | ");
	for (i = 0; i < N; i += 1)
		printf("%d | ", A[i]);
	printf(" ]\n");
	printf("    Новый массив А в 16й = [ | ");
	for (i = 0; i < N; i += 1)
		printf("%x | ", A[i]);
	printf(" ]\n");
	printf("    Новый массив А в 2й = [ | ");
	for (i = 0; i < N; i += 1)
		printBits(sizeof(A[i]), &A[i]);
	printf("]\n");

	printf("\nПрограмма завершена\n");
	return 0;
}