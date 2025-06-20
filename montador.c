#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void int_to_bin(int val, int bits, char *out) {
    for (int i = bits - 1; i >= 0; i--) {
        out[bits - 1 - i] = ((val >> i) & 1) + '0';
    }
    out[bits] = '\0';
}

int reg_to_int(char *reg) {
    return atoi(reg + 1); // remove o 'x' e converte
}

void instrucao_binaria(char *linha, FILE *saida) {
    char op[10], rd[10], rs1[10], rs2[10];
    int im;

    char bin[33]; // instrução binária final
    bin[32] = '\0';

    char rd_bin[6], rs1_bin[6], rs2_bin[6], funct3[4], funct7[8], opcode[8], imm_bin[13];

    // Tipo R: add, sll, or
    if (sscanf(linha, "add %[^,], %[^,], %s", rd, rs1, rs2) == 3) {
        strcpy(funct7, "0000000"); strcpy(funct3, "000"); strcpy(opcode, "0110011");
        int_to_bin(reg_to_int(rs2), 5, rs2_bin);
        int_to_bin(reg_to_int(rs1), 5, rs1_bin);
        int_to_bin(reg_to_int(rd), 5, rd_bin);
        sprintf(bin, "%s%s%s%s%s%s", funct7, rs2_bin, rs1_bin, funct3, rd_bin, opcode);
        fprintf(saida, "%s\n", bin);

    } else if (sscanf(linha, "sll %[^,], %[^,], %s", rd, rs1, rs2) == 3) {
        strcpy(funct7, "0000000"); strcpy(funct3, "001"); strcpy(opcode, "0110011");
        int_to_bin(reg_to_int(rs2), 5, rs2_bin);
        int_to_bin(reg_to_int(rs1), 5, rs1_bin);
        int_to_bin(reg_to_int(rd), 5, rd_bin);
        sprintf(bin, "%s%s%s%s%s%s", funct7, rs2_bin, rs1_bin, funct3, rd_bin, opcode);
        fprintf(saida, "%s\n", bin);

    } else if (sscanf(linha, "or %[^,], %[^,], %s", rd, rs1, rs2) == 3) {
        strcpy(funct7, "0000000"); strcpy(funct3, "110"); strcpy(opcode, "0110011");
        int_to_bin(reg_to_int(rs2), 5, rs2_bin);
        int_to_bin(reg_to_int(rs1), 5, rs1_bin);
        int_to_bin(reg_to_int(rd), 5, rd_bin);
        sprintf(bin, "%s%s%s%s%s%s", funct7, rs2_bin, rs1_bin, funct3, rd_bin, opcode);
        fprintf(saida, "%s\n", bin);

    // Tipo I: andi, addi
    } else if (sscanf(linha, "andi %[^,], %[^,], %d", rd, rs1, &im) == 3) {
        strcpy(funct3, "111"); strcpy(opcode, "0010011");
        int_to_bin(im & 0xFFF, 12, imm_bin);
        int_to_bin(reg_to_int(rs1), 5, rs1_bin);
        int_to_bin(reg_to_int(rd), 5, rd_bin);
        sprintf(bin, "%s%s%s%s%s", imm_bin, rs1_bin, funct3, rd_bin, opcode);
        fprintf(saida, "%s\n", bin);

    } else if (sscanf(linha, "addi %[^,], %[^,], %d", rd, rs1, &im) == 3) {
        strcpy(funct3, "000"); strcpy(opcode, "0010011");
        int_to_bin(im & 0xFFF, 12, imm_bin);
        int_to_bin(reg_to_int(rs1), 5, rs1_bin);
        int_to_bin(reg_to_int(rd), 5, rd_bin);
        sprintf(bin, "%s%s%s%s%s", imm_bin, rs1_bin, funct3, rd_bin, opcode);
        fprintf(saida, "%s\n", bin);

    // Tipo I: lh rd, offset(rs1)
    } else if (sscanf(linha, "lh %[^,], %d(%[^)])", rd, &im, rs1) == 3) {
        strcpy(funct3, "001"); strcpy(opcode, "0000011");
        int_to_bin(im & 0xFFF, 12, imm_bin);
        int_to_bin(reg_to_int(rs1), 5, rs1_bin);
        int_to_bin(reg_to_int(rd), 5, rd_bin);
        sprintf(bin, "%s%s%s%s%s", imm_bin, rs1_bin, funct3, rd_bin, opcode);
        fprintf(saida, "%s\n", bin);

    // Tipo S: sh rs2, offset(rs1)
    } else if (sscanf(linha, "sh %[^,], %d(%[^)])", rs2, &im, rs1) == 3) {
        strcpy(funct3, "001"); strcpy(opcode, "0100011");
        int imm_high = (im >> 5) & 0x7F;
        int imm_low  = im & 0x1F;
        char imm_high_bin[8], imm_low_bin[6];
        int_to_bin(imm_high, 7, imm_high_bin);
        int_to_bin(imm_low, 5, imm_low_bin);
        int_to_bin(reg_to_int(rs1), 5, rs1_bin);
        int_to_bin(reg_to_int(rs2), 5, rs2_bin);
        sprintf(bin, "%s%s%s%s%s%s", imm_high_bin, rs2_bin, rs1_bin, funct3, imm_low_bin, opcode);
        fprintf(saida, "%s\n", bin);

    // Tipo B: bne rs1, rs2, offset
    } else if (sscanf(linha, "bne %[^,], %[^,], %d", rs1, rs2, &im) == 3) {
        strcpy(funct3, "001"); strcpy(opcode, "1100011");
        int imm12 = (im >> 12) & 0x1;
        int imm11 = (im >> 11) & 0x1;
        int imm10_5 = (im >> 5) & 0x3F;
        int imm4_1 = (im >> 1) & 0xF;
        char i12[2], i11[2], i10_5[7], i4_1[5];
        int_to_bin(imm12, 1, i12);
        int_to_bin(imm11, 1, i11);
        int_to_bin(imm10_5, 6, i10_5);
        int_to_bin(imm4_1, 4, i4_1);
        int_to_bin(reg_to_int(rs1), 5, rs1_bin);
        int_to_bin(reg_to_int(rs2), 5, rs2_bin);
        // ordem: imm[12], imm[10:5], rs2, rs1, funct3, imm[4:1], imm[11], opcode
        sprintf(bin, "%s%s%s%s%s%s%s%s", i12, i10_5, rs2_bin, rs1_bin, funct3, i4_1, i11, opcode);
        fprintf(saida, "%s\n", bin);

    } else {
        fprintf(stderr, "Instrução inválida: %s", linha);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo.asm> [-o <saida.txt>]\n", argv[0]);
        return 1;
    }

    FILE *entrada = fopen(argv[1], "r");
    if (!entrada) {
        perror("Erro ao abrir arquivo de entrada");
        return 1;
    }

    FILE *saida = stdout;
    if (argc == 4 && strcmp(argv[2], "-o") == 0) {
        saida = fopen(argv[3], "w");
        if (!saida) {
            perror("Erro ao abrir arquivo de saída");
            fclose(entrada);
            return 1;
        }
    }

    char linha[100];
    while (fgets(linha, sizeof(linha), entrada)) {
        if (linha[0] == '\n' || linha[0] == '#') continue;
        instrucao_binaria(linha, saida);
    }

    fclose(entrada);
    if (saida != stdout) fclose(saida);

    return 0;
}
