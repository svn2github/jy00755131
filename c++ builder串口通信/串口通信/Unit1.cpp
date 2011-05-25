//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "MSCommLib_OCX"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{

      MSComm1->InBufferCount = 0;
      MSComm1->OutBufferCount = 0;
      MSComm1->InputMode =1;              //二进制模式

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TForm1::MSComm1Comm(TObject *Sender)
{
    int i,lenIn;
    OleVariant str;
    AnsiString txx;
    char buff[256];
    rec_lenth = FRAMELEN_REC;
    if(MSComm1->CommEvent == 2)
    {
            lenIn = MSComm1->InBufferCount;   //接收缓冲区
            if(lenIn > 0)
            {
                str = MSComm1->Input;     //返回和删除接收缓冲区中的字符
                for(i=0;i<lenIn;i++)
                {
                    rec_buf[rec_point + i] = str.GetElement(i);
                   // if(rec_lenth>0)
                    //{
                    //    rec_length --;
                   // }
                    //else return;
                }

            for(i=0;i<lenIn;i++) buff[i] =0x0;
            memmove(buff,rec_buf,lenIn);
           // rec_point = rec_point + lenIn;
           // for(int j = 0; j<lenIn; j++ )
                this->nmdp_send->SendBuffer(buff,i,lenIn);
            }
    }

}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
      if(!MSComm1->PortOpen)MSComm1->PortOpen =true;
      else MSComm1->PortOpen = false;
      this->Edit1->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::nmdp_recDataReceived(TComponent *Sender,
      int NumberBytes, AnsiString FromIP, int Port)
{
    Char udp_buf[256];
    int lenbuf = sizeof(udp_buf);
    int iLen = 0;
    Form1->nmdp_rec->ReadBuffer(udp_buf,lenbuf,iLen);
    for(int i = 0; i<iLen; i++ )
    this->Edit1->Text = Edit1->Text + udp_buf[i];
    serial_DataSend(udp_buf,iLen);

}
//---------------------------------------------------------------------------
void __fastcall TForm1::serial_DataSend(unsigned char* buff,unsigned int uiCount)
{
    int i;
    char send_buf[256];
    //Char buff[256];
    OleVariant send_v;
    //int send_length;

    send_v = VarArrayCreate(OPENARRAY(int,(0,uiCount)),varByte);
    send_v.ArrayRedim(uiCount-1);           //循环从0开始，因此减1
    memmove(send_buf,buff,uiCount);
    for(i=0;i<uiCount;i++)
    {
        send_v.PutElement(send_buf[i],i);       //将需要发送的元素放进缓冲
    }
    MSComm1->Output = send_v;                   //通知控件开始发送

}

void __fastcall TForm1::Button2Click(TObject *Sender)
{
      if(MSComm1->PortOpen) MSComm1->PortOpen =false;
}
//---------------------------------------------------------------------------



