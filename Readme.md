## libft

Started as my very first school project. I kept adding functions that i needed for different project.
Many functions follow the school code style guidelines and the allowed functions for diffrent school projects, many don't.


When calling the make file call it with 'make NDEBUG=-NDEBUG' to disable debug mode

I have designed the libray for my 64bit PML4(64 byte cache lines) linux system.
Somethimes I tried to make it portable but I simply have to much fun with low level optimizations so it some functions are probably broken.
For example the functions 'ft_assert' I used for school projects or the the hash map implementation are broken for diffrent systems.

Incompatible system example duo to PML5 (only examples, not complete, https://en.wikipedia.org/wiki/Intel_5-level_paging):
Intel Ice Lake (Laptop Intel Core 10th gen, some Xeons)
AMD EPYC 9004/8004
AMD Storm peak Ryzen Threadripper PRO 7900WX


