ocaml-ao
========

This package contains an OCaml interface for the cross-platform
audio output library, otherwise known as libao.

Please read the COPYING file before using this software.

Prerequisites:
==============

- ocaml
- libao
- findlib
- dune >= 2.0

Compilation:
============

```
$ dune build
```

This should build both the native and the byte-code version of the
extension library.

Installation:
=============

Via `opam`:

```
$ opam install ao
```

Via `dune` (for developers):
```
$ dune install
```

Author:
=======

This author of this software may be contacted by electronic mail
at the following address: savonet-users@lists.sourceforge.net.
