	
;calculate new location of point from $R9, $R10 to $R11, $R12 by multiple matrix which is in $R8

%macro newLocation 0

	cvtsi2sd xmm0, R10
	movsd  xmm1, QWORD [R8]        	;matrix[0]
	mulsd  xmm1, xmm0

	cvtsi2sd xmm0, R9
	movsd  xmm2, QWORD [R8 + 8]     ;matrix[1]
	mulsd  xmm0, xmm2

	addsd  xmm0, xmm1
	movsd  xmm1, QWORD [R8 + 16]    ;matrix[2]
	addsd  xmm0, xmm1

	cvttsd2si r12, xmm0

;************************

	cvtsi2sd xmm0, R10
	movsd  xmm1, QWORD [R8 + 24]     ;matrix[3]
	mulsd  xmm1, xmm0

	cvtsi2sd xmm0, R9
	movsd  xmm2, QWORD [R8 + 32]     ;matrix[4]
	mulsd  xmm0, xmm2

	addsd  xmm0, xmm1
	movsd  xmm1, QWORD [R8 + 40]     ;matrix[5]
	addsd  xmm0, xmm1
	
	cvttsd2si r11, xmm0

%endmacro


	section .text
	
	global x86_function


;RDI = ptr to screen
;RSI = ptr to picture
;RDX = width
;RCX = height
;R8 = matrix   //9 element



x86_function:
	push rbp	; push "calling procedure" frame pointer
	mov rbp, rsp	; set new frame pointer 


;------------------------------------------------------------------------------
	push rbx
	push r12
	push r13
;------------------------------------------------------------------------------

	mov rax, rcx
	sub rax, 1
	imul rax, rdx
	imul rax, 4

	add rsi, rax
	add rdi, rax


;*****************************************


	xor r9, r9

loop_1_begin:
	cmp r9, RCX
	je loop_1_end

	xor r10, r10
loop_2_begin:
	cmp r10, RDX
	je loop_2_end



	newLocation


; if ($r12 >= 0 && $r12 < $rdx)


	cmp r12, 0 
	jl new_point_not_in_range

	cmp r12, rdx
	jge new_point_not_in_range

	cmp r11, 0 
	jl new_point_not_in_range

	cmp r11, rcx
	jge new_point_not_in_range



	mov rax, R11
	mov r13, rdx
	imul r13, rax
	imul r13, -1
	add r13, r12				;$r13 = $r11 * $rdx + $r12 // $r13 = newJ * weight + newI
	imul r13, 4				

	mov ebx, [rsi + r13] 		;$ebx = picture[newI + weight*newJ]

	mov rax, R9
	mov r13, rdx
	imul r13, rax
	imul r13, -1
	add r13, R10 				;$r13 = $r9 * $rdx + $r10 // $rax = j * weight + i
	imul r13, 4

	mov [rdi + r13], ebx		;screen[i + weight*j] = $ebx

	inc r10
	jmp loop_2_begin

new_point_not_in_range:

	mov rax, R9
	mov r13, rdx
	imul r13, rax
	imul r13, -1
	add r13, R10 				;$r13 = $r9 * $rdx + $r10 // $rax = j * weight + i
	imul r13, 4

	mov dword[rdi + r13], 0


	inc r10
	jmp loop_2_begin
loop_2_end:


	inc r9
	jmp loop_1_begin
loop_1_end:


;------------------------------------------------------------------------------
	pop r13
	pop r12
	pop rbx
;------------------------------------------------------------------------------

	mov rsp, rbp	; restore original stack pointer
	pop rbp			; restore "calling procedure" frame pointer
	ret

