#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

// This simple JIT program just returns the user input number
int dyn_program1(const char *user_input)
{
    //  mov eax, 0x0
    //  ret
    unsigned char program[] = {0xb8 // -  mov eax
                               ,
                               0x00 //
                               ,
                               0x00 //
                               ,
                               0x00 //
                               ,
                               0x00 // -  0x0       // We will change this value by the input value:  program[4] = user value
                               ,
                               0xc3}; // -  ret

    // Assign the user value to the program
    const int input_num = atoi(user_input);
    memcpy(&program[1], &input_num, 4);

    // Defining the dynamic memory that our mini JIT program will take up
    void *lptr = mmap(NULL, sizeof(program), PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);
    // Injecting our mini program dynamically into the memory
    memcpy(lptr, program, sizeof(program));

    // This define the pointer to call the mini program
    const int (*dyn_func)() = lptr;
    // Calling this function pointer will make execute our mini program
    // `call_res` will handle the return value, in this case, this will be
    // the value entered by the user.
    const int call_res = dyn_func();

    // We no longer need our mini dynamic JIT program
    // Se we free the memory used
    munmap(lptr, sizeof(program));

    // Display some info
    printf("You entered '%s' and the JIT function returned '%d'.\n", user_input, call_res);
    return call_res;
}

// This simple JIT program just returns the user input number multiplied by 10
int dyn_program2(const char *user_input)
{
    // mov eax, 0
    // imul eax, 10
    // ret
    unsigned char program[] = {0xb8 // -  mov eax
                               ,
                               0x00, 0x00, 0x00, 0x00 // -  0x0       // We will change this value by the input value:  program[4] = user value
                               ,
                               0x6b // -  imul eax, eax, 0xa
                               ,
                               0xc0, 0x0a, 0xc3}; // -  ret

    // Assign the user value to the program
    const int input_num = atoi(user_input);
    memcpy(&program[1], &input_num, 4);

    // Defining the dynamic memory that our mini JIT program will take up
    void *lptr = mmap(NULL, sizeof(program), PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);
    // Injecting our mini program dynamically into the memory
    memcpy(lptr, program, sizeof(program));

    // This define the pointer to call the mini program
    const int (*dyn_func)() = lptr;
    // Calling this function pointer will make execute our mini program
    // `call_res` will handle the return value, in this case, this will be
    // the value entered by the user.
    const int call_res = dyn_func();

    // We no longer need our mini dynamic JIT program
    // Se we free the memory used
    munmap(lptr, sizeof(program));

    // Display some info
    printf("You entered '%s' and the JIT function returned '%d'.\n", user_input, call_res);
    return call_res;
}

int main(int argc, char *argv[])
{
    // The usage of this mini program
    if (argc < 2)
        return fprintf(stderr, "Usage: main <number>\n"), EXIT_FAILURE;

    const int res0 = dyn_program1(argv[1]);
    const int res1 = dyn_program2(argv[1]);

    return EXIT_SUCCESS;
}