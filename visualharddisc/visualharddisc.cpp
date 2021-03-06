#include <iostream>
#include <stdio.h>
#include "stdafx.h"
//#include "zpcfsys.h"
typedef	unsigned char	byte;
typedef	unsigned short	zjie;
typedef	unsigned short	zchr;
typedef	unsigned int	word;

#define SectorBytes	512
#define DirSector	248
#define DatSector	278


FILE*	vdiskfp;

/**************************************************
* 读一个扇区的数据，并转换成大头zjie结构。
* buf: 数据缓冲区
* adr: 要读的扇区号，从磁盘开始
**************************************************/
void readOneClstr(byte* buf, int adr) {
	fseek(vdiskfp, adr*SectorBytes, SEEK_SET);
	fread(buf, 1, SectorBytes, vdiskfp);
}
/**************************************************
* 写一个扇区的数据，并转换成大头zjie结构。
* buf: 数据缓冲区
* adr: 要写的扇区号，从磁盘开始
**************************************************/
int writeOneClstr(byte* buf, int adr) {
	fseek(vdiskfp, adr*SectorBytes, SEEK_SET);
	return fwrite(buf, 1, SectorBytes, vdiskfp);
}

int main(int argc, char** argv)
{
	int	i, j, k, size, dirsct = 248;
	zjie	first;
	byte	buf[SectorBytes];
	byte	fnam[30] = "string.txt", nam[30] = "STRING  TXT ";

	vdiskfp = fopen("D:\\vdisc\\vdc.vhd", "rb");

	readOneClstr(buf, 248);
	for (i = 0; i<SectorBytes; i++) {
		if ((i & 15) == 0)printf("\n");
		printf("%02X ", buf[i]);
	}
	printf("\n");
	for (j = 0; j<SectorBytes / 32; j++) {
		for (i = 0; i<11; i++) {
			if (buf[j * 32 + i] != nam[i])break;
		}
		if (i == 11)break;
	}
	if (j == SectorBytes / 32) {
		printf("No file.");
	}

	first = buf[j * 32 + 26] + buf[j * 32 + 27] * 256;
	size = buf[j * 32 + 28]
		+ (buf[j * 32 + 29] << 8)
		+ (buf[j * 32 + 30] << 16)
		+ (buf[j * 32 + 31] << 24);
	printf("first= %d, size= %d\n", first, size);
	for (i = 0; i<32; i++) {
		printf("%02X ", buf[j * 32 + i]);
	}
	printf("\n=========================\n\n");

	readOneClstr(buf, DatSector + first);
	for (i = 0; i<SectorBytes; i++) {
		printf("%c", buf[i]);
	}

	return 0;
}

