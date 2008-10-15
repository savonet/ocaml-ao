(*
 Copyright (C) 2003-2006  Bardur Arantsson
 Copyright (C) 2006-2007  the Savonet Team

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

open Printf

let () =
   (* List all the drivers. *)
   List.iter
     (fun driver ->
        printf "\nname: %s\n" (Ao.driver_name driver);
        printf "short_name: %s\n" (Ao.driver_short_name driver);
        printf "comment: %s\n" (Ao.driver_comment driver);
        printf "author: %s\n" (Ao.driver_author driver);
        printf "priority: %d\n" (Ao.driver_priority driver);

        printf "pref. format: %s\n" (
          match (Ao.driver_preferred_byte_format driver) with
              | `NATIVE -> "native"
              | `BIG_ENDIAN -> "big endian"
              | `LITTLE_ENDIAN -> "little endian"
              | `UNKNOWN -> "(unknown)");

        printf "kind: %s\n" (
          match (Ao.driver_kind driver) with
              | `LIVE -> "Live"
              | `FILE -> "File"
              | `UNKNOWN -> "(unknown)");

        printf "options: [%s]\n"
          (String.concat "," (Ao.driver_options driver));
     )
      Ao.drivers;

   let device =
     Ao.open_live ~options:[("x","y");("x1","y1")]
       ~driver:(Ao.find_driver "null") ()
   in
   let buf = String.create 41029 in
     (* Play garbage (literally!). *)
     Ao.play device buf;
     (* Close device. *)
     Ao.close device;
     (* And we're done. *)
     printf "Goodbye!\n"
