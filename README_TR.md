# AeroPico FC: Raspberry Pi Pico Sabit Kanat Uçuş Kontrolcüsü

Bu proje, Raspberry Pi Pico üzerinde çalışan bir **sabit kanatlı uçak uçuş kontrolcüsüdür**. Şu anda proje, hobi/seviye prototip düzeyinde bir kontrol yazılımı sunar ve endüstriyel sertifikalı bir uçuş bilgisayarı olarak değerlendirilmemelidir.

## 📂 Mevcut Dosya Yapısı ve Mimari

Aşağıdaki modüller halihazırda projede çalışmaktadır:

* `src/main.cpp`
  * `setup()` ve `loop()` ile uçuş kontrol döngüsünü başlatır.
  * `FlightManager` ve `SystemManager` nesnelerini yapılandırır.
* `src/config.h`
  * Donanım pinleri, PWM sabitleri, RC kanal atamaları ve PID / uçuş parametrelerini tutar.
* `src/def.h`
  * Ana döngü süre sabiti (`LOOP_TIME`).
* `src/core/FlightManager.*`
  * Sensörleri okur, füzyonu çalıştırır ve RC girişlerini sağlar.
  * Roll / pitch / yaw durum verilerini üst katmana iletir.
* `src/core/SensorFusion.*`
  * Madgwick benzeri sensör füzyonu ile açı tahmini yapar.
  * IMU-only mod veya tam GY-87 manyetometre destekli modda çalışabilir.
* `src/core/SystemTimer.*`
  * Core1 üzerinde çalışan PID döngüsünü yönetir.
  * Outer-loop açı PID ve inner-loop oran PID kontrolünü uygular.
* `src/core/FixedWingMixer.*`
  * Sabit kanatlı uçaklar için roll/pitch/yaw miksini yapar.
  * Motor ESC ve servo sinyallerini koordine eder.
* `src/core/PID.*`
  * Genelleştirilmiş PID kontrolörü sağlar.
* `src/drivers/Sensors.*`
  * MPU6050 ve GY-87 sensörlerini okur.
  * Gyro/accel verilerini ve varsa magnetometre/barometre verilerini sağlar.
* `src/drivers/RX.*`
  * SBUS alıcısından kanalları okur ve 1000-2000 aralığına normalleştirir.
  * Alıcı geçersizse güvenli moda düşer.
* `src/drivers/Output.*`
  * ESC ve servo PWM çıkışlarını oluşturur.
  * Servoları ve motoru yazan son katmandır.
* `src/telemetry/`
  * MAVLink tabanlı telemetri ara yüzü için dosyalar içerir.
* `src/utils/`
  * Logger ve yardımcı matematiksel fonksiyonlar içerir.

## 🚀 Şu Anda Neler Var?

* Sabit kanatlı uçaklar için temel uçuş kontrol döngüsü
* SBUS RC girişi
* MPU6050 + isteğe bağlı GY-87 sensör desteği
* Madgwick tarzı sensör füzyonu
* Outer-loop angle PID + inner-loop rate PID kontrol yapısı
* Servo ve ESC PWM çıkışı
* Sabit kanatlı mixer yapısı

## ⚠️ Ne Eksik?

Bu proje hâlen prototip düzeyindedir. Aşağıdaki kritik özellikler mevcut değildir veya tam entegre edilmemiştir:

* Endüstriyel seviye güvenlik ve sertifikasyon
* Tam uçuş modu yönetimi (Stabilize, Manual, RTH vb.)
* HIL/SIL doğrulama ve kapsamlı test altyapısı
* Fail-safe, watchdog ve güç/EMI yönetimi
* Dinamik PID kalibrasyonu ve uçuş sırasında ayarlama
* GCS telemetri entegrasyonu tam olarak tamamlanmış değil
* Yedek sensör kontrolü ve hata toleransı

## 🛠 Nasıl Derlenir?

1. `platformio.ini` dosyasında doğru ortamın seçildiğinden emin olun. Bu proje için `framework = arduino` ve `board_build.core = earlephilhower` gereklidir.
2. Projeyi PlatformIO ile açın.
3. VS Code üzerindeki "Build" komutunu çalıştırın.
4. `firmware.uf2` oluşursa, Pico'yu `BOOTSEL` moduna alıp dosyayı kopyalayın.

> Not: Yerel makinenizde `pio` ya da `platformio` komutu yüklü olmalı.
> `Sensors.cpp` içinde `Wire.begin()` kullanılıyor; `Wire.begin(SDA, SCL)` çağrısı Arduino-mbed tabanlı çekirdekte desteklenmez.

## 🛠 Donanım Gereksinimleri

* Raspberry Pi Pico
* MPU6050 IMU (ivmeölçer + jiroskop)
* GY-87 modülü (isteğe bağlı): MPU6050 + HMC5883L manyetometre + BMP180 barometre
* SBUS destekli alıcı
* ESC ve fırçasız motor / servo düzeni

## Donanım Yetenekleri ve Sensörler

Bu uçuş kontrolcüsü, modüler bir yapıya sahiptir ve `config.h` üzerinden kolayca yapılandırılabilir:

* `USE_GY87` tanımı etkinse, sistem manyetometre ve barometre verilerini kullanmak üzere sensör füzyonunu genişletir.
* `USE_GY87` devre dışıysa, sistem temel IMU verilerini (ivmeölçer + jiroskop) kullanarak daha düşük işlemci yüküyle çalışır.
* Bu yapı, hem temel MPU6050 tabanlı konfigürasyonları hem de GY-87 destekli daha donanımlı kurulumları destekleyecek şekilde tasarlanmıştır.

## 📋 Gelecek Geliştirmeler

* `telemetry/` klasöründeki MAVLink desteğini aktif hale getirme
* Black box kaydı ve uçuş verisi logging
* Hata toleransı ve fail-safe mekanizmaları
* Pilotun seçebileceği uçuş modları
* Gelişmiş sensör filtrasyonu (EKF / Mahony / Kalman)

---

### 💡 Kullanıcıya Not

Bu proje, gerçek uçuş denemeleri öncesinde mutlaka kapsamlı donanım testi, kalibrasyon ve güvenlik kontrolü gerektirir. README'de yazılanlar mevcut kod üzerine göre güncellenmiştir ve proje hâlâ bir prototip seviyesindedir.

Teşekkürler / Referanslar
Bu proje, aşağıdaki açık kaynaklı çalışmalardan esinlenerek geliştirilen mimari kavramlardan ve matematiksel uygulamalardan yararlanmaktadır:

* JoshuaPeddle tarafından geliştirilen PicoFlight: Modüler yazılım mimarisi, IMU füzyon uygulaması ve düşük seviyeli donanım soyutlama mantığı nedeniyle referans alınmıştır.
 
[https://github.com/JoshuaPeddle/PicoFlight]

Developed by Muhammed Fatih Emre Özçelik/ozcelik329
Copyright © 2026 Muhammed Fatih Emre Özçelik. All rights reserved
 
