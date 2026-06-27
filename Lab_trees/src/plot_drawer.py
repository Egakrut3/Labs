import csv
import matplotlib.pyplot as plt

n = list(range(100_000, 1_000_001, 100_000))

with open("results/AVL.csv", "r") as f:
    reader = csv.reader(f)
    avl_insert = [float(x) for x in next(reader)]
    avl_delete = [float(x) for x in next(reader)]

with open("results/Treap.csv", "r") as f:
    reader = csv.reader(f)
    treap_insert = [float(x) for x in next(reader)]
    treap_delete = [float(x) for x in next(reader)]

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

ax1.plot(n, avl_insert, 'b-o', label='AVL', markersize=4)
ax1.plot(n, treap_insert, 'r-s', label='Treap', markersize=4)
ax1.set_title('Вставка')
ax1.set_xlabel('n')
ax1.set_ylabel('Время')
ax1.legend()
ax1.grid(True)

ax2.plot(n, avl_delete, 'b-o', label='AVL', markersize=4)
ax2.plot(n, treap_delete, 'r-s', label='Treap', markersize=4)
ax2.set_title('Удаление')
ax2.set_xlabel('n')
ax2.set_ylabel('Время')
ax2.legend()
ax2.grid(True)

plt.tight_layout()plt.savefig("results/Plot.png")
plt.show()
