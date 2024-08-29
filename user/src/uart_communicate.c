#include "uart_communicate.h"


void uart_communicate_init(uart_communicate* uc)
{
	uc->HEAD_FLAG = 0;
	uc->RX_FLAG = 0;
	uc->Data_len = 2;
	uc->Data_Yesense[0] = 0x59;
	uc->Data_Yesense[1] = 0x53;
	uc->res = 0;
	uc->BodyX = 0;
	uc->BodyY = 0;
	uc->BodyZ = 0;
	uc->flagAveage = 0;
	uc->j = 0;
	uc->angleAveage = 0;
	uc->angleXA = 0;
//	uc->write=0;
//	uc->get_head=0;
//	uc->huart=huart;
}

/*�������*/
float yaw_last = 0, yaw_now = 0;
int n = 0;
void G_output_infoSet()
{
	yaw_last = yaw_now;
	yaw_now = g_output_info.yaw;
	if (yaw_now - yaw_last < -100) // ������ͻ��
		n += 1;
	else if (yaw_now - yaw_last > 100)
		n -= 1;
	g_output_info.yaw += n * 360;
}


void uart_communicate_receive(uart_communicate* uc, uint8_t c)
{if (uc->RX_FLAG == 1)
		{
			uc->Data_len++;
			uc->Data_Yesense[uc->Data_len - 1] = c;
			if (uc->Data_len > 97) // ������Χ
			{
				uc->Data_len = 2;
				uc->RX_FLAG = 0;
			}
		}

		if (uc->HEAD_FLAG == 1)
		{
			if (c == 0x53) // ֡ͷ2
			{
				uc->Data_len -= 2;
				uc->res = analysis_data(uc->Data_Yesense, uc->Data_len);
				uc->Data_len = 2;
				uc->RX_FLAG = 1;
				uc->HEAD_FLAG = 0;
				if (uc->res == 0 || uc->res == 1)
					G_output_infoSet();
			}
			else if (c != 0x59)		
				uc->HEAD_FLAG = 0;
		}
		
		if (c == 0x59) // ֡ͷ1
		{
			uc->HEAD_FLAG = 1;
		}
}
//void uart_communicate_receive(uart_communicate* uc, uint16_t c)
//{
//	if (uc->RX_FLAG == 1) // �ж��Ƿ����ڽ�������
//  {
//    uc->Data_len++; // �������ݳ��ȼ���
//    uc->Data_Yesense[uc->Data_len - 1] = c; // �洢���յ�������
//    if (uc->Data_len > 97) // ������ݳ����Ƿ񳬳���������Χ
//    {
//      uc->Data_len = 2; // �������ݳ���
//      uc->RX_FLAG = 0; // ��������
//    }
//	}
//	if (uc->HEAD_FLAG == 1)
//	{
//		if (c == 0x53) // ֡ͷ2
//		{
//			uc->Data_len -= 2;
//			uc->res = analysis_data(uc->Data_Yesense, uc->Data_len);
//			uc->Data_len = 2;
//			uc->RX_FLAG = 1;
//			uc->HEAD_FLAG = 0;
//			if (uc->res == 0 || uc->res == 1)
//				G_output_infoSet();
//		}
//		else if (c != 0x59)		
//			uc->HEAD_FLAG = 0;
//	}
//	if (c == 0x59) // ֡ͷ1
//		uc->HEAD_FLAG = 1;
//}
