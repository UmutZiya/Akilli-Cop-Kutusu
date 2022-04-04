#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <FirebaseArduino.h>

const char *ssid = "umutziya";                 // bağlanılacak olan kablosuz ağın SSID si
const char *password = "123456789";           // bağlanılacak olan kablosuz ağın şifresi

#define echo D5
#define trig D6
#define firebase_erisim_linki "akilli-cop-kutusu-bda0f-default-rtdb.firebaseio.com"   // oluşturduğunuz projenizdeki veritabanınızın erişim linki
#define firebase_sifre "N7FXmjJ2r42z1NJ57bRffNqwHpvfXI4diFJ7IUO3"    // veritabanı secret key

int sayac = 0;

Servo motor;

void setup() {
  
  Serial.begin(9600);                       //Seri iletişim hızı 9600 bps olarak ayarlanarak başlatılıyor.
  delay(1000);
  
  WiFi.mode(WIFI_STA);                        //ESP8266 istasyon moduna alınıyor. 
  WiFi.begin(ssid, password);                 //SSID ve şifre girilierek wifi başlatılıyor.

  //------- Wifi ağına bağlanıncaya kadar beklenilen kısım ------------
  Serial.print("Bağlantı kuruluyor");
  while (WiFi.status() != WL_CONNECTED) 
  { // Wifi bağlantısı sağlanana kadar bekleniyor.
    delay(500);                               // Bu arada her 500 ms de bir seri ekrana yan yana noktalar yazdırılarak
    Serial.print(".");                        //görsellik sağlanıyor.
  }
  Serial.println("");                         //Bir alt satıra geçiliyor.
  Serial.println("Bağlantı sağlandı...");      //seri ekrana bağlantının kurulduğu bilgisi gönderiliyor.

  Serial.print("Alınan IP addresi: ");        // kablosuz ağdan alınan IP adresi
  Serial.println(WiFi.localIP());             // kablosuz ağdan alınan IP adresi
  
  Firebase.begin(firebase_erisim_linki, firebase_sifre);      // Firebase erişimini başlat
  
  pinMode(echo,INPUT);
  pinMode(trig,OUTPUT);
  motor.attach(D1);
  motor.write(0);
}

void loop() {

  
  digitalWrite(trig, LOW); 
  delayMicroseconds(5);
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trig, LOW);  
float  zaman = pulseIn(echo, HIGH);  
 float  mesafe= (zaman /29.1)/2;    
  Serial.print("Uzaklik ");  
  Serial.print(mesafe);
  Serial.println(" cm");  
  delay(500); 

  if(mesafe < 10){ 
    motor.write(90);
    delay(5000);
    motor.write(0);
    sayac=sayac+1;
    Serial.print(sayac);
    Serial.println(" kez açıldı.");

    Firebase.setInt("Kapagin acilip kapanma sayisi", sayac);  // "Kapagin acilip kapanma sayisi" etiketinin altına sayac yaz
    
    if (Firebase.failed()) {                              // yazma hatası oldu mu?
      Serial.print("Hata Var:");                        // evet olmuş, seri porttan bana bildir
      Serial.println(Firebase.error());  
      return;                                           // loop başına dön
  }
  delay(1000); 
    
  }

}
