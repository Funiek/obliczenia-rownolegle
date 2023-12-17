#ifndef MANDEL_H_
#define MANDEL_H_



void AllocMem(int Width,int Height);
void FreeMem(int Width,int Height);
void WritePGM(const char *fname,int Width,int Height);
void CalcImgRow(char *Row,int num,int Width,int Height);

extern char **Image; 

#endif /*MANDEL_H_*/
