# inventory-where


## Intro

`inventory-where` Is responsible for keeping track of "where is given *item*".

## Abstract Model Behind

For current use-case, I model physical space as [forest][Tree (graph theory)] (or [tree][Tree (graph theory)] with *"root"* abstract starting vertex numbered `0`), where each item is in another item (or *"root"* `0` psudo-item). Items capable of containing other items will be also called *containers*.

## Tasks

* Collect information about "which *item* is in which *item*.
* Answer queries:
  * list tree/forest
  * show me path to given *item* (from root)

## "Inventory" Input `.inv` file

### Session

Starts with `$++START_SESSION:V00++$` (also `$++START_SESSION:V0++$` should work for V00..V09 revisions), should end with `$++EXIT_SESSION++$`.

### Basic structure of putting *items* into *containers*

Interleaved line pairs:

```
container-id
item-id
```

Describing that in given *container*, given *item* is placed.

For convenience of putting more *items* into given *container*, we can open and close it:

```
container-id
$++OPEN_CONTAINER++$
item1
item2
...
itemn
$++CLOSE_CONTAINER++$
```

### Example session

```
$++START_SESSION:V00++$
1234
5678
1234
9012
98765
$++OPEN_CONTAINER++$
12
34
56
78
$++CLOSE_CONTAINER++$
$++EXIT_SESSION++$

```

Describes following [tree][Tree (graph theory)] of things:

* Universe
    * 1234
        * 5678
        * 9012
    * 98765
        * 12
        * 34
        * 56
        * 78

## Output

## Session `.inv` Log

Whole session is supposed to be tee-ed into sessions log `.inv` file. As it's copy of input, this will allow to reply session (for any purpose, from integrity check to rebuild database).

## Internal

[Tree (graph theory)]: https://en.wikipedia.org/wiki/Tree_(graph_theory)
