#ifndef _K210_H_
#define _K210_H_

void K210_SendCmd(char *cmd);
float *K210_GetIPD(unsigned short timeOut);
void K210_Clear(void);

#endif
