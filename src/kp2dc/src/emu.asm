global sd0
global c2dec

section .text

;; 0x400C00 Image Base Modifier

;; String Decryptor
sd0:
  push ebp
  mov ebp, esp
  mov edi, [ebp + 0x8]           ; Parameter: Pointer to String Buffer
  mov eax, [ebp + 0xc]          ; Parameter: String ID
  mov ecx, [ebp + 0x10]
  push ebx
  push esi
  mov ebx, ecx
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
  pop esi
  pop ebx
  pop ebp
  ret

;; C2 Server Decryptor / Encryptor
;; 0: ptr to base64 decoded data
;; 1: data size
;; 2: ptr to decryption key
;; 3: decryption key length
c2dec:
  push ebp
  mov ebp, esp
  push esi
  xor esi, esi
  cmp dword [ebp + 0xc], esi
  jbe addr_403ddd
addr_403dc2:
  mov eax, dword [ebp + 0x8]
  lea ecx, [esi + eax]
  xor edx, edx
  mov eax, esi
  div dword [ebp + 0x14]
  mov eax, dword [ebp + 0x10]
  mov al, byte [edx + eax]
  xor byte [ecx], al
  inc esi
  cmp esi, dword [ebp + 0xc]
  jb addr_403dc2
addr_403ddd:
  pop esi
  pop ebp
  ret

;; section .data

;; ed0:  incbin "sample.bin"
