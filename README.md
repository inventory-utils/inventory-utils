Grzegorz Wierzowiecki 2016

# Introduction

## Initial motivation

`inventory-utils` are aimed to support in task of organising information about collected physical items, using barcode reader and barcodes. Imagine food items in packages containing barcodes, and shelves in your kitchen labeled with barcodes or boxes with electronic items. This might be especially helpful in cellar, where you rarely go, and might be hard to remember not only in which box is what, but also what do the heck we even have in cellar.

## Hardware / Project assumes standard input

Most barcode readers, can be attached as keyboard (e.g. via usb, bluetooth) and are visible as keyboard. So project assumed (at this point) standard input (keyboard/file/whatever).

# Design

## KISS

Following [KISS principle], I try to structure whole system into parts responsible for limited tasks, so they can be simple, specialised, understandable, finally leading to programs doing their tasks well.

For the overall task of items (currently mostly physical with barcodes are in my consideration) *inventory*, so far I've splited task into following parts and programs:

* `inventory-where`
* `inventory-what`

Under consideration:

* `inventory-contents` - supporting in task of listing contents of containers, like generating lists "what is inside given box" that one can be print and sticked on a box.

### inventory-where

Status: *Currently worked on*

`inventory-where` Is responsible for keeping track of "where is given *item*".

For more info, please check [inventory-where-README.md].

### inventory-what

Status: *Gated on inventory-where*

#### TL;DR-

`inventory-what` is responsible for translating codes that are used to identify items, into richer and more meaningful to users descriptions, richer data.

#### Tasks

* Manage information about *items*
* Support user in collecting such information
  * for example ask user if one wants to provide more data each time new item is seen
* Help user to find code identifying interesting to him item

# License

[Apache License 2.0] is located in `LICENSE` file and based on auto generated template by Github, probably same as one here in overview: http://choosealicense.com/licenses/apache-2.0/.

If anyone needs for any reason more permissive license (like BSD, X11 AKA MIT or even Public Domain), I am happy to release it under multiple licenses if there will be any reason, please get in touch with me - Grzegorz Wierzowiecki.

Therefore, all people contributing to this repository agree that code might be released even at Public Domain license at any point. I've chosen *Apache License 2.0* currently as it seemed permissive enough and compatible to me with some other open source licenses - check [License compatibility] and [License compatibility diagram] - so one could use this code/project in project released on LGPLv3, GPLv3, while **protecting user in patent cases** (as opposed to more lax licenses like X11 AKA MIT, which lack of such protection clauses).

[Apache License 2.0]: http://www.apache.org/licenses/LICENSE-2.0.html

# TDD

In order to provide framework for [TDD] (Test Driven Development), skeleton provides `make test` target that is supposed to return `0` on success and `1` on failure. Such is supposed to run all unit-tests. `make test` is triggered in `.travis.yml` automatically since commit https://github.com/inventory-utils/inventory-utils/commit/151264bdbb9b1043bfe43955912e63c4dbb1780a .

## Travis CI - Continous Integration

Continuous integration test is setup in `.travis.yml` and dashboard at: https://travis-ci.org/inventory-utils/

[KISS principle]: https://en.wikipedia.org/wiki/KISS_principle
[License compatibility diagram]: https://en.wikipedia.org/wiki/File:Floss-license-slide-image.png
[License compatibility]: https://en.wikipedia.org/wiki/License_compatibility
[TDD]: https://en.wikipedia.org/wiki/Test-driven_development
[Tree (graph theory)]: https://en.wikipedia.org/wiki/Tree_(graph_theory)
[inventory-where-README.md]: https://github.com/inventory-utils/inventory-utils/blob/master/docs/inventory-where-README.md
