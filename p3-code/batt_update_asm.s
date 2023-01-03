.file	"batt_update.c"
.text
.global  set_batt_from_ports
        
## ENTRY POINT FOR REQUIRED FUNCTION
set_batt_from_ports:
        ## takes one argument, pointer to a batt_t
        ## %rdi is pointer

        ## Arg1 %rdi batt_t

        movw BATT_VOLTAGE_PORT(%rip), %dx # copy global variable to register dx 
        movb BATT_STATUS_PORT(%rip), %cl # copy global variable to register cl

        cmpw $0, %dx # if (BATT_VOLTAGE_PORT < 0)
        jl .BOUND_ERROR

        movw %dx, 0(%rdi) # set batt->mlvolts
        sarw $1, 0(%rdi)# batt->mlvolts = BATT_VOLTAGE_PORT / 2
        movw 0(%rdi), %ax #to calcult percent, ax = batt->mlvolts
        subw $3000, %ax
        sarw $3, %ax #int percent = (batt->mlvolts - 3000) / 8

        cmpw $100, %ax #if (percent >= 100)
        jge .FULL_BATT
        cmpw $0, %ax #else if (percent < 0)
        jl .ZERO_BATT
        movb %al, 2(%rdi)
        jmp .SET_MODE

.BOUND_ERROR:
        movl $1, %eax    ##return 1                                           
        ret

.FULL_BATT:
        movb $100, 2(%rdi) #batt->percent = 100
        jmp .SET_MODE

.ZERO_BATT:
        movb $0, 2(%rdi) #batt->percent = 0
        jmp .SET_MODE

.SET_MODE:
        testb $16, BATT_STATUS_PORT(%rip) ##if (BATT_STATUS_PORT & 1 << 4)
        jz .SET_VOLT
        movb $1, 3(%rdi) #batt->mode = 1
        movl $0, %eax #return 0
        ret

.SET_VOLT:
        movb $2, 3(%rdi) ##batt->mode = 2
        movl $0, %eax #return 0
        ret
### Change to definint semi-global variables used with the next function 
### via the '.data' directive
.data
my_int:
        .int 0x00
my_array:                       # declare multiple ints sequentially starting at location
        .int 0b0111111                   # 'my_array' for an array. Each are spaced 4 bytes from the
        .int 0b0000110            # next and can be given values using the same prefixes as 
        .int 0b1011011            # are understood by gcc.
        .int 0b1001111
        .int 0b1100110
        .int 0b1101101
        .int 0b1111101
        .int 0b0000111
        .int 0b1111111
        .int 0b1101111

## WARNING: Don't forget to switch back to .text as below
## Otherwise you may get weird permission errors when executing 
.text
.global  set_display_from_batt

## ENTRY POINT FOR REQUIRED FUNCTION
set_display_from_batt:  
        ## assembly instructions here

        #r9d: batt.mlvolt
        #r10d: display
        #r11d: batt.mode
        #r12d: right
        #r13d: middle
        #r14d: left
        #r15d: batt.percent
	## two useful techniques for this problem
        movl    my_int(%rip),%ebx    # load my_int into register ebx
        leaq    my_array(%rip),%r8  # load pointer to beginning of my_array into r8

        movq %rdi, %r9 ##set batt.mlvolt
        andq $0xFFFF, %r9 ##initialize

        movq %rdi, %r15 ##set batt.percent
        shrq $16, %r15 ##right shift 16 
        andq $0xFF, %r15
        
        movq %rdi, %r11 ##set batt.mode
        shrq $24, %r11 ##right shift 24 
        andq $0xFF, %r11

        cmpb $2, %r11b ##if (batt.mode == 2)
        je .VOLT_MODE
        jne .PERCENT_MODE

.VOLT_MODE:

        addl $5, %r9d ##batt.mlvolts + 5 to round up the lowest digit
        ## calculate right, middle and lefts
        
        movl %r9d, %eax ##left (batt.mlvolts / 1000) % 10
        cqto
        movl $1000, %r14d
        idivl %r14d
        movl %eax, %r14d

        movl %edx, %eax ##middle (batt.mlvolts / 100) % 10
        cqto
        movl $100, %r13d
        idivl %r13d
        movl %eax, %r13d
        
        movl %edx, %eax ##right ((batt.mlvolts + 5) / 10) % 10
        cqto
        movl $10, %r12d
        idivl %r12d
        movl %eax, %r12d

        movl (%r8, %r12, 4), %r12d ##*display |= masks[right] << 3;
        sall $3, %r12d
        orl %r12d, %ebx

        movl (%r8, %r13, 4), %r13d ##*display |= masks[middle] << 10;
        sall $10, %r13d
        orl %r13d, %ebx

        movl (%r8, %r14, 4), %r14d ##*display |= masks[left] << 17;
        sall $17, %r14d
        orl %r14d, %ebx

        movl $1, %r14d ##v
        shll $1, %r14d
        orl %r14d, %ebx

        movl $1, %r14d ##decimal
        shll $2, %r14d
        orl %r14d, %ebx

        jmp .BATT_BAR

