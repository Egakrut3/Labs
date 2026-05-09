# Tree Benchmarks

## Быстрая проверка

Собрать и запустить маленькие smoke-тесты для всех деревьев:

```sh
make -C Lab_trees smoke_all
```

Для одного дерева:

```sh
make -C Lab_trees smoke TREE=AVL_tree
```

Доступные значения `TREE`:

```text
Naive_tree AVL_tree Treap Splay_tree RB_tree B_tree Skip_list
```

## Запуск benchmark

Один benchmark для одного дерева:

```sh
make -C Lab_trees bench TREE=AVL_tree DATA_TYPE=random OPERATION=all RELEASE=1
```

Параметры:

- `DATA_TYPE=random` или `DATA_TYPE=sorted`
- `OPERATION=insert`, `OPERATION=erase` или `OPERATION=all`
- `RELEASE=1` включает оптимизированную сборку для замеров
- `HYPERFINE_RUNS=5` задаёт число повторов

Пример с одним размером:

```sh
make -C Lab_trees bench TREE=AVL_tree DATA_TYPE=random OPERATION=insert INSERT_CNT=100000 RELEASE=1
```

## Все тесты

Запустить random-тесты для всех деревьев:

```sh
make -C Lab_trees bench_all DATA_TYPE=random OPERATION=all RELEASE=1
```

Запустить набор по заданию:

```sh
make -C Lab_trees bench_required RELEASE=1
```

`bench_required` запускает random-тесты для всех деревьев, а также sorted-тесты для `Naive_tree` и `AVL_tree`.

## Графики

Построить графики для одного дерева:

```sh
make -C Lab_trees graph TREE=AVL_tree DATA_TYPE=random OPERATION=all
```

Построить графики для всех деревьев:

```sh
make -C Lab_trees graph_all DATA_TYPE=random OPERATION=all
```

Построить графики для набора по заданию:

```sh
make -C Lab_trees graph_required
```

Результаты лежат в папке конкретного дерева:

```text
Lab_trees/<Tree>/results/raw/
Lab_trees/<Tree>/results/summary/
Lab_trees/<Tree>/results/graphs/
```

## Очистка

Очистить одно дерево:

```sh
make -C Lab_trees clean TREE=AVL_tree
```

Очистить все деревья:

```sh
make -C Lab_trees clean_all
```
