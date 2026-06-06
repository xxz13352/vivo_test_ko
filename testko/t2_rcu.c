// t2_rcu: 带 modversions + 显式 import __rcu_read_lock/__rcu_read_unlock(我补的符号 + RT驱动同款)。
// 目的: 触发与 RT@gnmtl 相同的符号解析路径(__rcu_read_lock 经我的 ksymtab 补丁 resolve),
//       看是否复现 load_module+0x2d14 的 NULL deref。
// 用 rcu_read_lock()/rcu_read_unlock() 宏 —— 在 PREEMPT_RCU=y 的编译树会生成对 __rcu_read_lock
//   的外部调用(与 RT 驱动一致); 若编译树 PREEMPT_RCU=n 则内联, 那 t2 就退化成 t1(届时 modverify 会显示)。
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/rcupdate.h>

static int __init t2_init(void)
{
	rcu_read_lock();
	rcu_read_unlock();
	printk(KERN_INFO "t2_rcu loaded\n");
	return 0;
}

static void __exit t2_exit(void)
{
}

module_init(t2_init);
module_exit(t2_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("t2_rcu: modversions + __rcu_read_lock import");
