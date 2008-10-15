/* $Id: pymadfile.c,v 1.20 2003/02/05 06:29:23 jaq Exp $
 *
 * Copyright (C) 2003  Bardur Arantsson
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
 * USA
 */

#include <stdio.h>
#include <assert.h>
#include <ao/ao.h>
#include <string.h>
#include <caml/memory.h>
#include <caml/signals.h>
#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/fail.h>

#define my_failwith(format, args ...) \
  { char errmsg[1024]; snprintf(errmsg, sizeof(errmsg), format, args); caml_failwith(errmsg); }

/* polymorphic variant utility macros */
#define decl_var(x) static value var_##x
#define import_var(x) var_##x = caml_hash_variant(#x)
#define get_var(x) var_##x


/* cached polymorphic variants */
decl_var(UNKNOWN);
decl_var(LITTLE_ENDIAN);
decl_var(BIG_ENDIAN);
decl_var(NATIVE);
decl_var(LIVE);
decl_var(FILE);

/* initialize the module */
void _ao_stubs_initialize() {
  /* initialize polymorphic variants */
  import_var(UNKNOWN);
  import_var(LITTLE_ENDIAN);
  import_var(BIG_ENDIAN);
  import_var(NATIVE);
  import_var(LIVE);
  import_var(FILE);
  /* initialize libao */
  ao_initialize();
}

/* device type */
struct ao_device_t {
  ao_device *device;
};

/* convert raw int to byte_format_t */
static value bf_of_int(int format)
{
  switch (format)
    {
    case AO_FMT_LITTLE:
      return get_var(LITTLE_ENDIAN);
    case AO_FMT_BIG:
      return get_var(BIG_ENDIAN);
    case AO_FMT_NATIVE:
      return get_var(NATIVE);
    default:
      return get_var(UNKNOWN);
    }
}

static value list_prepend(value tail, value el)
{
  /* List elements are just tuples containing an element
     and a 'next' pointer. */
  value head = caml_alloc_tuple(2);

  Field(head,0) = el;
  Field(head,1) = tail;

  return head;
}

void _ao_stubs_shutdown() {
  /* shut down libao */
  ao_shutdown();
}

static void dealloc_device(value v)
{
  struct ao_device_t *dev =
    (struct ao_device_t *) Field(v,1);

  if (dev->device) {
    /* perform cleanup */
    ao_close(dev->device);
    dev->device = NULL;
  }
}

static value handle_open_error()
{
  switch (errno)
    {
    case AO_ENODRIVER:
      caml_failwith("No appropriate driver");
    case AO_ENOTLIVE:
      caml_failwith("Requested driver is not \"live\"");
    case AO_ENOTFILE:
      caml_failwith("Requested driver is not a \"file\" driver");
    case AO_EFAIL:
      caml_failwith("Failed to initialize device");
    case AO_EOPENDEVICE:
      caml_failwith("Failed to open device");
    case AO_EFILEEXISTS:
      caml_failwith("Failed to open device: File already exists");
    case AO_EOPENFILE:
      caml_failwith("Failed to open device: Cannot create output file");
    default:
      caml_failwith("Failed to open device: Unknown error");
    }
}

static void set_format(ao_sample_format *format,
                       value bits,
                       value rate,
                       value channels,
                       value byte_format)
{
  format->bits = Int_val(bits);
  format->rate = Int_val(rate);
  format->channels = Int_val(channels);

  if (byte_format == get_var(LITTLE_ENDIAN)) { 
    format->byte_format = AO_FMT_LITTLE;
  } else if (byte_format == get_var(BIG_ENDIAN)) {
    format->byte_format = AO_FMT_BIG;
  } else if (byte_format == get_var(NATIVE)) {
    format->byte_format = AO_FMT_NATIVE;
  } else {
    caml_failwith("Unknown format given.");
  }
}

static void set_options(ao_option **_options, value opts)
{
  while (opts != Val_int(0)) {
    value v_pair,e1,e2;
    /* extract (name,value) pair */
    v_pair = Field(opts, 0);
    e1 = Field(v_pair, 0);
    e2 = Field(v_pair, 1);
    /* add to options */
    if (!ao_append_option(_options,
                          String_val(e1),
                          String_val(e2))) {
      my_failwith("Couldn't append option \"%s\"", String_val(e1));
    };
    /* next! */
    opts = Field(opts, 1);
  };
}

value _ao_stubs_open_live_aux_native(value bits, value rate, value channels,
                                     value byte_format, value opts, value driver)
{
  struct ao_device_t *dev;
  ao_option *options = NULL;
  ao_sample_format format;

  /* allocate device */
  if ((dev = malloc(sizeof(struct ao_device_t)))==NULL) {
    caml_raise_out_of_memory();
  };
  /* no device to start with */
  dev->device = NULL;
  /* set format */
  set_format(&format, bits, rate, channels, byte_format);
  /* set options */
  set_options(&options, opts);
  /* open device */
  if (!(dev->device = ao_open_live(Int_val(driver),&format, options)))
    {
      /* free options */
      ao_free_options(options);
      /* raise an exception */
      handle_open_error();
    };
  /* free options */
  ao_free_options(options);
  /* return */
  {
    value v_retval = caml_alloc_final(2, dealloc_device, 100, 50000);
    Field(v_retval,1) = (value) dev;
    return v_retval;
  }
}

