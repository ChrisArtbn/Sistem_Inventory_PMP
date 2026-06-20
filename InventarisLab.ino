extern "C" {
    #include "sistem_inventory.h"
} 

static int uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        Serial.println();
    } else {
        Serial.print(c);
    }
    return 0;
}

static int uart_getchar(FILE *stream) {
    while (!Serial.available()); 
    int c = Serial.read();
    
    if (c == '\n' || c == '\r') {
        Serial.println(); 
    } else {
        Serial.print((char)c);
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

    Serial.println(F("\n\nSistem Booting Selesai. Memori Siap."));
    
    mulai_program();
}

void loop() {
}