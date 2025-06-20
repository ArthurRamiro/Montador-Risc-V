# Instruções do tipo R (registradores)
add x1, x2, x3
sll x4, x5, x6
or  x7, x8, x9

# Instruções do tipo I (imediato)
andi x10, x11, 15
addi x12, x13, 20

# Instrução tipo I (load - lh)
lh x14, 32(x15)

# Instrução tipo S (store - sh)
sh x16, 48(x17)

# Instrução tipo B (branch - bne)
bne x18, x19, 64
