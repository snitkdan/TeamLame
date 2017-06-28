//  software delay
void delay(unsigned long *aPtr)
{
    int aValue = *aPtr;
    volatile unsigned long i = 0;
    volatile unsigned int j = 0;
    for (i = aValue; i > 0; i--)
    {
        for (j = 0; j < 100000; j++);
    }
    return;
}
