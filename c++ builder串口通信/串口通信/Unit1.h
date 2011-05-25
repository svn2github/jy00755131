//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "MSCommLib_OCX.h"
#include <ExtCtrls.hpp>
#include <OleCtrls.hpp>
#include <NMUDP.hpp>


#define FRAMELEN_SEND 128
#define FRAMELEN_REC 256
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TMSComm *MSComm1;
        TButton *Button1;
        TNMUDP *nmdp_send;
        TNMUDP *nmdp_rec;
        TButton *Button2;
        TEdit *Edit1;
        void __fastcall FormCreate(TObject *Sender);

        void __fastcall MSComm1Comm(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall nmdp_recDataReceived(TComponent *Sender,
          int NumberBytes, AnsiString FromIP, int Port);
        void __fastcall serial_DataSend(unsigned char* buff,unsigned int uiCount);
        void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        int rec_point;                      //接收字符指针，指向目前应该存储的位置起点
        int rec_length;                     //本报文还有多少字符需要接收
        int send_lenth;
        int rec_lenth;
        unsigned char rec_buf[4096];        //串口接收缓冲区
       // int    send_length;                    //串口需要发送的字符数量

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
