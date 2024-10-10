#include <SI_EFM8SB1_Register_Enums.h>                  // SI_SFR declarations
#include "InitDevice.h"

//SDA:0.3
//SCL:0.4

sbit pin05 = P0^5;
int send_data[3];
int ms;
int i;
float reference_voltage;//基準電圧
float result_voltage;//測定結果の電圧
int integer_part;//整数部分
float fractional_part;//小数部分
float sum_voltage;//測定結果の電圧の合計値
float result;//AD変換値を電圧にした値
int digit;
uint8_t slave;
uint8_t send_data1;
uint8_t send_data2;
uint8_t max_count;
uint8_t receive_data_first;
uint8_t receive_data_second;
uint32_t ADC_receive_data;

void delay_ms (ms);
void smbus0 (slave);//スレイブアドレスを送るだけ、読み込み用
void smbus1 (slave,send_data1);//スレイブアドレスに加えて1つデータを送る
void smbus2 (slave,send_data1, send_data2);//スレイブアドレスに加えて2つデータを送る
void LCD_Init(void);//LCDの設定をする
void LCD_Data(void);//LCDにデータを送る
void LCD_Integer_Data(integer);//整数をLCDに表示
void ADC_Init(void);//AD変換の設定をする
void ADC_Data(void);//AD変換を実行

void SiLabs_Startup (void) {}
int main (void)
{
	enter_DefaultMode_from_RESET();
	enter_Mode2_from_DefaultMode();
	LCD_Init();
	ADC_Init();

	REF0CN = 0x08; //基準電圧を変更できる
	reference_voltage = 3.3;//基準電圧の値 3.3v

	while(1){
	   //smbus2(0x7C, 0x00, 0x0C);//ディスプレイオン
	   //smbus2(0x7C, 0x00, 0x31);
     //smbus2(0x7C, 0x00, 0x70);


	   ADC_Data();//AD変換を実行
	   smbus2(0x7C, 0x00, 0x80);//場所の指定

	   integer_part = (int)result;//AD変換結果の整数部分
	   LCD_Integer_Data(integer_part);//整数部分を出力
	   smbus2(0x7C, 0x40, 0x2E);//小数点を出力

	   fractional_part = result-integer_part;
	   for(i=1;i<=3;i++){
	       fractional_part*= 10;
	       digit = (int)fractional_part;
	       LCD_Integer_Data(digit);//整数部分を出力
	       fractional_part-= digit;
	   }
	   //smbus2(0x7C, 0x00, 0x08);//ディスプレイオン
	   //delay_ms(1000);
	}
}

void smbus0 (slave)
{
  send_data[0] = slave;
  max_count = 0;
  SMB0CN0_STA = 1;
  delay_ms(10);
}

void smbus1 (slave, send_data1)
{
  send_data[0] = slave;
  send_data[1] = send_data1;
  max_count = 1;
  SMB0CN0_STA = 1;
  delay_ms(10);
}

void smbus2 (slave, send_data1, send_data2)
{
  send_data[0] = slave;
  send_data[1] = send_data1;
  send_data[2] = send_data2;
  max_count = 2;
  SMB0CN0_STA = 1;
  delay_ms(10);
}

void LCD_Init(){
//  smbus2(0x7C, 0x00, 0x38);
//  smbus2(0x7C, 0x00, 0x39);
//  smbus2(0x7C, 0x00, 0x14);
//  smbus2(0x7C, 0x00, 0x53);
//  smbus2(0x7C, 0x00, 0x73);
//  smbus2(0x7C, 0x00, 0x6C);
//  smbus2(0x7C, 0x00, 0x0C);//ディスプレイオン
//  smbus2(0x7C, 0x00, 0x01);

  smbus2(0x7C, 0x00, 0x38);
  smbus2(0x7C, 0x00, 0x39);
  smbus2(0x7C, 0x00, 0x14);
  smbus2(0x7C, 0x00, 0x73);
  smbus2(0x7C, 0x00, 0x56);
  smbus2(0x7C, 0x00, 0x6C);
  smbus2(0x7C, 0x00, 0x38);
  smbus2(0x7C, 0x00, 0x01);
  smbus2(0x7C, 0x00, 0x0C);//ディスプレイオン
  //smbus2(0x7C, 0x00, 0x01);
}

void LCD_Data(){
  smbus2(0x7C, 0x00, 0x80);//場所の指定
  for(i = 7; i >= 0; i--) {
      char receive_data_bit =(receive_data_first>>i) & 1;
      if(receive_data_bit == 0){
          smbus2(0x7C, 0x40, 0x30);//文字コード 0
      }
      else{
          smbus2(0x7C, 0x40, 0x31);//文字コード 1
      }
  }

  for(i = 7; i >= 0; i--) {
      char receive_data_bit =(receive_data_second>>i) & 1;
      if(receive_data_bit == 0){
          smbus2(0x7C, 0x40, 0x30);//文字コード
      }
      else{
          smbus2(0x7C, 0x40, 0x31);//文字コード
      }
   }
}

void LCD_Integer_Data(integer){
  switch (integer){
    case 0:
      smbus2(0x7C, 0x40, 0x30);
      break;
    case 1:
      smbus2(0x7C, 0x40, 0x31);
      break;
    case 2:
      smbus2(0x7C, 0x40, 0x32);
      break;
    case 3:
      smbus2(0x7C, 0x40, 0x33);
      break;
    case 4:
      smbus2(0x7C, 0x40, 0x34);
      break;
    case 5:
      smbus2(0x7C, 0x40, 0x35);
      break;
    case 6:
      smbus2(0x7C, 0x40, 0x36);
      break;
    case 7:
      smbus2(0x7C, 0x40, 0x37);
      break;
    case 8:
      smbus2(0x7C, 0x40, 0x38);
      break;
    case 9:
      smbus2(0x7C, 0x40, 0x39);
      break;
  }
}

void ADC_Init(){
   //ピン設定
   ADC0MX = ADC0MX_ADC0MX__ADC0P12;

   //ビットモード設定
   ADC0CF = ADC0CF_AD8BE__NORMAL | ADC0CF_ADGN__GAIN_1 | (0x02 << ADC0CF_ADSC__SHIFT)
       | ADC0CF_ADTM__TRACK_NORMAL;

   //AD変換有効設定
   ADC0CN0 = 0x80;
}

void ADC_Data(){
  sum_voltage = 0;
  for(i=1;i<=1;i++){
     ADC0CN0_ADBUSY=1;//AD変換開始
     while(ADC0CN0_ADBUSY){}//AD変換が終わるまでまつ
     ADC_receive_data = ADC0;//AD変換の結果を代入
     result_voltage = reference_voltage*ADC_receive_data/1023;//電圧値にする
     sum_voltage+=result_voltage;
  }

  result = sum_voltage/1;
}

void delay_ms (ms)
{
   TCON &= ~0x30;                      // Stop Timer0; Clear TCON_TF0
   TMOD &= ~0x0f;                      // 16-bit free run mode
   TMOD |=  0x01;

   CKCON0 |= 0x04;                      // Timer0 counts SYSCLKs

   while (ms) {
      TCON_TR0 = 0;                         // Stop Timer0
      TH0 = ((-24500000/1000) >> 8);     // Overflow in 1ms
      TL0 = ((-24500000/1000) & 0xFF);
      TCON_TF0 = 0;                         // Clear overflow indicator
      TCON_TR0 = 1;                         // Start Timer0
      while (!TCON_TF0);                    // Wait for overflow
      ms--;                            // Update ms counter
   }

   TCON_TR0 = 0;                            // Stop Timer0
}
