GLOBAL _mutexLock
GLOBAL _mutexUnlock

EXTERN _contextSwitchProcess

section .text

_mutexLock:
    mov rax, 1

    xchg rax, [rdi]

    test rax, rax

    jz ok

    call _contextSwitchProcess
    jmp _mutexLock

ok:
    ret

_mutexUnlock:
    mov rax, 0

    xchg rax, [rdi]

    ret