value _ao_stubs_open_live_aux_bytecode(value *args, int n)
{
  return _ao_stubs_open_live_aux_native(args[0],args[1],args[2],
                                        args[3],args[4],args[5]);
}

value _ao_stubs_open_file_aux_native(value bits, value rate, 
                                     value channels, value byte_format,
                                     value opts,  value driver,
                                     value overwrite, value filename)
{
  struct ao_device_t *dev;
  ao_option *options = NULL;
  ao_sample_format format;

  /* allocate device */
  if ((dev = malloc(sizeof(struct ao_device_t)))==NULL) {
    caml_raise_out_of_memory();
  };
  /* no device to start with */
  dev->device = NULL;
  /* set format */
  set_format(&format, bits, rate, channels, byte_format);
  /* set options */
  set_options(&options, opts);
  /* open file */
  if (!(dev->device = ao_open_file(Int_val(driver),String_val(filename),
                                   Bool_val(overwrite),&format, options)))
    {
      /* free options */
      ao_free_options(options);
      /* raise an exception */
      handle_open_error();
    };
  /* free options */
  ao_free_options(options);
  /* return */
  {
    value v_retval = caml_alloc_final(2, dealloc_device, 100, 50000);
    Field(v_retval,1) = (value) dev;
    return v_retval;
  }
}

value _ao_stubs_open_file_aux_bytecode(value *args, int n)
{
  return _ao_stubs_open_file_aux_native(args[0],args[1],args[2],args[3],
                                        args[4],args[5],args[6],args[7]);
}

void _ao_stubs_close(value v_dev) {
  dealloc_device(v_dev);
}

void _ao_stubs_play(value v_dev, value v_buf) {

  struct ao_device_t *dev =
    (struct ao_device_t *) Field(v_dev,1);
  int n = caml_string_length(v_buf);
  char* buf = malloc(n);

  assert(dev != NULL && buf != NULL);

  if (dev->device==NULL) {
    caml_failwith("Cannot play on a closed device");
  };

  memcpy(buf,String_val(v_buf),n);
  caml_enter_blocking_section();
  ao_play(dev->device, buf, n);
  caml_leave_blocking_section();
  free(buf);
}

value _ao_stubs_get_default_driver()
{
  return Val_int(ao_default_driver_id());
}

value _ao_stubs_get_drivers()
{
  int n;
  value list = Val_unit;
  /* get number of drivers */
  ao_driver_info_list(&n);
  /* build list of drivers backwards */
  while (--n >= 0) {
    list = list_prepend(list, Val_int(n));
  };
  /* return the completed list */
  return list;
}

value _ao_stubs_find_driver(value v_short_name)
{
  int driver =
    ao_driver_id(String_val(v_short_name));

  if (driver < 0) {
    my_failwith("Could not find driver for name \"%s\"", String_val(v_short_name));
  };

  return Val_int(driver);
}

value _ao_stubs_driver_kind(value v_drv)
{
  ao_info *info =
    ao_driver_info(Int_val(v_drv));

  assert(info != NULL);

  switch (info->type)
    {
    case AO_TYPE_LIVE:
      return get_var(LIVE);
    case AO_TYPE_FILE:
      return get_var(FILE);
    default:
      /* should not happen, but you never know. */
      return get_var(UNKNOWN);
    };
}

value _ao_stubs_driver_name(value v_drv)
{
  ao_info *info = ao_driver_info(Int_val(v_drv));
  assert(info != NULL);
  return caml_copy_string(info->name);
}

value _ao_stubs_driver_short_name(value v_drv)
{
  ao_info *info = ao_driver_info(Int_val(v_drv));
  assert(info != NULL);
  return caml_copy_string(info->short_name);
}

value _ao_stubs_driver_comment(value v_drv)
{
  ao_info *info = ao_driver_info(Int_val(v_drv));
  assert(info != NULL);
  return caml_copy_string(info->comment);
}

value _ao_stubs_driver_author(value v_drv)
{
  ao_info *info = ao_driver_info(Int_val(v_drv));
  assert(info != NULL);
  return caml_copy_string(info->author);
}

value _ao_stubs_driver_priority(value v_drv)
{
  ao_info *info = ao_driver_info(Int_val(v_drv));
  assert(info != NULL);
  return Val_int(info->priority);
}

value _ao_stubs_driver_preferred_byte_format(value v_drv)
{
  ao_info *info = ao_driver_info(Int_val(v_drv));
  assert(info != NULL);
  return bf_of_int(info->preferred_byte_format);
}

value _ao_stubs_driver_options(value v_drv)
{
  int i;
  ao_info *info;
  const char *opt;
  value list = Val_unit;

  info = ao_driver_info(Int_val(v_drv));

  assert(info != NULL);

  for (i = info->option_count-1; i>=0; i--) {
    if ((opt = info->options[i])) {
      list = list_prepend(list, caml_copy_string(opt));
    }
  }

  return list;
}
