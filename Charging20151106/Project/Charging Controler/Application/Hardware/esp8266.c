/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include<string.h>
#include "drv_usart.h"
#include "global_value.h"
#include "self_buffer.h"
#include "user_lib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "esp8266.h"
#include "protocol.h"

/* Synchronisation */
xSemaphoreHandle UsartTimeoutSem;

/**
* @brief  �����ַ���
* @param  char *s, char *t ;��s�в���t
* @retval s_temp(t��s�е�λ��)�ɹ�     0 ��ʧ�� ��
*/
char *mystrstr(char *s, char *t)
{
  char    *s_temp;        /*the s_temp point to the s*/
  char    *m_temp;        /*the mv_tmp used to move in the loop*/
  char    *t_temp;        /*point to the pattern string*/

  if (NULL == s || NULL == t) return NULL;

  /*s_temp point to the s string*/
  for (s_temp = s; *s_temp != '\0'; s_temp++)
  {
    /*the move_tmp used for pattern loop*/
    m_temp = s_temp;
    /*the pattern string loop from head every time*/
    for (t_temp = t; *t_temp == *m_temp; t_temp++, m_temp++);
    /*if at the tail of the pattern string return s_tmp*/
    if (*t_temp == '\0') return s_temp;

  }
  return NULL;
}

s8 Esp8266PowerOn( void )
{
  return 0;
}

/**
* @brief  ESP8266����
* @param  None
* @retval 1--�ظ��ǶԵģ�0--�ظ��Ǵ����
 * @note
*/
s8 Esp8266Reset( void )
{
  return 0;
}




/**
* @brief  �жϽ��������Ƿ�Ϊ��Ҫ�Ļظ�
* @param  None
* @retval 1--�ظ��ǶԵģ�0--�ظ��Ǵ����
* @note   ����Sim5218WaitResponse()�еڶ����βξ����ظ����ݣ�����ظ�������ȷ����ô�ͷ��ź���Sim5218CallBack��
Sim5218WaitResponse()�У�������1�����򷵻�0
*/
s8 Esp8266Init( void )
{

  /*����Զ�̷�����*/
  g_server_ip[0] = 192;
  g_server_ip[1] = 168;
  g_server_ip[2] = 0;
  g_server_ip[3] = 105;

  g_server_port = 6800;

  return 0;
}



/**
* @brief  �������ڽ��յ���������
* @param  ���ڽ����ַ���
* @retval None
* @note   ���ڽ����ַ���ΪGPS��Ϣ����GPS������1����Ϊ������Ϣ����SMS������1��
���ܺ�����Ҫ���CREG:0��2�����������
*/
void Esp8266PromptProtocol(char *Buffer,uint8 len)
{
   FrameExtraction(Buffer,len,0);
}


/**
* @brief  ���ڽ�������
* @param  None
* @retval None
* @note   ���ȼ���ߡ��������ڽ��ն�֡�ź������ص������ź��������ȵȴ����ڽ�����һ֡(�����յ��������ַ����3ms������һ֡)���ͷŵ��ź�����
�����յ������ݣ����Ƿ���ģ�������з��͸����ڵ�ָ��ظ������ǻظ�������������ûص�����ת��ģ��������Ϊģ����յ���(����ţ�gps)����ô
�������ݿ���ʲô���ݣ���ô�ͷ�sche�ź���������Ϊflag��ֵ��ת��ģ������ȥִ�С�
*/
void WifiParseTask(void * pvParameters)
{
  char auch_buf[MAX_REV_LEN];
  
  Usart4RevBufInit();

  InitQueue();

  Usart4Init();

  vSemaphoreCreateBinary(UsartTimeoutSem);

  for( ;; )
  {
    if(xSemaphoreTake(UsartTimeoutSem, portMAX_DELAY) == pdTRUE)
    {
      memset(auch_buf, 0, MAX_REV_LEN);
      
      memcpy(auch_buf, g_rev_usart1.p_parse, g_rev_usart1.uch_parse_len);

      g_rev_usart1.b_parse_busy = false;
      
      if(mystrstr(auch_buf,"StationMac") != NULL)
      {
           printf("%s\r\n",auch_buf);
      }
      else if(mystrstr(auch_buf,"TCP close") != NULL)
      {
           printf("TCP close\r\n");
      }
      else if(mystrstr(auch_buf,"TCP success") != NULL)
      {
           printf("TCP success\r\n");
      }
      else if(mystrstr(auch_buf,"WIFI success") != NULL)
      {
           printf("WIFI success\r\n");
      }
      else if(mystrstr(auch_buf,"WIFI close") != NULL)
      {
           printf("WIFI close\r\n");
      }
      
      Esp8266PromptProtocol(auch_buf,g_rev_usart1.uch_parse_len);
    }
  }
}

