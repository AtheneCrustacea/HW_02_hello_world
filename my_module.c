#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define MAX_BUF_SIZE_IN_BYTES 128

static char my_str[MAX_BUF_SIZE_IN_BYTES];
static unsigned int idx;
static char ch_val;

///////////////////////// idx

static int idx_set(const char *val, const struct kernel_param *kp) {
  unsigned int received_val;
  int ret = kstrtoint(val, 10, &received_val);
  if (ret) {
    pr_err("kstrtoint error\n");
    return ret;
  }
  if (received_val >= MAX_BUF_SIZE_IN_BYTES) {
    pr_err("idx_set out of bound\n");
  } else {
    idx = received_val;
  }
  pr_info("idx value = %d\n", idx);
  return 0;
}

static const struct kernel_param_ops idx_ops = {
    .set = idx_set,
};

module_param_cb(idx, &idx_ops, &idx,
                0664); // Nothing bad ever happens from some reading, right?
MODULE_PARM_DESC(
    idx, "Parameter to pass position of the next symbol to write with "
         "'ch_val'. Accepted range [0, 128].\n"); // TODO: format string to use
                                                  // 'MAX_BUF_SIZE_IN_BYTES'
/////////////////////////ch_val

static int ch_val_set(const char *val, const struct kernel_param *kp) {
  ch_val = *val; // TODO: add some utf-8 symbol validation
  pr_info("ch_val value = %c\n", ch_val);
  my_str[idx] = ch_val;
  return 0;
}

static const struct kernel_param_ops ch_val_ops = {
    .set = ch_val_set,
};

module_param_cb(ch_val, &ch_val_ops, &ch_val, 0664);
MODULE_PARM_DESC(ch_val, "UTF-8 symbol to write into parameter 'my_str'.");

///////////////////////// my_str

static int my_str_get(char *val, const struct kernel_param *kp) {
  int ret = sprintf(val, "%s\n", my_str);
  memset(my_str, 0, MAX_BUF_SIZE_IN_BYTES);
  return ret;
}

static const struct kernel_param_ops my_str_ops = {
    .get = my_str_get,
};

module_param_cb(my_str, &my_str_ops, &my_str, 0444); // No write allowed here
MODULE_PARM_DESC(
    my_str,
    "Output string, result of writing 'ch_val' by index "
    "'idx'. Warning: Zeroized after reading!"); // Could've made flag of new
                                                // writing session after reading
                                                // and find max index to zeroize after
                                                // that. To lazy to implement + this works just fine

///////////////////////// init/exit
static int __init hello_init(void) {
  pr_info("init\n");
  return 0;
}

static void __exit hello_exit(void) {
  pr_info("exit\n");
  return;
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("H8SM Sergey Molodyk");
MODULE_DESCRIPTION("Hello world simple module");