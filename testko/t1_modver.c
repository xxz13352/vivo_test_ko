// t1_modver: 空模块, 但带 modversions(编译时不去 CONFIG_MODVERSIONS)。
// 与 t0 唯一差别 = 是否走 modversions 路径。目的: 隔离"modversions/__versions 处理"是否致崩。
// 调用 printk(内核基础导出符号), 确保有一个 __versions 条目 + 一个 CRC 校验, 模拟真实模块。
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init t1_init(void)
{
	printk(KERN_INFO "t1_modver loaded\n");
	return 0;
}

static void __exit t1_exit(void)
{
}

module_init(t1_init);
module_exit(t1_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("t1_modver: modversions enabled, imports printk only");
