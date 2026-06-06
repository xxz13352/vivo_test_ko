# testko —— Y52s insmod 崩溃隔离测试

真机 insmod 外挂驱动 `4.14.141.sh`(RT@gnmtl)崩在 `load_module+0x2d14`(NULL deref @ `mod+0x140`,
`simplify_symbols` 之后)。二分查找符号修正已生效(不再 `Unknown symbol`),但驱动是**畸形 ELF**
(`.modinfo` 声称 `modversions` 但 `__versions` 节 size=0)。本目录用 GitHub Actions 交叉编译 3 个
极小 aarch64 ko,真机逐个 insmod **二分定位崩溃归属**。

## 三变体

| ko | 特征 | 测什么 |
|---|---|---|
| `t0_empty_nomodver.ko` | 空模块, **去** modversions, 不 import 外部符号 | 最纯加载链 scaffold |
| `t1_modver.ko` | 空模块(调 printk), **带** modversions | modversions/`__versions` 处理路径 |
| `t2_rcu.ko` | modversions + import `__rcu_read_lock` | 我的 ksymtab 补丁 + RT 同款符号路径 |

## 怎么跑(用户本机无工具链 → GitHub Actions)

1. 把 **本仓库**(只需 `testko/` + `.github/`,`.gitignore` 已屏蔽工作区大文件)推到一个 GitHub 仓库:
   ```
   cd C:\Users\mi\Desktop\y52sap
   git init && git add .gitignore testko .github
   git commit -m "testko: isolate Y52s insmod crash"
   git branch -M main
   git remote add origin https://github.com/<你的用户名>/<仓库名>.git
   git push -u origin main
   ```
2. GitHub 仓库 → Actions 页 → 选 `build-testko` → Run workflow。
3. 跑完下载 artifact `testko-aarch64-4.14.141`,解压把 `.ko` 放到工作区 `testko/out/`。
4. 告诉我,我用 adb 逐个 insmod + 抓 pstore 二分。

## 真机二分判读(我来做)

- **t0 崩** → 加载链 scaffold/我的内核补丁本身有问题(与 modversions/符号无关)。
- **t0 过、t1 崩** → modversions/`__versions` 处理路径致崩(很可能与 RT 空 `__versions` 同根)。
- **t1 过、t2 崩** → `__rcu_read_lock` 符号 / 我的 ksymtab 补丁交互致崩。
- **全过** → 崩溃是 RT 驱动**畸形 ELF 独有**(空 `__versions` + 其它),非通用问题 → 转向修驱动 ELF 或换驱动。

## 注意

- vermagic/CRC **不会**匹配 Y52s(与 RT 驱动同样情况)—— 无妨,加载靠 `boot_y52_insmodfix.img` 的补丁放行。
- 真机测试前需先刷 `out/boot_y52_insmodfix.img`(已含 insmod_fix 二分查找修正版)。
- Actions 的 inspect 步骤会打印每个 ko 的 vermagic/`__versions`大小/UND 符号 —— 这些日志我也要看(判断 t2 是否真 import 了 `__rcu_read_lock`)。
