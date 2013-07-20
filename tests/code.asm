zero:
nop            ;0000.0000
dab zero,begin
begin:
	str B 123,i  ;0001.0101 0111.1011
	not A        ;0110.0000
	shr #25			 ;1110.1110 0001.1001
	jmp begin    ;1000.1100 0000.0001
	jmp zero     ;1000.1100 0000.0000
	hlt          ;1111.1100

org 16
db 32
db 32
dab 14,15;enough
dab 1 , 2 , 3 , 4,6 ,7 ;thats it
dab zero,begin