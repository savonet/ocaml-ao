(lang dune 2.8)
(version 0.2.4)
(name ao)
(source (github savonet/ocaml-ao))
(license LGPL-2.1)
(authors "The Savonet Team <savonet-users@lists.sourceforge.net>")
(maintainers "The Savonet Team <savonet-users@lists.sourceforge.net>")

(generate_opam_files true)
(use_standard_c_and_cxx_flags false)

(package
 (name ao)
 (synopsis "Bindings for the AO library which provides high-level functions for using soundcards")
 (depends
  conf-ao
  conf-pkg-config
  dune
  dune-configurator)
)
