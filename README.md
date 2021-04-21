## 1.Proble Statement
Read a file with RISC-V code, and convert to machine code.

## 2.Input File/Format
( 1 )This code can only convert I, R, S, Type and branch.\
( 2 )Seperate operands with ",", and there will be ":" after label.\
( 3 )Code will be stored in a text file.

## 3.Output Format
Corresponing machine code.

## 4.Input Example
<pre>
    add x2,x2,x23
    addi x24,x24,2
    bne x24,x23,L2
    sw x27,0(x10)
    beq x0,x0,L1
L2: sw x1,0(x2)
L1: addi x24,x24,1
</code></pre>

## 5.Output Example
00000001011100010000000100110011 //This is only the first instruction.


## 6.How this Program Works
( 1 )Handle the inputs.\
( 2 )Determine the type.\
( 3 )Convert to machine code according to each type.

## 7.Functions
( 1 )processR(), processS(), processLoad(), processI(), processSI(), processB(), there are functions that handle different types\
&nbsp; &nbsp; &nbsp;  of instructions.\
( 2 )decimalToBinary(), it will receive a character and compute to binary and return.   
( 3 )fillZero(), fill zero to a specific bit.\
( 4 )immFillZero(), same function as above, but it is for immediate.
