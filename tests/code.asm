zero:
nop            ;0000.0000
begin:
	str B 123,i  ;0001.0101 0111.1011
	not A        ;0110.0000
	shr #25			 ;1110.1110 0001.1001
	jmp begin    ;1000.1100 0000.0001
	jmp zero     ;1000.1100 0000.0000
	hlt          ;1111.1100