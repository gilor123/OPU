.entry LABEL0
LABEL1:	add $15,$27,$31
	sub $3,$23,$19
	and $2,$5,$13
	or $23,$5,$30
	nor $21,$23,$25
LABEL2:	move $10,$20
	mvhi $3,$13
	mvlo $21,$22
LABEL3:	addi $4,-44,$23
	subi $31,564,$12
	andi $12,157,$21
.entry LABEL4
	ori $1,-234,$23
	nori $13,324,$2
	blt $3,$22,LABEL2
.extern LABEL5
LABEL6:	beq $23,$10,LABEL3
LABEL4:	bne $21,$22,LABEL3
	bgt $9,$8,LABEL2
LABEL7:	lb $4,345,$7
	sb $15,231,$2
	lw $30,-332,$31
	sw $12,345,$3
	lh $31,3423,$3
	jmp LABEL5
	sh $12,-23,$23
LABEL8:	jmp $2
	jmp LABEL1
	la LABEL3
	call LABEL1
LABEL0:	.db 1,34,76,24
	.dh 23,45,75,32
	.dw 21314,32423,5645,675
STR: 	.asciz "Gil Or"
END:	stop
