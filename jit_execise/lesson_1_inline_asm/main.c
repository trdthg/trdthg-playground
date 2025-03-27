#include <stdio.h>

int add_int(int a, int b)
{
    int result;

#ifdef __riscv
    asm volatile(
        "add %[res], %[val1], %[val2]"
        : [res] "=r"(result)
        : [val1] "r"(a), [val2] "r"(b));
    result += 1;
#elif defined(__x86_64__)
    asm volatile(
        "mov %[val1], %%eax\n\t" // Move a to eax
        "add %[val2], %%eax\n\t" // Add b to eax
        "mov %%eax, %[res]"      // Move result from eax to result
        : [res] "=r"(result)
        : [val1] "r"(a), [val2] "r"(b)
        : "%eax" // Clobber eax
    );
    result += 2;
#elif defined(__arm__)
    asm volatile(
        "add %[res], %[val1], %[val2]"
        : [res] "=r"(result)
        : [val1] "r"(a), [val2] "r"(b));
    result += 3;
#else
// #error 是 C 和 C++ 预处理器指令中的一种，用于在编译期间生成自定义错误消息。当编译器遇到 #error 指令时，它会停止编译并输出后面的错误消息。
#error "Unsupported architecture"
#endif

    return result;
}

float riscv_add_float(float a, float b)
{
    float result;
#ifdef __riscv
    asm volatile(
        "fadd.s %[res], %[val1], %[val2]"
        : [res] "=f"(result)
        : [val1] "f"(a), [val2] "f"(b));
#endif
    return result;
}

int main()
{
    int a = 3;
    asm(
        "jnz to_here%=\n\t"
        "jz to_there%=\n\t"
        "to_here%=:\n\t"
        "to_there%=:" ::"r"(a));

    float x = 5.5f;
    float y = 3.2f;
    int sum = add_int(1, 2);
    printf("The sum of %d and %d is %d\n", 1, 2, sum);

    // float sum = riscv_add_float(x, y);
    // printf("The sum of %f and %f is %f\n", x, y, sum);

    return 0;
}