Grzegorz Wierzowiecki 2016

# inventory-where


## Intro

`inventory-where` Is responsible for keeping track of "where is given *item*".

## Abstract Model Behind

For current use-case, I model whole physical space - also called *universe* - as [forest][Tree (graph theory)] (or [tree][Tree (graph theory)] with *"root"* abstract starting vertex numbered `0`), where each item is in another item (or *"root"* `0` psudo-item). Items capable of containing other items will be also called *containers*.

## Tasks

* Collect information about "which *item* is in which *item*.
* Answer queries:
  * list tree/forest
  * show me path to given *item* (from root)

## "Inventory" Input `.inv` file

### Optional Timestamps before commands

Before each command there can be optional timestamp in [ISO 8601] **and colon and space** format exactly matching following pattern (btw matching `date --iso-8601=seconds`):
`YYYY-MM-DDTHH:mm:SS+hh:mm: ` ~ `year-month-dayThour:minutes:seconds+timezone_hour:timezone_minutes: `, example `2016-05-21T01:16:00+02:00: ` .

It has to match exactly (including colon and space).

If such timestamp provided, than it will be used during interpretation of command instead of systemclock. (Usefull for replying commands).

(TODO: provide example and unittests with timestamps.)


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

### Simple Example session

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


### Switching modes (TODO)

TODO: switching between adding and removing items will be done via codes:

```
$++ADDING++$
$++REMOVING++$
```

### Adding Key-Values describing items (TODO)

TODO: Adding Key values using `$?=.?$`, e.g. `$key=value$`.

All one and two letter keys are reserved.

Only keys using `[a-zA-Z0-9]` are allowed for user, other are reserved.

Some of (considered to be) reserved key-val pairs:

* `$u=...` - unique id. Might be translated by above layer `inventory-what`

### Restricted codes

Using codes starting with `$` can lead to underfined behaviour. (Later plan is to relax restriction to codes starting with `$` and having later at some point `$`).

For special codes it's planned to provide special "verbatim" codes, to enter next line or block of lines "verbatim" as they are.

### Input - summary of special codes


Summary of special codes described in this document:

```
$++START_SESSION:V00++$
$++START_SESSION:V0++$
$++EXIT_SESSION++$
$++OPEN_CONTAINER++$
$++CLOSE_CONTAINER++$
```

TODO: Codes to add:

```
$++ADDING++$
$++REMOVING++$
$key=val$
$++SETTING_KEYVALS++$
$++PREVIOUS_MODE++$
$++DUMP++$
$++CLOSE_ALL++$
$++RESET_SESSION++$
```

For special codes it's planned to provide special "verbatim" codes, to enter next line or block of lines "verbatim" as they are.

## Output

## Session `.inv` Log

Whole session is supposed to be tee-ed into sessions log `.inv` file. As it's copy of input, this will allow to reply session (for any purpose, from integrity check to rebuild database).

## Internal

[ISO 8601]: https://en.wikipedia.org/wiki/ISO_8601
[Tree (graph theory)]: https://en.wikipedia.org/wiki/Tree_(graph_theory)
