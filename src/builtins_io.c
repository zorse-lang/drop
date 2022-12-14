#include "builtins.h"
#include "builtins_io.h"
#include "doops.h"

#ifndef NO_IO

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif

JS_C_FUNCTION(js_open)
{
  JS_ParameterString(ctx, 0);
  JS_ParameterNumber(ctx, 1);

  int mode = 0666;
  if (JS_ParameterCount(ctx) > 2)
  {
    JS_ParameterNumber(ctx, 2);
    mode = JS_GetIntParameter(ctx, 2);
  }
  const char *s0 = JS_GetStringParameter(ctx, 0);
  int fd = open(s0, JS_GetIntParameter(ctx, 1) | O_BINARY, mode);
  JS_FreeString(ctx, s0);
  JS_RETURN_NUMBER(ctx, fd);
}

JS_C_FUNCTION(js_close)
{
  JS_ParameterNumber(ctx, 0);
  int err = close(JS_GetIntParameter(ctx, 0));
  JS_RETURN_NUMBER(ctx, err);
}

JS_C_FUNCTION(js_fsync)
{
  JS_ParameterNumber(ctx, 0);
  int err = fsync(JS_GetIntParameter(ctx, 0));
  JS_RETURN_NUMBER(ctx, err);
}

JS_C_FUNCTION(js_fdatasync)
{
  JS_ParameterNumber(ctx, 0);
  int err = fdatasync(JS_GetIntParameter(ctx, 0));
  JS_RETURN_NUMBER(ctx, err);
}

JS_C_FUNCTION(js_seek)
{
  JS_ParameterNumber(ctx, 0);
  JS_ParameterNumber(ctx, 1);

  off_t err = lseek(JS_GetIntParameter(ctx, 0), (off_t)JS_GetNumberParameter(ctx, 1), SEEK_SET);
  JS_RETURN_NUMBER(ctx, err);
}

JS_C_FUNCTION(js_truncate)
{
  JS_ParameterString(ctx, 0);
  JS_ParameterNumber(ctx, 1);

  const char *s0 = JS_GetStringParameter(ctx, 0);
  int err = truncate(s0, (off_t)JS_GetNumberParameter(ctx, 1));
  JS_FreeString(ctx, s0);
  JS_RETURN_NUMBER(ctx, err);
}

JS_C_FUNCTION(js_mkdir)
{
  JS_ParameterString(ctx, 0);
  JS_ParameterNumber(ctx, 1);
  const char *s0 = JS_GetStringParameter(ctx, 0);
  int err = mkdir(s0, JS_GetIntParameter(ctx, 1));
  JS_FreeString(ctx, s0);
  JS_RETURN_NUMBER(ctx, err);
}

JS_C_FUNCTION(js_rename)
{
  JS_ParameterString(ctx, 0);
  JS_ParameterString(ctx, 1);
  const char *s0 = JS_GetStringParameter(ctx, 0);
  const char *s1 = JS_GetStringParameter(ctx, 1);
  int err = rename(s0, s1);
  JS_FreeString(ctx, s0);
  JS_FreeString(ctx, s1);
  JS_RETURN_NUMBER(ctx, err);
}

JS_C_FUNCTION(js_rmdir)
{
  JS_ParameterString(ctx, 0);
  const char *s0 = JS_GetStringParameter(ctx, 0);
  int err = rmdir(s0);
  JS_FreeString(ctx, s0);
  JS_RETURN_NUMBER(ctx, err);
}

JS_C_FUNCTION(js_unlink)
{
  JS_ParameterString(ctx, 0);
  const char *s0 = JS_GetStringParameter(ctx, 0);
  int err = unlink(s0);
  JS_FreeString(ctx, s0);
  JS_RETURN_NUMBER(ctx, err);
}

JS_C_FUNCTION(js_access)
{
  JS_ParameterString(ctx, 0);
  JS_ParameterNumber(ctx, 1);
  const char *s0 = JS_GetStringParameter(ctx, 0);
  int err = access(s0, JS_GetIntParameter(ctx, 1));
  JS_FreeString(ctx, s0);
  JS_RETURN_NUMBER(ctx, err);
}

