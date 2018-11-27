# MIPS Dynamic Instruction Decode

This is a implementation of disassembling to get a basic trace of instructions.
The program fetches instructions as tuples in hex and decodes into several statistics.
Instructions are encoded in 32 bit MIPS-I instruction format.

The input file will always be named trace.txt. The input file will always be in the same directory as the program executable before starting.

### Sample Input FILE

<PC, instruction> pairs i.e. <00401038 00134880>
```
00401038 00134880
0040103c 01364820
00401040 8d280000
00401044 15150002
00401048 22730001
0040104c 0810040e
00401038 00134880
0040103c 01364820
00401040 8d280000
00401044 15150002
00401048 22730001
0040104c 0810040e
00401038 00134880
0040103c 01364820
00401040 8d280000
00401044 15150002
00401050 ad2f0000
```

## Statistics Tracked

* Total Dynamic Instructions - **insts** : The total number of dynamic instructions from the input file. This is equivalent to the number of lines in the input FILE

* Number of R-type Instructions - **r-type** : The total number of dynamic instructions that are R-type.

* Number of I-type Instructions - **i-type** : The total number of dynamic instructions that are I-type.

* Number of J-type Instructions - **j-type** : The total number of dynamic instructions that are J-type. (Note: the sum of *r-type*, *i-type*, and *j-type* equals *insts*)

* Percentage of forward taken branches - **fwd-taken** : Percentage of dynamic instructions which are branch or jump; is taken; the next instruction executed has a target address greater than current address +4 (Note: jumps are always taken)

* Percentage of backward taken branches - **bkw-taken** : Percentage of dynamic instructions which are branch or jump; is taken; the next instruction executed has a target address less than current address +4 (Note: jumps are always taken)

* Percentage of not taken branches - **not-taken** : Percentage of dynamic instructions which are branch; is **NOT** taken; the next instruction executed has exactly current address +4 (Note: jumps cannot be not-taken, so a jump instruction will never cause this statistic to increase)

* Percentage of loads - **loads** : Percentage of dynamic instructions that are load instructions. This includes *lbu*, *lhu*, and *lw* (Note: *lui* is **NOT** a load)

* Percentage of stores - **stores** : Percentage of dynamic instructions that are store instructions.

* Percentage of arithmetic instructions - **arith** : Percentage of dynamic instructions that are **NOT** branch, jump, load, or store Instructions

* Number of register reads - **reg-<x>** : The total number of times the register *<x>* has been used as the source register operand. The number of register reads will appear in the first column after the *reg-<x>* identifier. (Note: some R-type instructions have two source register instructions)

* Number of register writes - **reg-<x>** : The total number of times the register *<x>* has been used as a destination operand. The number of register writes will appear in the second column after the *reg-<x>* identifier. (Note: some instructions use *rd field* to denote a destination register operand whereas other instructions may use the *rt field*)

### Output format

```
insts: <num-insts>
r-type: <num-r-type>
i-type: <num-i-type>
j-type: <num-j-type>
fwd-taken: <pct-fwd-taken>
bkw-taken: <pct-bkw-taken>
not-taken: <pct-not-taken>
loads: <pct-loads>
stores: <pct-stores>
arith: <pct-arithmetic>
reg-0: <num-reads> <num-writes>
reg-1: <num-reads> <num-writes>
reg-2: <num-reads> <num-writes>
reg-3: <num-reads> <num-writes>
reg-4: <num-reads> <num-writes>
reg-5: <num-reads> <num-writes>
reg-6: <num-reads> <num-writes>
reg-7: <num-reads> <num-writes>
reg-8: <num-reads> <num-writes>
reg-9: <num-reads> <num-writes>
reg-10: <num-reads> <num-writes>
reg-11: <num-reads> <num-writes>
reg-12: <num-reads> <num-writes>
reg-13: <num-reads> <num-writes>
reg-14: <num-reads> <num-writes>
reg-15: <num-reads> <num-writes>
reg-16: <num-reads> <num-writes>
reg-17: <num-reads> <num-writes>
reg-18: <num-reads> <num-writes>
reg-19: <num-reads> <num-writes>
reg-20: <num-reads> <num-writes>
reg-21: <num-reads> <num-writes>
reg-22: <num-reads> <num-writes>
reg-23: <num-reads> <num-writes>
reg-24: <num-reads> <num-writes>
reg-25: <num-reads> <num-writes>
reg-26: <num-reads> <num-writes>
reg-27: <num-reads> <num-writes>
reg-28: <num-reads> <num-writes>
reg-29: <num-reads> <num-writes>
reg-30: <num-reads> <num-writes>
reg-31: <num-reads> <num-writes>
```
