nasm bootload.asm
dd if=/dev/zero of=floppya.img bs=512 count=2880
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
dd if=map of=floppya.img bs=512 count=1 seek=256 conv=notrunc 
dd if=config of=floppya.img bs=512 count=1 seek=258 conv=notrunc
bcc -ansi -c -o kernel.o kernel.c
bcc -ansi -c -o Math.o Math.c
#bcc -ansi -c -o DiskIO.o DiskIO.c
bcc -ansi -c -o string.o string.c
#bcc -ansi -c -o UtilityItems.o UtilityItems.c 
gcc -o loadFile loadFile.c
as86 kernel.asm -o kasm.o
ld86 -o kernel -d kernel.o kasm.o 
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=259 
./loadFile kitty1
bcc -ansi -c -o fib.o fib.c
as86 blackdos.asm -o bdos_asm.o
ld86 -o fib -d fib.o bdos_asm.o
./loadFile fib
bcc -ansi -c -o shell.o Shell.c
ld86 -o Shell -d shell.o bdos_asm.o Math.o string.o
./loadFile Shell
./loadFile Stenv
bcc -ansi -c -o t3.o t3.c
ld86 -o t3 -d t3.o bdos_asm.o
./loadFile t3
bcc -ansi -c -o cal.o cal.c
ld86 -o cal -d cal.o bdos_asm.o
./loadFile cal
