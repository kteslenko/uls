static int hex_digit_to_dec(char digit) {
    if (digit >= 48 && digit <= 57) {
        return digit - 48;
    }
    if (digit >= 97 && digit <= 102) {
        digit -= 32;
    }
    if (digit >= 65 && digit <= 70) {
        return digit - 55;
    }
    return -1;
}

unsigned long mx_hex_to_nbr(const char *hex) {
    unsigned long nbr = 0;

    if (hex == 0) {
        return 0;
    }

    for (int i = 0; hex[i] != '\0'; i++) {
        int digit = hex_digit_to_dec(hex[i]);

        if (digit == -1) {
            return 0;
        }

        nbr *= 16;
        nbr += digit;
    }

    return nbr;
}
