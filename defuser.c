#include "defuser.h"

#include <inttypes.h>
#include <stdio.h>

#define MAX_FIELDS 8

/*
 * 把 values 中的 count 个整数写成一行“空格分隔的无符号十进制整数”，存入 out。
 * 成功返回 0；count 为 0（还没有实现）、snprintf 出错或结果被截断时返回非零。
 */
static int write_answer(char *out, size_t cap, const uint32_t *values, size_t count) {
    size_t used = 0;
    for (size_t i = 0; i < count; ++i) {
        int size = snprintf(out + used, cap - used, "%s%" PRIu32, i ? " " : "", values[i]);
        if (size < 0 || (size_t)size >= cap - used) return 1;
        used += (size_t)size;
    }
    return count == 0;
}

int solve_phase_1(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 1;
    uint32_t hi = (uint32_t)(challenge >> 32);
    uint32_t mid = (uint32_t)(challenge >> 16);
    uint32_t lo = (uint32_t)challenge;
    uint32_t step1 = 3u * (hi - mid) + 0x63d0u;
    uint32_t step0 = lo * 2u + (hi ^ 0x473fu);
    values[0] = (step0 + step1 * 4u) ^ 0x1570u;
    // DONE: 根据 phase_01 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    return write_answer(out, cap, values, count);
}

int solve_phase_2(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 6;
    uint32_t first = (uint32_t)((challenge >> 9) & 0x3fu);
    values[0] = first + 0x1bu;
    for (uint32_t i = 1; i < 6u; ++i) {
        uint32_t term = (uint32_t)((challenge >> (i + 12u)) & 0x3fu);
        term += i;
        term ^= values[i - 1u];
        values[i] = term * 5u;
    }
    // DONE: 根据 phase_02 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    return write_answer(out, cap, values, count);
}

int solve_phase_3(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 2;
    uint32_t index = (uint32_t)(challenge & 7u);
    uint32_t x = (uint32_t)(challenge >> 32) ^ (uint32_t)(challenge >> 3);
    uint32_t result = 0;

    switch (index) {
    case 0:
        result = (2u * x) ^ 0x96c8u;
        break;
    case 1:
        result = (x ^ 0x1bau) + (x >> 1);
        break;
    case 2:
        result = (2u * x) ^ 0x718fu;
        break;
    case 3:
        result = 3u * x + 0x600eu;
        break;
    case 4:
        result = 5u * x + 0x1cfb6u;
        break;
    case 5:
        result = 3u * x + 0xfbefu;
        break;
    case 6:
        result = (x ^ 0x9315u) + (x >> 2);
        break;
    case 7:
        result = 5u * x + 0x45c59u;
        break;
    }
    values[0] = index;
    values[1] = result;
    // DONE: 根据 phase_03 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    return write_answer(out, cap, values, count);
}

static uint32_t p4_walk(uint32_t target, uint32_t low,
                        uint32_t high, uint32_t salt) {
    uint32_t mid = low + ((high - low) >> 1);

    if (target == mid) {
        return (mid + 0x85u) ^ salt;
    }

    uint32_t path = mid + salt;
    if (target < mid) {
        uint32_t result = p4_walk(target, low, mid - 1u, salt);
        return (result * 5u ^ path) ^ 0x13u;
    }

    uint32_t result = p4_walk(target, mid + 1u, high, salt);
    return (result * 5u ^ path) ^ 0x1du;
}

int solve_phase_4(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 1;
    uint32_t target = (uint32_t)(challenge & 0x7fu);
    uint32_t salt = (uint32_t)(challenge >> 7) ^
                (uint32_t)(challenge >> 32);

    values[0] = p4_walk(target, 0u, 127u, salt);
    // DONE: 根据 phase_04 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    return write_answer(out, cap, values, count);
}

typedef struct P5Node {
    const struct P5Node *next;
    uint32_t value;
    uint32_t tag;
} P5Node;

static const P5Node p5_n0;
static const P5Node p5_n1;
static const P5Node p5_n2;
static const P5Node p5_n3;
static const P5Node p5_n4;
static const P5Node p5_n5;
static const P5Node p5_n6;
static const P5Node p5_n7;
static const P5Node p5_n8;
static const P5Node p5_n9;
static const P5Node p5_n10;
static const P5Node p5_n11;