static js_object_type stat_to_js(JS_CONTEXT ctx, struct stat *st_buf)
{
  js_object_type obj = JS_NewClassObject(ctx, "Stats");

  JS_ObjectSetNumber(ctx, obj, "dev", st_buf->st_dev);
  JS_ObjectSetNumber(ctx, obj, "ino", st_buf->st_ino);
  JS_ObjectSetNumber(ctx, obj, "mode", st_buf->st_mode);
  JS_ObjectSetNumber(ctx, obj, "nlink", st_buf->st_nlink);
  JS_ObjectSetNumber(ctx, obj, "uid", st_buf->st_uid);
  JS_ObjectSetNumber(ctx, obj, "gid", st_buf->st_gid);
  JS_ObjectSetNumber(ctx, obj, "rdev", st_buf->st_rdev);
  JS_ObjectSetNumber(ctx, obj, "size", st_buf->st_size);
  JS_ObjectSetNumber(ctx, obj, "blksize", st_buf->st_blksize);
  JS_ObjectSetNumber(ctx, obj, "blocks", st_buf->st_blocks);
  JS_ObjectSetNumber(ctx, obj, "atime", st_buf->st_atime);
  JS_ObjectSetNumber(ctx, obj, "mtime", st_buf->st_mtime);
  JS_ObjectSetNumber(ctx, obj, "ctime", st_buf->st_ctime);

  return obj;
}

JS_C_FUNCTION(js_stat)
{
  JS_ParameterString(ctx, 0);
  struct stat st_buf;
  const char *s0 = JS_GetStringParameter(ctx, 0);
  int err = stat(s0, &st_buf);
  JS_FreeString(ctx, s0);
  if (err)
    JS_RETURN_NOTHING(ctx);

  js_object_type obj_id = stat_to_js(ctx, &st_buf);
  JS_RETURN_OBJECT(ctx, obj_id);
}

JS_C_FUNCTION(js_fstat)
{
  JS_ParameterNumber(ctx, 0);
  struct stat st_buf;
  int err = fstat(JS_GetIntParameter(ctx, 0), &st_buf);
  if (err)
    JS_RETURN_NOTHING(ctx);

  js_object_type obj_id = stat_to_js(ctx, &st_buf);
  JS_RETURN_OBJECT(ctx, obj_id);
}

JS_C_FUNCTION(js_chmod)
{
  JS_ParameterString(ctx, 0);
  JS_ParameterNumber(ctx, 1);
  const char *s0 = JS_GetStringParameter(ctx, 0);
  int err = chmod(s0, JS_GetIntParameter(ctx, 1));
  JS_FreeString(ctx, s0);
  JS_RETURN_NUMBER(ctx, err);
}

JS_C_FUNCTION(js_chown)
{
  JS_ParameterString(ctx, 0);
  JS_ParameterNumber(ctx, 1);
  JS_ParameterNumber(ctx, 2);
  const char *s0 = JS_GetStringParameter(ctx, 0);
  int err = chown(s0, (uid_t)JS_GetIntParameter(ctx, 1), (gid_t)JS_GetIntParameter(ctx, 2));
  JS_FreeString(ctx, s0);
  JS_RETURN_NUMBER(ctx, err);
}

JS_C_FUNCTION(js_link)
{
  JS_ParameterString(ctx, 0);
  JS_ParameterString(ctx, 1);
  const char *s0 = JS_GetStringParameter(ctx, 0);
  int err = link(s0, JS_GetStringParameter(ctx, 1));
  JS_FreeString(ctx, s0);
  JS_RETURN_NUMBER(ctx, err);
}

JS_C_FUNCTION(js_ftruncate)
{
  JS_ParameterNumber(ctx, 0);
  JS_ParameterNumber(ctx, 1);

  int err = ftruncate(JS_GetIntParameter(ctx, 0), (off_t)JS_GetNumberParameter(ctx, 1));
  JS_RETURN_NUMBER(ctx, err);
}

