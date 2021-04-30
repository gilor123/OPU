


/*Sets operations function*/

void read_set (pset setPointer, int numbers[]) {

    int i=0;
    int mask;

    setPointer->veryLow = 0;
    setPointer->low = 0;
    setPointer->High = 0;
    setPointer->veryHigh = 0;

    while (numbers[i]!=(-1)) {
        mask = (1<<(numbers[i]%32));

        if (numbers[i]<(8*sizeof(int))) {
            setPointer->veryLow = ((setPointer->veryLow)|mask);
        }
        if (numbers[i]>=(8*sizeof(int)) && numbers[i]<(16*sizeof(int))) {
            setPointer->low = ((setPointer->low)|mask);
        }
        if (numbers[i]>=(16*sizeof(int)) && numbers[i]<(24*sizeof(int))) {
            setPointer->High = ((setPointer->High)|mask);
        }
        if (numbers[i]>=(24*sizeof(int)) && numbers[i]<(32*sizeof(int))) {
            setPointer->veryHigh = ((setPointer->veryHigh)|mask);
        }
        numbers[i]=0;
        i++;
    }
}

void print_set (pset setPointer) {

    int i=0;
    int mask;
    int valuesCounter=0;

    while (i<(32* sizeof(int))) {
        mask = (1<<i%32);
        if (i<(8*sizeof(int))) {
            if (((setPointer->veryLow) | mask) == setPointer->veryLow) {
                printf("%d   ",i);
                valuesCounter++;
            }
        }
        if (i>=(8*sizeof(int)) && i<(16*sizeof(int))) {
            if (((setPointer->low) | mask) == setPointer->low) {
                printf("%d   ",i);
                valuesCounter++;
            }
        }
        if (i>=(16*sizeof(int)) && i<(24*sizeof(int))) {
            if (((setPointer->High) | mask) == setPointer->High) {
                printf("%d   ",i);
                valuesCounter++;
            }

        }
        if (i>=(24*sizeof(int)) && i<(32*sizeof(int))) {
            if (((setPointer->veryHigh) | mask) == setPointer->veryHigh) {
                printf("%d   ",i);
                valuesCounter++;
            }
        }
        if (valuesCounter == 16) {
            printf("\n");
        }
        i++;
    }
    if (valuesCounter == 0)
        printf("The set is empty");
}

void union_set (pset set1, pset set2, pset set3) {

    set3->veryLow = ((set1->veryLow) | (set2->veryLow));
    set3->low= ((set1->low) | (set2->low));
    set3->High = ((set1->High) | (set2->High));
    set3->veryHigh = ((set1->veryHigh) | (set2->veryHigh));

}

void intersect_set (pset set1, pset set2, pset set3) {

    set3->veryLow = ((set1->veryLow) & (set2->veryLow));
    set3->low= ((set1->low) & (set2->low));
    set3->High = ((set1->High) & (set2->High));
    set3->veryHigh = ((set1->veryHigh) & (set2->veryHigh));

}

void sub_set (pset set1, pset set2, pset set3) {

    int i=0;
    int mask;

    set3->veryLow = 0;
    set3->low = 0;
    set3->High = 0;
    set3->veryHigh = 0;

    while (i<(32*sizeof(int))) {
        mask = (1<<i%32);
        if (i<(8*sizeof(int))) {
            if (((set1->veryLow & mask) == mask) && ((set2->veryLow & mask) != mask)) {
                set3->veryLow = (set3->veryLow | mask);
            }
        }
        if (i>=(8*sizeof(int)) && i<(16*sizeof(int))) {
            if (((set1->low & mask) == mask) && ((set2->low & mask) != mask)) {
                set3->low = (set3->low | mask);
            }
        }
        if (i>=(16*sizeof(int)) && i<(24*sizeof(int))) {
            if (((set1->High & mask) == mask) && ((set2->High & mask) != mask)) {
                set3->High = (set3->High | mask);

            }
        }
        if (i>=(24*sizeof(int)) && i<(32*sizeof(int))) {
            if (((set1->veryHigh & mask) == mask) && ((set2->veryHigh & mask) != mask)) {
                set3->veryHigh = (set3->veryHigh | mask);
            }
        }
        i++;
    }
}

void symdiff_set (pset set1, pset set2, pset set3){

    set3->veryLow = ((set1->veryLow) ^ (set2->veryLow));
    set3->low= ((set1->low) ^ (set2->low));
    set3->High = ((set1->High) ^ (set2->High));
    set3->veryHigh = ((set1->veryHigh) ^ (set2->veryHigh));

}