static const P5Node p5_n0  = {&p5_n1,  0x0679u, 0x7857u};
static const P5Node p5_n1  = {&p5_n6,  0x0949u, 0xd523u};
static const P5Node p5_n2  = {&p5_n7,  0x0471u, 0x08cdu};
static const P5Node p5_n3  = {&p5_n4,  0x087eu, 0x00f9u};
static const P5Node p5_n4  = {&p5_n0,  0x021eu, 0x5db4u};
static const P5Node p5_n5  = {0,       0x0536u, 0x4714u};
static const P5Node p5_n6  = {&p5_n9,  0x0b14u, 0x21d6u};
static const P5Node p5_n7  = {&p5_n3,  0x031au, 0x5c47u};
static const P5Node p5_n8  = {&p5_n10, 0x0a72u, 0x5946u};
static const P5Node p5_n9  = {&p5_n11, 0x073du, 0x6236u};
static const P5Node p5_n10 = {&p5_n2,  0x006fu, 0x3d69u};
static const P5Node p5_n11 = {&p5_n5,  0x015au, 0x1f0bu};

static const P5Node *const p5_heads[8] = {
    &p5_n8, &p5_n10, &p5_n2, &p5_n7,
    &p5_n3, &p5_n4, &p5_n0, &p5_n1
};

int solve_phase_5(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 2;
    const P5Node *node = p5_heads[(uint32_t)challenge & 7u];
    uint32_t acc = (uint32_t)(challenge >> 6) ^
                   (uint32_t)(challenge >> 32) ^
                   node->tag ^ 0x3625u;
    acc += node->value;

    uint32_t steps = (uint32_t)(challenge >> 3) & 3u;
    for (uint32_t i = 0; i < steps; ++i) {
        node = node->next;
        acc ^= node->tag;
        acc += node->value;
    }

    values[0] = acc;
    values[1] = node->value;
    // DONE: 根据 phase_05 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    return write_answer(out, cap, values, count);
}

static uint32_t p6_op0(uint32_t x) {
    return (x ^ 0x7ad2u) + 4u * x;
}

static uint32_t p6_op1(uint32_t x) {
    return ((x << 5) + 0x6d1cu) ^ (5u * x);
}

static uint32_t p6_op2(uint32_t x) {
    return ((x << 1) | (x >> 31)) ^ 0x78b8u;
}

static uint32_t p6_op3(uint32_t x) {
    return (x >> 2) | (x << 30);
}

static uint32_t p6_op4(uint32_t x) {
    return (x ^ (x >> 1)) + 0xe3d2u;
}

static uint32_t p6_op5(uint32_t x) {
    return (x + 0x836fu) ^ (x >> 3);
}

static uint32_t (*const p6_ops[8])(uint32_t) = {
    p6_op0, p6_op1, p6_op4, p6_op2,
    p6_op2, p6_op5, p6_op1, p6_op3
};

int solve_phase_6(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 1;
    uint32_t mix = (uint32_t)(challenge >> 6) ^
               (uint32_t)(challenge >> 32);
    uint32_t first_op = (uint32_t)challenge & 7u;
    uint32_t second_op = (uint32_t)(challenge >> 3) & 7u;

    uint32_t result = p6_ops[first_op](mix);
    result ^= 0xc596u;
    values[0] = p6_ops[second_op](result) + 0xbf1du;
    // DONE: 根据 phase_06 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    return write_answer(out, cap, values, count);
}

int solve_secret(uint64_t challenge, uint64_t context, char *out, size_t cap) {
    uint32_t c_mix = (uint32_t)(challenge >> 32) ^ (uint32_t)challenge;
    uint32_t t_mix = (uint32_t)(context >> 32) ^ (uint32_t)context;
    uint32_t selector = (uint32_t)context & 3u;

    uint32_t mix_input = c_mix ^ t_mix;
    uint32_t mixed = ((mix_input >> 1) ^ 0x9fa0u) +
                 (mix_input ^ 0x82d3u);
    uint32_t second = mixed ^
                  (selector * 0x82d3u) ^
                  (uint32_t)(context >> 16);
    uint32_t delta = t_mix + selector + 0x9fa0u + (c_mix >> 1);
    uint32_t third = second ^ delta;

    int n = snprintf(out, cap,
                 "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXOPEN%u %u %u",
                 (unsigned int)selector,
                 (unsigned int)second,
                 (unsigned int)third);
    return n < 0 || (size_t)n >= cap ? 1 : 0;
    // DONE（选做）: 根据 secret_gate、secret_copy 和 secret_phase 的汇编构造完整的第七行，写入 out 后返回 0。
    // 第七行不一定只有整数，可以直接用 snprintf 写入；还没完成时保持返回 1，测试会显示 not attempted。
}
