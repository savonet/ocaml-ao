(*
  Copyright (C) 2003  Bardur Arantsson

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*)

type t

type driver_t

type driver_kind_t = [`LIVE | `FILE | `UNKNOWN]

type byte_format_t = [`LITTLE_ENDIAN | `BIG_ENDIAN | `NATIVE | `UNKNOWN]

external _initialize : unit -> unit = "_ao_stubs_initialize"

external _shutdown : unit -> unit = "_ao_stubs_shutdown"

let () =
  _initialize ();
  at_exit _shutdown

external get_default_driver : unit -> driver_t = "_ao_stubs_get_default_driver"

external get_drivers : unit -> driver_t list = "_ao_stubs_get_drivers"

external find_driver : string -> driver_t = "_ao_stubs_find_driver"

let drivers = get_drivers ()

external driver_kind : driver_t -> driver_kind_t = "_ao_stubs_driver_kind"

external driver_name : driver_t -> string = "_ao_stubs_driver_name"

external driver_short_name : driver_t -> string = "_ao_stubs_driver_short_name"

external driver_comment : driver_t -> string = "_ao_stubs_driver_comment"

external driver_author : driver_t -> string = "_ao_stubs_driver_author"

external driver_priority : driver_t -> int = "_ao_stubs_driver_priority"

external driver_preferred_byte_format : driver_t -> byte_format_t = "_ao_stubs_driver_preferred_byte_format"

external driver_options : driver_t -> string list = "_ao_stubs_driver_options"

external open_live_aux : int -> int -> int -> byte_format_t -> (string*string) list -> driver_t -> t =
  "_ao_stubs_open_live_aux_bytecode" "_ao_stubs_open_live_aux_native"

let open_live
      ?bits:(bits=16)
      ?rate:(rate=44100)
      ?channels:(channels=2)
      ?byte_format:(byte_format=`LITTLE_ENDIAN)
      ?options:(options=[])
      ?driver:(driver=get_default_driver ()) () =
  open_live_aux bits rate channels byte_format options driver

external open_file_aux : int -> int -> int -> byte_format_t -> (string*string) list -> driver_t -> bool -> string -> t =
  "_ao_stubs_open_file_aux_bytecode" "_ao_stubs_open_file_aux_native"

let open_file
  ?bits:(bits=16)
  ?rate:(rate=44100)
  ?channels:(channels=2)
  ?byte_format:(byte_format=`LITTLE_ENDIAN)
  ?options:(options=[])
  ?driver:(driver=get_default_driver ())
  ?overwrite:(overwrite=false)
  (filename:string) =
   open_file_aux bits rate channels byte_format options driver overwrite filename

external play : t -> string -> unit =
  "_ao_stubs_play"

external close : t -> unit =
  "_ao_stubs_close"
