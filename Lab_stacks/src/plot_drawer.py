import csv
import matplotlib.pyplot as plt

def read_csv_to_list(filename):
	with open(filename, 'r') as f:
		reader = csv.reader(f)
		for row in reader:
			return [float(x.strip()) for x in row if x.strip()]
	return []

DA_data = read_csv_to_list("results/DA_4.csv")
FL_data = read_csv_to_list("results/FL_4.csv")

n_values = [1000 * i for i in range(1, 1001)]

plt.figure(figsize = (12, 7))
plt.plot(n_values, DA_data, label = "Stack on DA", color = "blue", linewidth = 1.5)
plt.plot(n_values, FL_data, label = "Stack on FL", color = "red", linewidth = 1.5)

plt.xlabel("Number of pushes")
plt.ylabel("Time (seconds)")
plt.title("Stack Performance: Dynamic Array vs Forward List")
plt.legend()
plt.grid(True, linestyle='--', alpha=0.6)

plt.tight_layout()
plt.savefig("results/Plot.png")
plt.show()
