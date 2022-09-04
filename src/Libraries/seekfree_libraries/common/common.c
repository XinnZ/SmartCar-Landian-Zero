#include "Cpu0_Main.h"
#include "IfxScuEru.h"
#include "SysSe/Bsp/Bsp.h"
#include "TC377_config.h"
#include "zf_uart.h"
#include "common.h"

IFX_ALIGN(4) IfxCpu_syncEvent g_cpuSyncEvent = 0;       //>�¼�ͬ������
IFX_ALIGN(4) IfxCpu_mutexLock g_mutexLockIMAGE = 0;     //>ͼ��������
IFX_ALIGN(4) IfxCpu_mutexLock g_mutexLockMOTOR = 0;     //>���ƴ�������

App_Cpu0 g_AppCpu0; //Ƶ����Ϣ����
void MCU_Init(void)
{
	disableInterrupts();
	//>���ÿ��Ź�
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());


    //>��ȡʱ��Ƶ�� ���ڲ鿴��ǰϵͳ����Ƶ��
    g_AppCpu0.info.pllFreq = IfxScuCcu_getPllFrequency();
    g_AppCpu0.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreIndex());
    g_AppCpu0.info.sysFreq = IfxScuCcu_getSpbFrequency();
    g_AppCpu0.info.stmFreq = IfxStm_getFrequency(&MODULE_STM0);
#if(PRINTF_ENABLE)
    uart_init(DEBUG_UART, DEBUG_UART_BAUD, DEBUG_UART_TX_PIN, DEBUG_UART_RX_PIN);
#endif
}
