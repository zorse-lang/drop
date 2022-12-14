#ifndef BUILTINS_H
#define BUILTINS_H

#include "doops.h"
#include "js_engine.h"

#define DEFAULT_FRAMEWORK_DIR "framework"

js_object_type global_stash(JS_CONTEXT ctx);
void js_std_dump_error(JSContext *ctx);
void log_log(int level, const char *file, int line, const char *data);
void register_builtins(struct doops_loop *loop, JS_CONTEXT ctx, int argc, char *argv[], char *envp[]);
void register_global_function(JS_CONTEXT ctx, const char *func_name, js_c_function function, int nargs);
void register_object(JS_CONTEXT ctx, const char *object_name, ...);
int native_line_number(JS_CONTEXT ctx, int level, char *func_name, int max_len);
void helper_notify(JS_CONTEXT ctx, const char *object, const char *event_name);
void duk_eval_file(JS_CONTEXT ctx, const char *path, const char *directory);
void duk_run_file(JS_CONTEXT ctx, const char *path);
void js_add_path(const char *path);
void js_deinit();
JS_CONTEXT js();
struct doops_loop *js_loop();

#endif
