#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xe28fa882, "module_layout" },
	{ 0x48106fee, "class_destroy" },
	{ 0x3d598bde, "device_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x37d92150, "cdev_add" },
	{ 0x65f84319, "cdev_init" },
	{ 0xb10dd8d4, "device_create" },
	{ 0xcee0cc87, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xce807a25, "up_write" },
	{ 0x1e6d26a8, "strstr" },
	{ 0x85df9b6c, "strsep" },
	{ 0x8522d6bc, "strncpy_from_user" },
	{ 0x57bc19d2, "down_write" },
	{ 0x41ed3709, "get_random_bytes" },
	{ 0x92997ed8, "_printk" },
	{ 0x9c6febfc, "add_uevent_var" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x619cb7dd, "simple_read_from_buffer" },
	{ 0x37a0cba, "kfree" },
	{ 0xa916b694, "strnlen" },
	{ 0xf9c0b663, "strlcat" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x183bd601, "kmem_cache_alloc_trace" },
	{ 0xd1c97624, "kmalloc_caches" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");

