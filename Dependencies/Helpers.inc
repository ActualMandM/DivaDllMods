ASM_HOOK MACRO NAME, ADDRESS
.data
public original&NAME&
original&NAME& QWORD ADDRESS

.code
implOf&NAME& proc
ENDM

END_ASM_HOOK MACRO NAME
implOf&NAME& endp
ENDM