extern "C" {
    #include "inventory_final.h"
} 

// Jembatan agar printf bisa tampil sempurna di Serial Monitor
static int uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        Serial.println(); // Biarkan core Arduino yang menangani enter secara native
    } else {
        Serial.print(c);
    }
    return 0;
}

// Jembatan agar scanf bisa membaca ketikan dan merespon Enter
static int uart_getchar(FILE *stream) {
    while (!Serial.available()); 
    int c = Serial.read();
    
    // Supaya saat mengetik dan menekan Enter, kursor layar otomatis pindah baris
    if (c == '\n' || c == '\r') {
        Serial.println(); 
    } else {
        Serial.print((char)c); // Tampilkan ketikan (echo) ke layar
    }
    return c;
}

static FILE uartout = {0};

void setup() {
    Serial.begin(9600);
    while (!Serial); 

    fdev_setup_stream(&uartout, uart_putchar, uart_getchar, _FDEV_SETUP_RW);
    stdout = &uartout;
    stdin = &uartout;

    // MENGGUNAKAN F() AGAR TEKS DISIMPAN DI FLASH MEMORY
    Serial.println(F("\n\nSistem Booting Selesai. Memori Siap."));
    
    mulaiProgram();
}

void loop() {
}