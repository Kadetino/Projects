#include <stdio.h>
#include <locale.h>

short int A2asm(char x, char y, char z) {
	short int v=0, temp=0;

	// Выражение: -1+(-3*z+x*(y+2))/(4*z-3)
	__asm { 
		

		// Вычисление числителя (-3*z+x*(y+2))
		mov al, y; // Поместить y в AL
		cbw; // Расширить AL со знаком до слова
		add ax,2; // Вычислить y+2

		mov cx, ax; // Сохранить результат
		mov al, x; // Поместить x в AL

		cbw; // Расширить AL со знаком до слова
		imul cx; // Вычислить x(y+2)

		mov cx, ax; // Сохранить результат
		mov bx, dx;
		mov al, z; // Поместить z в AL
		cbw; // Расширить AL со знаком до слова
		imul ax,-3; // Вычислить 3*z
		//neg ax; // Вычислить -3*z
		add cx, ax; // Вычислить -3*z+x*(y+2)
		adc dx, bx;
		


		// Вычисление знаменателя (4*z-3)
		mov al, z; // Поместить z в AL
		//cbw; // Расширить AL со знаком до слова
		imul al, 4; // Вычислить 4*z 
		sub ax, 3; // Вычислить 4*z-3
		//sbb dx, 0;
		mov bx, ax; // Сохранить результат
		mov ax, cx;

		// Деление
		idiv bx; // Произвести деление AX на (4*z-3)

		// Получение результата из Ассемблера
		dec ax; // уменьшить на единицу AX
		mov v, ax; // Сохранить результат
	}

	return v;
}

void clean_stdin() {
	int c;
	do {
		c = getchar();
	} while (c != '\n' && c != EOF);
}

void tests() {
	char x=0, y=0, z=0;
	short int v=0, v_c=0, v_as=0;
	printf("---Результат на тестовых значениях---\n");
	// Тест 1
	x = -0x2; y = 0x3; z = 0x1; v = -0x0E;
	v_c = -1 + (-3 * (short int)z + (short int)x * ((short int)y + 2)) / (4 * (short int)z - 3);
	v_as = A2asm(x, y, z);
	printf("x = %x, y = %x, z = %x, v = %x \n", x, y, z, v);
	printf("Результат на C:\t%d или\t%x (в 16-ричной СС)\n", v_c, v_c);
	printf("Ассемблер:\t%d или\t%x (в 16-ричной СС)\n\n", v_as, v_as);

	// Тест 2
	x = 0x2E; y = 0x7F; z = -0x0B; v = -0x7F;
	v_c = -1 + (-3 * (short int)z + (short int)x * ((short int)y + 2)) / (4 * (short int)z - 3);
	v_as = A2asm(x, y, z);
	printf("x = %x, y = %x, z = %x, v = %x \n", x, y, z, v);
	printf("Результат на C:\t%d или\t%x (в 16-ричной СС)\n", v_c, v_c);
	printf("Ассемблер:\t%d или\t%x (в 16-ричной СС)\n\n", v_as, v_as);
	printf("-------------------------------------\n");
}

int main() {
	setlocale(LC_ALL, "rus");
	tests();
	char x=0, y=0, z=0;
	short int v_c=0,v_as=0;
	int i=0, everythingIsFine = 1;

	
	while (everythingIsFine) {
		// Выход
		printf("Введите: 0 - для завершения работы; любое число, кроме 0 - для продолжения работы:\n");
		int temp = scanf_s("%x", &everythingIsFine);
		clean_stdin();
		if (temp) {
			if (everythingIsFine == 0) break;
		}
		else {
			printf("Это было не число.\n");
			continue;
		}
		// Ввод значений
		// X
		printf("Значения можно вводить в 16й системе: от -FF до FF\nВведите x:\n");
		if (scanf_s("%x", &i)) {
			clean_stdin();
			x = (char)i;
			// Y
			printf("Введите y:\n");
			if (scanf_s("%x", &i)) {
				clean_stdin();
				y = (char)i;
				// Z
				printf("Введите z:\n");
				if (scanf_s("%x", &i)) {
					clean_stdin();
					z = (char)i;
				}
				else {
					clean_stdin();
					printf("Введенное значение Z не соответствует допустимому диапазону\n\n");
					everythingIsFine = 0;
				}
			}
			else {
				clean_stdin();
				printf("Введенное значение Y не соответствует допустимому диапазону\n\n");
				everythingIsFine = 0;
			}
		}
		else {
			clean_stdin();
			printf("Введенное значение X не соответствует допустимому диапазону\n\n");
			everythingIsFine = 0;
		}

		// Вычисления
		if (((4*z-3) == 0) && everythingIsFine) {
			printf("Деление на 0. Перезапуск.\n\n");
			continue;
		}
		else if(everythingIsFine) {
			v_c = -1 + (-3 * (short int)z + (short int)x * ((short int)y + 2)) / (4 * (short int)z - 3);
			v_as = A2asm(x, y, z);
			printf("x = %x, y = %x, z = %x \n", x, y, z);
			printf("Результат на C:\t%d или\t%x (в 16-ричной СС)\n", v_c, v_c);
			printf("Ассемблер:\t%d или\t%x (в 16-ричной СС)\n\n", v_as, v_as);
		}
		else {
			everythingIsFine = 1;
		}
	}

	printf("\nПрограмма завершена\n");
	return 0;
}
