nasm bootload.asm
dd if=/dev/zero of=floppya.img bs=512 count=2880
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
dd if=map of=floppya.img bs=512 count=1 seek=256 conv=notrunc 
dd if=config of=floppya.img bs=512 count=1 seek=258 conv=notrunc
bcc -ansi -c -o kernel.o kernel.c
bcc -ansi -c -o malloc.o malloc.c
bcc -ansi -c -o Math.o Math.c
bcc -ansi -c -o	UtilityItems.o UtilityItems.c
bcc -ansi -c -o DiskIO.o DiskIO.c
bcc -ansi -c -o	String.o string.c
gcc -o loadFile loadFile.c
as86 kernel.asm -o kasm.o
ld86 -o kernel -d kernel.o kasm.o malloc.o Math.o UtilityItems.o  DiskIO.o String.o 
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=259 
#./loadFile kitty1
./loadFile kitty2
bcc -ansi -c -o fib.o fib.c
as86 blackdos.asm -o bdos_asm.o
ld86 -o fib -d fib.o bdos_asm.o
./loadFile fib