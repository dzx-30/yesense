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

/*处理航向角*/
float yaw_last = 0, yaw_now = 0;
int n = 0;
void G_output_infoSet()
{
	yaw_last = yaw_now;
	yaw_now = g_output_info.yaw;
	if (yaw_now - yaw_last < -100) // 发生了突变
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
			if (uc->Data_len > 97) // 超出范围
			{
				uc->Data_len = 2;
				uc->RX_FLAG = 0;
			}
		}

		if (uc->HEAD_FLAG == 1)
		{
			if (c == 0x53) // 帧头2
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
		
		if (c == 0x59) // 帧头1
		{
			uc->HEAD_FLAG = 1;
		}
}
//void uart_communicate_receive(uart_communicate* uc, uint16_t c)
//{
//	if (uc->RX_FLAG == 1) // 判断是否正在接收数据
//  {
//    uc->Data_len++; // 增加数据长度计数
//    uc->Data_Yesense[uc->Data_len - 1] = c; // 存储接收到的数据
//    if (uc->Data_len > 97) // 检查数据长度是否超出缓冲区范围
//    {
//      uc->Data_len = 2; // 重置数据长度
//      uc->RX_FLAG = 0; // 结束接收
//    }
//	}
//	if (uc->HEAD_FLAG == 1)
//	{
//		if (c == 0x53) // 帧头2
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
//	if (c == 0x59) // 帧头1
//		uc->HEAD_FLAG = 1;
//}
