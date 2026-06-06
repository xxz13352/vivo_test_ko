// t0_empty: 最纯基线模块 —— init 直接 return 0, 不 import 任何外部符号, 编译时去 modversions。
// 目的: 验证 Y52s 加载"最干净的 ko"能否不崩(测加载链 scaffold 本身)。
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init t0_init(void)
{
	return 0;
}

static void __exit t0_exit(void)
{
}

module_init(t0_init);
module_exit(t0_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("t0_empty: pure baseline, no external symbols");