JS_C_FUNCTION(js_lstat)
{
  JS_ParameterString(ctx, 0);
  struct stat st_buf;
  const char *s0 = JS_GetStringParameter(ctx, 0);
  int err = lstat(s0, &st_buf);
  JS_FreeString(ctx, s0);
  if (err)
    JS_RETURN_NOTHING(ctx);

  js_object_type obj_id = stat_to_js(ctx, &st_buf);
  JS_RETURN_OBJECT(ctx, obj_id);
}

JS_C_FUNCTION(js_fchmod)
{
  JS_ParameterNumber(ctx, 0);
  JS_ParameterNumber(ctx, 1);
  int err = fchmod(JS_GetIntParameter(ctx, 0), JS_GetIntParameter(ctx, 1));
  JS_RETURN_NUMBER(ctx, err);
}

JS_C_FUNCTION(js_lchmod)
{
  JS_ParameterString(ctx, 0);
  JS_ParameterNumber(ctx, 1);
  const char *s0 = JS_GetStringParameter(ctx, 0);
  int err = lchmod(s0, JS_GetIntParameter(ctx, 1));
  JS_FreeString(ctx, s0);
  JS_RETURN_NUMBER(ctx, err);
}

JS_C_FUNCTION(js_fchown)
{
  JS_ParameterNumber(ctx, 0);
  JS_ParameterNumber(ctx, 1);
  JS_ParameterNumber(ctx, 2);
  int err = fchown(JS_GetIntParameter(ctx, 0), (uid_t)JS_GetIntParameter(ctx, 1), (gid_t)JS_GetIntParameter(ctx, 2));
  JS_RETURN_NUMBER(ctx, err);
}

JS_C_FUNCTION(js_lchown)
{
  JS_ParameterString(ctx, 0);
  JS_ParameterNumber(ctx, 1);
  JS_ParameterNumber(ctx, 2);
  const char *s0 = JS_GetStringParameter(ctx, 0);
  int err = lchown(s0, (uid_t)JS_GetIntParameter(ctx, 1), (gid_t)JS_GetIntParameter(ctx, 2));
  JS_FreeString(ctx, s0);
  JS_RETURN_NUMBER(ctx, err);
}

JS_C_FUNCTION(js_symlink)
{
  JS_ParameterString(ctx, 0);
  JS_ParameterString(ctx, 1);
  const char *s0 = JS_GetStringParameter(ctx, 0);
  const char *s1 = JS_GetStringParameter(ctx, 1);
  int err = symlink(s0, s1);
  JS_FreeString(ctx, s0);
  JS_FreeString(ctx, s1);
  JS_RETURN_NUMBER(ctx, err);
}

JS_C_FUNCTION(js_realpath)
{
  char resolved_path[PATH_MAX];

  JS_ParameterString(ctx, 0);
  const char *s0 = JS_GetStringParameter(ctx, 0);
  char *str = realpath(s0, resolved_path);
  JS_FreeString(ctx, s0);
  if (str)
    JS_RETURN_STRING(ctx, str);
  JS_RETURN_UNDEFINED(ctx);
}

JS_C_FUNCTION(js_opendir)
{
  JS_ParameterString(ctx, 0);
  const char *s0 = JS_GetStringParameter(ctx, 0);
  DIR *dir = opendir(s0);
  JS_FreeString(ctx, s0);
  if (dir)
    JS_RETURN_POINTER(ctx, dir);
  JS_RETURN_UNDEFINED(ctx);
}

JS_C_FUNCTION(js_closedir)
{
  JS_ParameterPointer(ctx, 0);
  int err = closedir((DIR *)JS_GetPointerParameter(ctx, 0));
  JS_RETURN_NUMBER(ctx, err);
}

