; (var1 * -var2)
mov eax, [0x600000]
mov ecx, [0x600004]
neg ecx
imul ecx

; (var3 - ebx)
mov ecx, [0x600008]
sub	ecx, ebx
cdq
idiv ecx

; (var3) = (var1 * -var2) / (var3 -ebx)
mov dword ptr [0x600008], eax
done:
