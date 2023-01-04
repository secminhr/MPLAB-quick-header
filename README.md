# MPLAB-quick-header
MPLAB quick header for final 

- [x] digital programmed-IO
usage:
```c
setIOMode(port, bit, IN/OUT);
write(port, bit, HIGH/LOW);
read(port, bit);
```
examples:
```c
setIOMode(B, 4, OUT); //set RB4 as output
write(C, 2, HIGH); //write RC2 as HIGH

if (read(A, 4)) {
    //RA4 is HIGH
} else {
    //RA4 is LOW
}
```

- [ ] internal interrupt INT0~2
- [ ] ADC
- [ ] Timer
- [ ] CCP
- [ ] UART
