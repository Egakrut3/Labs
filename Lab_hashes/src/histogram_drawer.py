import matplotlib.pyplot as plt
import csv

files_and_labels = [
	("results/hash_info_0.csv",	"unsigned int: Остаток от деления"),
	("results/hash_info_1.csv",	"unsigned int: Битовое представление"),
	("results/hash_info_2.csv",	"unsigned int: Метод Кнута"),

	("results/hash_info_3.csv",	"float: Преобразование к int"),
	("results/hash_info_4.csv",	"float: Битовое представление"),
	("results/hash_info_5.csv",	"float: Мантисса"),
	("results/hash_info_6.csv",	"float: Экспонента"),
	("results/hash_info_7.csv",	"float: Мантисса * Экспонента"),

	("results/hash_info_8.csv",	"Строка: Длина"),
	("results/hash_info_9.csv",	"Строка: Сумма букв"),
	("results/hash_info_10.csv",	"Строка: Полиномиальный"),
	("results/hash_info_11.csv",	"Строка: CRC32"),
]

for path, title in files_and_labels:
	with open(path) as f:
		reader = csv.reader(f)
		heights = [int(x) for x in next(reader) if x]
	
	plt.bar(range(1000), heights, width=1.0)
	plt.axhline(y=1000, color="r", linestyle="--", label="Идеал (1000)")
	plt.xlabel("Корзина")
	plt.ylabel("Количество ключей")
	plt.title(title)
	plt.legend()
	plt.savefig(path.replace("csv", "png"), dpi=150)
	plt.close()
