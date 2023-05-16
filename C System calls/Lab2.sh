#!/bin/sh
#18.Написать программу копирования содержимого одного файла в другой, создавая последний в случае необходимости, 
# и оформить ее в виде команды интерпретатора shell ОС UNIX с выдачей сообщения о количестве блоков копируемого файла (блок = 512 байт) и байтов. 
# Предусмотреть аномальные ситуации и возможность прерывания от клавиатуры.

# Первый аргумент - имя файла-источника. Второй аргумент - конечный файл.
# Прерывание

prer(){
	echo "----Прерывание----"
	echo $lastEcho
}

trap prer 2

# Ввод названия файла-источника
# Проверка что хотя бы 1 аргумент был передан
if test $# -eq 0
	then
		echo "В функцию не были переданы аргументы."
		while :; do
			lastEcho="Введите имя файла-источника:"
			echo $lastEcho
			read fileSource
			# Проверка на то, что файл существует и есть read доступ
			if test -r $fileSource
				then
					# Проверка на то что не поток ввода
					if test -z $fileSource
						then echo "Файл не найден. Попробуйте снова."
						else
							echo "---------------"
							echo "Имя файла источника: $fileSource"
							echo "---------------"
							break
					fi
				else
					echo "Файл $fileSource не найден или нет возможности прочесть его. Попробуйте снова."
			fi
		done
	else
		# Проверка на то, что файл существует и есть read доступ
		if test -r $1
			then
				lastEcho="Введите имя файла-источника:"
				fileSource=$1
				echo "Имя файла источника: $fileSource"
			else
				echo "Файл $1 недоступен для чтения. Завершение программы."
				exit
		fi
fi



# Ввод названия конечного файла
# Проверка что 2 аргумента были переданы
if [ $# -eq 2 ]
	then
		# Проверка на то, что файл существует и есть write доступ
		if test -w $2
			then
				fileTarget=$2
				echo "Имя конечного файла: $fileTarget"
			else
				echo "Файл $2 недоступен для записи. Завершение программы."
				exit
		fi
	else
		echo "В функцию не было передано название конечного файла."
		while :; do
			lastEcho="Введите имя конечного файла:"
			echo $lastEcho
			read fileTarget
			# Проверка на то, что файл существует
			if test -e $fileTarget
				then
					# Проверка на то, что есть write доступ
					if test -w $fileTarget
						then
							echo "---------------"
							echo "Имя конечного файла: $fileTarget"
							echo "---------------"
							break
						else
							echo "Файл $fileTarget недоступен для записи. Попробуйте снова."
					fi
				else
					echo "---------------"
					echo "Имя конечного файла: $fileTarget"
					echo "---------------"
					break
			fi
		done
fi


# Задание
cp $fileSource $fileTarget 2> /dev/null
byteCount=`wc -c $fileSource | cut -d " " -f 1`
blockCount=`ls --block-size=512 -s $fileSource | cut -d " " -f 1`
echo "Копируемый файл имеет $byteCount байтов и $blockCount блоков размером 512 байт"
if diff $fileSource $fileTarget
	then echo "Копирование успешно."
	else echo "Произошла ошибка."
fi
