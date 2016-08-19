target remote localhost:1234
break main
ignore 2 4850
skip kprintf
skip cs_enter
skip cs_leave
skip runq_debug
skip do_random_stuff
skip runq_choose
skip runq_remove
skip callout_setup
skip mdelay
continue
