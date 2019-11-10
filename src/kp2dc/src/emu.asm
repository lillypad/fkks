global sd0

section .text

;; 0x400C00 Image Base Modifier

sd0:
  push ebp
	;; push esi
  mov ebp, esp
  mov edi, [ebp + 0x8]           ; Parameter: Pointer to String Buffer
  mov eax, [ebp + 0xc]          ; Parameter: String ID
  mov ecx, [ebp + 0x10]
	push ebx
  push esi
  mov ebx, ecx
  ;; mov eax, 0xa7
  movzx eax, ax
  lea eax, [eax*8 + ebx + 0x668] ; Hard-coded Offset
  xor ecx, ecx
  xor esi, esi
  cmp cx, word [eax + 2]
  jae addr_40c985
addr_40c970:
  mov edx, dword [eax + 4]
  sub edx, 0x400c00             ; Calculated File Offset
  lea edx, [ebx + edx]
  movzx ecx, si
  mov dl, byte [edx + ecx]
  xor dl, byte [eax]
  inc esi
  mov byte [ecx + edi], dl
  cmp si, word [eax + 2]
  jb addr_40c970
addr_40c985:
  movzx eax, word [eax + 2]
  mov byte [eax + edi], 0
  mov eax, edi
  ;; pop ebx
  ;; pop esi
  pop esi
  pop ebx
  pop ebp
  ret

section .data

;; ed0:  incbin "sample.bin"