.PERCENT_MODE:

        movl $1, %r14d ##%
        shll $0, %r14d
        orl %r14d, %ebx

        movl %r15d, %eax ##left
        cqto
        movl $100, %r14d
        idivl %r14d
        movl %eax, %r14d

        movl %edx, %eax ##middle
        cqto
        movl $10, %r13d
        idivl %r13d
        movl %eax, %r13d

        movl %edx, %r12d ##right

        movl (%r8, %r12, 4), %r12d ##*display |= masks[right] << 3;
        sall $3, %r12d
        orl %r12d, %ebx

        cmpl $0, %r14d ## if (left == 0)
        jne .HAS_LEFT ## check whether left or middle is 0. then not show on display screen
        cmpl $0, %r13d
        jne .HAS_MIDDLE
        jmp .BATT_BAR

.HAS_LEFT:
        movl (%r8, %r14, 4), %r14d ##*display |= masks[left] << 17;
        sall $17, %r14d
        orl %r14d, %ebx

        movl (%r8, %r13, 4), %r13d ##*display |= masks[middle] << 10;
        sall $10, %r13d
        orl %r13d, %ebx

        jmp .BATT_BAR

.HAS_MIDDLE:
        movl (%r8, %r13, 4), %r13d ##*display |= masks[middle] << 10;
        sall $10, %r13d
        orl %r13d, %ebx
        jmp .BATT_BAR

.BATT_BAR:
        cmpl $5, %r15d ##compare the percent with 5, 30, 50, 70, 90, 100
        jl .LESS_THAN_5
        cmpl $30, %r15d
        jl .LESS_THAN_30
        cmpl $50, %r15d
        jl .LESS_THAN_50
        cmpl $70, %r15d
        jl .LESS_THAN_70
        cmpl $90, %r15d
        jl .LESS_THAN_90
        cmpl $100, %r15d
        jle .LESS_THAN_100

.LESS_THAN_5:
        movl %ebx, (%rsi)
        movl $0, %eax ##no bar shows and return 0
        ret
.LESS_THAN_30:
        movq $1, %r10
        shlq $24, %r10
        orl %r10d, %ebx

        movl %ebx, (%rsi)
        movl $0, %eax
        ret
.LESS_THAN_50:
        movq $1, %r10
        shlq $24, %r10
        orl %r10d, %ebx

        movq $1, %r10
        shlq $25, %r10
        orl %r10d, %ebx
        
        movl %ebx, (%rsi)
        movl $0, %eax
        ret
.LESS_THAN_70:
        movq $1, %r10
        shlq $24, %r10
        orl %r10d, %ebx

        movq $1, %r10
        shlq $25, %r10
        orl %r10d, %ebx

        movq $1, %r10
        shlq $26, %r10
        orl %r10d, %ebx
        
        movl %ebx, (%rsi)
        movl $0, %eax
        ret
.LESS_THAN_90:
        movq $1, %r10
        shlq $24, %r10
        orl %r10d, %ebx

        movq $1, %r10
        shlq $25, %r10
        orl %r10d, %ebx

        movq $1, %r10
        shlq $26, %r10
        orl %r10d, %ebx

        movq $1, %r10
        shlq $27, %r10
        orl %r10d, %ebx
        
        movl %ebx, (%rsi)
        movl $0, %eax
        ret
.LESS_THAN_100:
        movq $1, %r10
        shlq $24, %r10
        orl %r10d, %ebx

        movq $1, %r10
        shlq $25, %r10
        orl %r10d, %ebx

        movq $1, %r10
        shlq $26, %r10
        orl %r10d, %ebx

        movq $1, %r10
        shlq $27, %r10
        orl %r10d, %ebx

        movq $1, %r10
        shlq $28, %r10
        orl %r10d, %ebx
        
        movl %ebx, (%rsi)
        movl $0, %eax
        ret

.text
.global batt_update
        
## ENTRY POINT FOR REQUIRED FUNCTION
batt_update:
	## assembly instructions here
        pushq %rdi ##push onto the stack
        leaq 0(%rsp), %rdi ##move pointer to the stack
        call set_batt_from_ports #call set_batt_from_ports
        cmpl $0, %eax ##if (set_batt_from_ports(&batt) != 0)
        jne .ERROR 
        movl (%rsp), %edi ##batt struct
        leaq 0(%rsp), %rsi #pointer to top
        call set_display_from_batt ##set_display_from_batt(batt, &BATT_DISPLAY_PORT)
        popq %rdi
        movl %edi, BATT_DISPLAY_PORT(%rip) ##pointer to the display
        movl $0, %eax ##return 0
        ret

.ERROR:
        popq %rdi #pop the stack out
        movl $1, %eax ##return 1
        ret
        