JS_C_FUNCTION(js_readdir)
{
  JS_ParameterPointer(ctx, 0);
  struct dirent *e = readdir((DIR *)JS_GetPointerParameter(ctx, 0));
  if (e)
    JS_RETURN_STRING(ctx, e->d_name);
  JS_RETURN_NULL(ctx);
}

JS_C_FUNCTION(js_errno)
{
  JS_RETURN_NUMBER(ctx, errno);
}

JS_C_FUNCTION(js_strerror)
{
  JS_ParameterNumber(ctx, 0);
  JS_RETURN_STRING(ctx, strerror(JS_GetIntParameter(ctx, 0)));
}

JS_C_FUNCTION(js_read)
{
  JS_ParameterNumber(ctx, 0);
  JS_ParameterNumber(ctx, 2);

  js_size_t sz = 0;
  void *buf = JS_GetBufferParameter(ctx, 1, &sz);
  size_t nbytes = (size_t)JS_GetNumberParameter(ctx, 2);
  ssize_t offset = 0;

  if (JS_ParameterCount(ctx) > 3)
  {
    JS_ParameterNumber(ctx, 3);
    offset = (ssize_t)JS_GetNumberParameter(ctx, 3);
    if (offset < 0)
      offset = 0;
  }

  if (offset >= sz)
    JS_RETURN_NUMBER(ctx, 0);

  if (sz < (offset + nbytes))
    nbytes = sz - offset;

  if (offset)
    JS_RETURN_NUMBER(ctx, 0);
  ssize_t bytes_read = read(JS_GetIntParameter(ctx, 0), (unsigned char *)buf + offset, nbytes);
  JS_RETURN_NUMBER(ctx, bytes_read);
}

JS_C_FUNCTION(js_write)
{
  void *buf;
  js_size_t sz;

  JS_ParameterNumber(ctx, 0);
  buf = JS_GetBufferParameter(ctx, 1, &sz);

  ssize_t offset = 0;
  ssize_t nbytes = sz;
  int n = JS_ParameterCount(ctx);

  if (n > 2)
  {
    JS_ParameterNumber(ctx, 2);
    nbytes = (ssize_t)JS_GetNumberParameter(ctx, 2);
    if (nbytes <= 0)
      JS_RETURN_NUMBER(ctx, 0);
  }
  if (n > 3)
  {
    JS_ParameterNumber(ctx, 3);
    offset = (ssize_t)JS_GetNumberParameter(ctx, 3);
    if (offset < 0)
      offset = 0;
  }
  if (offset >= sz)
    JS_RETURN_NUMBER(ctx, 0);

  if (offset + nbytes >= sz)
    nbytes = sz - offset;

  ssize_t bytes_written = write(JS_GetIntParameter(ctx, 0), (unsigned char *)buf + offset, nbytes);
  JS_RETURN_NUMBER(ctx, bytes_written);
}

void register_io_functions(void *main_loop, void *js_ctx)
{
  JS_CONTEXT ctx = (JS_CONTEXT)js_ctx;

  register_object(ctx, "_C_library",
                  "open", js_open,
                  "seek", js_seek,
                  "read", js_read,
                  "write", js_write,
                  "fsync", js_fsync,
                  "fdatasync", js_fdatasync,
                  "close", js_close,
                  "truncate", js_truncate,
                  "mkdir", js_mkdir,
                  "rmdir", js_rmdir,
                  "rename", js_rename,
                  "unlink", js_unlink,
                  "access", js_access,
                  "stat", js_stat,
                  "fstat", js_fstat,
                  "chmod", js_chmod,
                  "ftruncate", js_ftruncate,
                  "lstat", js_lstat,
                  "fchmod", js_fchmod,
                  "lchmod", js_fchmod,
                  "fchown", js_fchown,
                  "lchown", js_lchown,
                  "symlink", js_symlink,
                  "chown", js_chown,
                  "link", js_link,
                  "opendir", js_opendir,
                  "readdir", js_readdir,
                  "closedir", js_closedir,
                  "realpath", js_realpath,
                  "errno", js_errno,
                  "strerror", js_strerror,
                  NULL, NULL);
}

#endif
