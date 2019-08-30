#include "stm32f4xx.h"

int __svc(0x00) svc_service_add(int x, int y);
int __svc(0x01) svc_service_sub(int x, int y);
int __svc(0x02) svc_service_mul(int x, int y);
int __svc(0x03) svc_service_div(int x, int y);

void SVC_Handler_C(uint32_t * svc_args);

__asm  void SVC_Handler(void)
{
  TST LR, #4
  ITE EQ
  MRSEQ R0, MSP
  MRSNE R0, PSP

  B       __cpp(SVC_Handler_C)
}

void SVC_Handler_C(uint32_t * svc_args)
{
  uint32_t svc_number;
    /*
   * We can extract the SVC number from the SVC instruction. svc_args[6]
   * points to the program counter (the code executed just before the svc
   * call). We need to add an offset of -2 to get to the upper byte of
   * the SVC instruction (the immediate value).
   */
  svc_number = ((char*)svc_args[6])[-2];

  switch(svc_number)
  {
    case 0:
        svc_args[0] = svc_args[0] + svc_args[1];
    break;
    case 1:
        svc_args[0] = svc_args[0] - svc_args[1];
    break;
        case 2:
        svc_args[0] = svc_args[0] * svc_args[1];
    break;
        case 3:
        svc_args[0] = svc_args[0] / svc_args[1];
    break;
  }
}