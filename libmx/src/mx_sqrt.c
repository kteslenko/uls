int mx_sqrt(int x) {
    int sqrt = 1;

    while (x / sqrt > sqrt) {
        sqrt++;
    }

    if (x / sqrt == sqrt
        && x % sqrt == 0) {
        return sqrt;
    }

    return 0;
}
