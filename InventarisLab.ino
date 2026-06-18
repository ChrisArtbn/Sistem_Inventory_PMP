extern "C" {
    #include "inventory_c++.h"
} 

static int uart_putchar(char c, FILE *stream) {
    if (c == '\n') Serial.write('\r');
    Serial.write(c);
    return 0;
}

static int uart_getchar(FILE *stream) {
    while (!Serial.available());
    int c = Serial.read();
    Serial.write(c); 
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