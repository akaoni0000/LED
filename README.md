## システム概要
マイコン・・・EFM8SB10F8G-QFN24（SILICON LABS）https://jp.silabs.com/mcu/8-bit-microcontrollers/efm8-sleepy-bee/device.efm8sb10f8g-qfn24?tab=softwareandtools<br>
開発環境・・・simplicity studio（ver 5）<br>
LCD・・・I2C接続薄型16文字×2行液晶、型番：AQM1602XA-RN-GBW<br>

電圧値のAD変換(10ビット)を行い、基準電圧の値から正規の電圧値に変換したものをLCDに表示する。(4桁)

##回路図
![スクリーンショット 2024-10-21 18 15 38](https://github.com/user-attachments/assets/514111c7-dcf9-450b-8b47-c96e3231f8fb)
![スクリーンショット 2024-10-21 18 15 46](https://github.com/user-attachments/assets/869725db-dea2-4b5f-93bd-ef1a134808f8)

##AD変換に使用できるピンについて
![スクリーンショット 2024-10-21 18 19 03](https://github.com/user-attachments/assets/15477ceb-0472-402c-af9c-a6974fd962df)

## LCDとのI2C
### 初期設定
0x7Cはスレイブアドレス（書き込み）
```
void LCD_Init(){
smbus2(0x7C, 0x00, 0x38);
smbus2(0x7C, 0x00, 0x39);
smbus2(0x7C, 0x00, 0x14);
smbus2(0x7C, 0x00, 0x73);
smbus2(0x7C, 0x00, 0x56);
smbus2(0x7C, 0x00, 0x6C);
smbus2(0x7C, 0x00, 0x38);
smbus2(0x7C, 0x00, 0x01);
smbus2(0x7C, 0x00, 0x0C);
}
```
この順番でデータを送信する
### 表示方法
最初に表示する場所の指定をするデータを送信する
```
smbus2(0x7C, 0x00, 0x80);//場所の指定
```
![スクリーンショット 2024-10-07 16 42 21](https://github.com/user-attachments/assets/9263c894-141d-4711-97a1-1041e67dfb79)<br>
![スクリーンショット 2024-10-07 16 28 05](https://github.com/user-attachments/assets/e23376f1-a2cb-427f-bfc8-689cc9a6ec30)<br>
表示する文字データを送信する 文字データの場合はスレイブアドレスの後に0x40を送信する
```
//例
smbus2(0x7C, 0x40, 0x30);//文字コード 0
```
![スクリーンショット 2024-10-07 16 31 10](https://github.com/user-attachments/assets/afff2ac7-f436-4241-b805-469ca77a8095)<br>
### 参考
![スクリーンショット 2024-10-07 16 49 45](https://github.com/user-attachments/assets/c0ad82d7-908e-4bfc-bfbc-8716955338d4)

