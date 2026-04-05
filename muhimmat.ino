//MUHİMMAT DEPOSU SAVUNMA SİSTEMİ

#include<Wire.h> //ekran kütüphanesi
#include<LiquidCrystal_I2C.h> //ekran kütüphanesi
#include<Servo.h> //servo kütüphanesi

LiquidCrystal_I2C lcd(0x27, 16, 2); // pin olarak arduıona sda ya a4 ve scl ye a5 i kullanıcaz bu sabit bir şey
Servo savasServosu; 

const int servoo = 5;
const int mesafeTrig=6; // hscr mesafe sensörü
const int mesafeEcho=7; // hscr mesafe sensörü
const int buzzer=8;
const int kirmiziLed=9;
const int yesilLed=10;


void setup() 
{
  lcd.init();          // Ekranı başlatır (Uyan komutu)
  lcd.backlight();     // Ekranın arka ışığını yakar 
  savasServosu.attach(servoo); // servonun output deme dili
  pinMode(buzzer,OUTPUT);
  pinMode(yesilLed,OUTPUT);
  pinMode(kirmiziLed,OUTPUT);
  pinMode(mesafeTrig,OUTPUT); // Ses dalgasını bu pinden göndereceğiz
  pinMode(mesafeEcho,INPUT);  // Yansıyan ses bu pinden geri gelecek

}

void loop() 
{
  // 1. Sesi fırlat (Trig)
  digitalWrite(mesafeTrig, LOW); //hazırlanma
  delayMicroseconds(2);
  digitalWrite(mesafeTrig, HIGH); //10mikrosaniye bağırma trig 10 msn karşı tarafa ses sinyali gönderir
  delayMicroseconds(10);
  digitalWrite(mesafeTrig, LOW); // bağırmayı keser ses sinyali durur
  long sure = pulseIn(mesafeEcho, HIGH); // yankının yani sesin gelme süresini ölçüyor
  int mesafe = (sure / 2) / 29.1; /*ölçülen sesi mesafeye çevirir. Sesin havada 1 santimetre yol alması tam olarak 29.1 mikrosaniye
  1cm 29.1 mikrosaniye ise x cm i kaç sure de alır  diye orantı kurduk*/

  if(mesafe>0 && mesafe<20) //düşman 0 ile 20cm aralığında mühimata çok yaklaşmış
  {
    digitalWrite(yesilLed, LOW); //yeşil led yanar
    digitalWrite(kirmiziLed,HIGH); //kırmızı led yanar
    digitalWrite(buzzer,HIGH); //buzer öter
    delay(500); //yarım saniye bekler
    digitalWrite(buzzer,LOW); //buzer öter
    lcd.clear();         // Önce ekranı temizle ki eski yazılar karışmasın
    lcd.setCursor(0, 0); // imleci 1. sütunun 1. satırına al
    lcd.print("DUSMAN GORULDU!");
    savasServosu.write(90);  // Tetiği çek
    delay(300);        // Mekanizmanın hareket süresi
    savasServosu.write(0);   // ateşş
    digitalWrite(buzzer,HIGH); //mermi sesi
    delay(150);
    digitalWrite(buzzer,LOW); //düşman öldü
  }
  else
  {
    savasServosu.write(0); //siah beklemede
    digitalWrite(kirmiziLed, LOW);
    digitalWrite(buzzer,LOW); // ses yok
    digitalWrite(yesilLed,HIGH); //etraf güvende ışığı
    lcd.setCursor(0, 0); // Birinci satırın başına git
    lcd.print("ASAYIS BERKEMAL"); // ekrana yazdırdık
  }
  

  
}