#include <stdio.h>
#include <locale.h>
#include <ctype.h>

/*
Вариант 39
Дана строка из трех десятичных цифр. Если вторая и третья совпадают и превышают первую, то заменить
вторую и третью цифры дополнением до 9, иначе — увеличить первую цифру на 1 ('9' сохраняется).
*/

// символы есть цифры
int input_is_digit(const char* input) {
	while (*input) {
		if (isdigit(*input++) == 0) return 0;
	}
	return 1;
}

// очистка stdin и количество очищенных символов
int clean_stdin() {
	int c,counter=-1;
	do {
		c = getchar();
		counter+=1;
	} while (c != '\n' && c != EOF);
	return counter;
}


int main() {
	setlocale(LC_ALL, "rus");
	char input[]="qqq";
	int temp = 0;

	// Ввод и проверка
	while (!input_is_digit(input) or temp != 0) {
		printf("Введите строку из трёх десятичных чисел: ");
		scanf_s("%3c", &input);
		//printf("DEBUG: Введёная строка: %s\n", input);
		temp = clean_stdin();

		if (!input_is_digit(input) or temp != 0) {
			printf("Это не строка из трёх десятичных чисел. Попробуйте снова.\n\n");
			continue;
		}
		printf("------\nВведённая строка на вход программы: %s\n", input);
		
	}

	// Ассемблер
	__asm {
		mov al, input[1]; // ввод второй цифры
		mov ah, input[2]; // ввод третьей цифры
		mov bl, input[0]; // ввод первой цифры

		// Если вторая и третья совпадают и ...
		cmp al, ah; // сравнивание 3го и 2го числа
		jnz other; // иначе
		// ... и если вторая превышает первую ...
		cmp bl, al; // сравнивание 1го и 2го числа
		jae other; // иначе
		// ... и если третья превышает первую
		cmp bl, ah; // сравнивание 1го и 3го числа
		jae other; // иначе

		// то заменить вторую и третью цифры дополнением до 9
		sub al, '9'; // шаг 1 получения дополнения до 9 для 2го числа
		neg al; // шаг 2 получения дополнения до 9 для 2го числа
		add al, 48; // получить и заменить дополнением до 9 для 2го числа
		sub ah, '9'; // шаг 1 получения дополнения до 9 для 3го числа
		neg ah; // шаг 2 получения дополнения до 9 для 3го числа
		add ah, 48; // получить и заменить дополнением до 9 для 3го числа
		jmp fin; // Конец вставки

	other: cmp bl, '9'; // проверка 1го числа на '9'
		jns fin; // Конец вставки, если число есть '9'
		inc bl; // Увеличить первую цифру на 1 ('9' сохраняется).

	fin: mov input[1], al; // возврат второй цифры
		mov input[2], ah; // возврат третьей цифры
		mov input[0], bl; // возврат первой цифры
	}

	printf("Результат: %s\n", input);
	printf("\nПрограмма завершена\n");

	return 0;
}
