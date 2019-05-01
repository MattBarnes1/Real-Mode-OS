#define PRINTS(x)  interrupt(33,0,x,0,0)
#define PRINTN(x)  interrupt(33,13,x,0,0)
#define LPRINTS(x) interrupt(33,0,x,1,0)
#define LPRINTN(x) interrupt(33,13,x,1,0)
#define SCANS(x)   interrupt(33,1,x,0,0)
#define SCANN(x)   interrupt(33,14,&x,0,0)
#define END        interrupt(33,5,0,0,0)
#define CLRSCRN()  interrupt(33,12,0x100,0,0)
#define BOOT() interrupt(33,11,0,0,0)
#define RM(FileName) interrupt(33, 7, &FileName,0,0)
#define ERROR(
//#define READFILE(FileName) interrupt(33, 
#define exec(FileName)
