; (var1 * -5)
mov eax, [0x600000]
mov ebx, -5
imul ebx
mov dword ptr [0x60000c], eax

; (-var2 % var3)
mov eax, [0x600004]
neg eax
cdq
mov ebx, [0x600008]
idiv ebx
mov ebx, edx

; var4 = (var1 * -5) / (-var2 % var3)
mov eax, dword ptr [0x60000c]
cdq
idiv ebx
mov dword ptr [0x60000c], eax
done:
