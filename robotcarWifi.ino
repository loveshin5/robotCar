#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
Ticker ti1;

ESP8266WebServer server(80);
char ssid[]="bsnetks";
char pass[]="072355901";

//超音波感測器函數庫
#include<Ultrasonic.h>
#define tr 0 
#define ec 2 
Ultrasonic ultrasonic(tr,ec); //元件名稱

//馬達控制腳位
#define R1 12 
#define R2 13
#define L1 14
#define L2 15

 //超音波
float cm;//超音波數值
boolean x=false;

void flip1()
{
  long ms=ultrasonic.timing();//時間計算
  cm=ultrasonic.convert(ms,Ultrasonic::CM);//自動轉換公分
  Serial.println(cm);
}



void setup() {
  Serial.begin(115200);
  Serial.println();
  WiFi.begin(ssid,pass);
  ti1.attach(1,flip1);
  delay(3000);
  
  server.on("/",HTTP_POST,funon);
  
  server.begin();
  Serial.println();
  Serial.println(WiFi.localIP());

  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
 
}

void funon()
{
  String a="normal"; 
  a=server.arg("ms");
  Serial.println(a);
  String s=server.arg("t1");
  Serial.println(s);
  if(a=="normal"){
    x=false;
    if(cm>10)
    {
      if(s=="1")
      {
         forward();
         server.send(200,"text/htmlcharset=utf-8","前進");
      }
      if(s=="4")
      {
        backward();
        server.send(200,"text/htmlcharset=utf-8","後退");
      }
      if(s=="2")
      {
        turnLeft();
        server.send(200,"text/htmlcharset=utf-8","左轉");
      }
      if(s=="3")
      { 
        turnRight();
        server.send(200,"text/htmlcharset=utf-8","右轉");
      }
      if(s=="0")
      {
        carstop();
        server.send(200,"text/htmlcharset=utf-8","停止");
      }
    }
   }
  if(a=="fight")
  {
    x=true;
    server.send(200,"text/htmlcharset=utf-8","戰鬥模式");
    if(s=="1")
    {
      forward();
    }
    if(s=="4")
    {
       backward();
    }
    if(s=="2")
    {
       turnLeft();
    }
    if(s=="3")
    {
       turnRight();
    }
    if(s=="0")
    {
       carstop();
    }
  }
}

void loop() {
  if(cm<=10 && x==false)
    {
      carstop();
      delay(3000);
      backward();
      delay(2000);
      carstop();
    }
  server.handleClient();
}

void forward() 
{
 digitalWrite(R1,HIGH);
 digitalWrite(R2,LOW);
 digitalWrite(L1,HIGH);
 digitalWrite(L2,LOW); 
}
void backward() 
{
 digitalWrite(R1,LOW);
 digitalWrite(R2,HIGH);
 digitalWrite(L1,LOW);
 digitalWrite(L2,HIGH); 
}
void turnRight() 
{
 digitalWrite(R1,HIGH);
 digitalWrite(R2,LOW);
 digitalWrite(L1,LOW);
 digitalWrite(L2,LOW); 
}
void turnLeft() 
{
 digitalWrite(R1,LOW);
 digitalWrite(R2,LOW);
 digitalWrite(L1,HIGH);
 digitalWrite(L2,LOW); 
}
void carstop() 
{
 digitalWrite(R1,LOW);
 digitalWrite(R2,LOW);
 digitalWrite(L1,LOW);
 digitalWrite(L2,LOW); 
}
