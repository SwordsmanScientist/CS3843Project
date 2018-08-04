// EncryptData.cpp
//
// This file uses the input data and key information to encrypt the input data
//

#include "Main.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// code to encrypt the data as specified by the project assignment
int encryptData(char *data, int dataLength)
{
	int resulti = 0;

	gdebug1 = 0;					// a couple of global variables that could be used for debugging
	gdebug2 = 0;					// also can have a breakpoint in C code

	__asm {

		//Move data to edi
		mov edi, data

		//Clear registers
		xor ecx, ecx
		xor edx, edx
		xor eax, eax
		xor ebx, ebx

		/*
		//Store passwordhash in ebx
		mov bh, gPasswordHash[0]
		mov bl, gPasswordHash[1]
		*/

		//Loop through file
		LOOP1:

			cmp ecx, gNumRounds
			je done

			push ecx

			mov bh, gPasswordHash[0 + ecx * 4]
			mov bl, gPasswordHash[1 + ecx * 4]
			shl ebx, 16

			mov bh, gPasswordHash[2 + ecx * 4]
			mov bl, gPasswordHash[3 + ecx * 4]

			test bx, bx
			jz isZero

			jmp cont

		isZero:
			mov bx, 0xFFFF
			jmp cont

		cont:
			mov dx, bx

			mov bx, 0
			shr ebx, 16
			
			xor ecx, ecx
			jmp l2

		l2:
			mov al, byte ptr[edi + ecx]
			
			xor al, gkey[ebx]
			mov byte ptr[edi + ecx], al

			add ebx, edx

			cmp ebx, 65537
			jae indexAbove

			push edx
			push ebx

			jmp cont2

		indexAbove:
			sub ebx, 65537
			push edx
			push ebx
			jmp cont2

		cont2:
			// Begin Encrypt

			//A: rotates left 3 times
			rol al, 3

			// B
			rol al, 4

			push ecx					// ECX will be changed in step C

			//C: reverse bit order
			xor edx, edx

			mov cl, 7					// Used to keep track of bits
		
			LOOP2:
				cmp cl, -1				// This jump is only taken when checking the final Bit
				je Zero
				shr al, 1				// Used to check each Bit of EAX (CL = 7 checks Bit 0 / CL = 6 checks Bit 1 / etc. )
				jnc SKIP				// There wasn't a 1 in Bit 0
				mov ah, 1				// Theres's a 1 in Bit 0 so place a 1 in Bit 8 of EAX
				shl ah, cl				// Move the value of Bit 8 to Bit 8 + CL (Moves 1 to the opposite of the original position.)
				add dl, ah				// Build the reversed byte in DL
				xor ah, ah				// Reset to 0 for next Bit check

				dec cl
				jmp LOOP2

			Zero :
				cmp al, 0				// Check if Bit 0 of EAX is 0
				je Next					// Jump to the end if it is 0

				mov ah, 1				// Move 1 to Bit 8 of EAX
				add dl, ah				// Build the final result in DL

				jmp Next				// Jump to the end

			SKIP :
				dec cl
				cmp cl, -1				// If CL reaches -1 then that means the final Bit is being checked
				jge LOOP2

			Next :
				mov al, dl				// Move reversed byte into AL
				xor edx, edx			// Reset EDX to 0

			pop ecx						// Get previous ECX

			//D: swap even/odd bits
			xor edx, edx

			mov dl, al					// Move byte into DL for manipulation
			mov dh, al					// Move byte into DH for manipulation

			and dl, 0x01				// Check Bit 0
			shl dl, 1					// Move value in Bit 0 to Bit 1

			and dh, 0x02				// Check Bit 1
			shr dh, 1					// Move value in Bit 1 to Bit 0

			or dl, dh					// OR the previous two results (Build the new byte in DL. DL will have the final result)

			mov dh, al					// Move original byte into DH
			and dh, 0x04				// Check Bit 2
			shl dh, 1					// Move value in Bit 2 to Bit 3

			or dl, dh					// Build the new byte in DL

			mov dh, al					// Move original byte into DH
			and dh, 0x08				// Check Bit 3
			shr dh, 1					// Move value in Bit 3 to Bit 2

			or dl, dh					// Build the new byte in DL

										// Check bit 4
			mov dh, al					// Move original byte into DH
			and dh, 0x10				// Check Bit 4
			shl dh, 1					// Move value in Bit 4 to Bit 5

			or dl, dh					// Build the new byte in DL

										// Check bit 5
			mov dh, al					// Move original byte into DH
			and dh, 0x20				// Check Bit 5
			shr dh, 1					// Move value in Bit 5 to Bit 4

			or dl, dh					// Build the new byte in DL

										// Check bit 6
			mov dh, al					// Move original byte into DH
			and dh, 0x40				// Check Bit 6
			shl dh, 1					// Move value in Bit 6 to Bit 7

			or dl, dh					// Build the new byte in DL

										// Check bit 7
			mov dh, al					// Move original byte into DH
			and dh, 0x80				// Check Bit 7
			shr dh, 1					// // Move value in Bit 7 to Bit 6

			or dl, dh					// Build the new byte in DL

			mov al, dl					// Move the new byte into AL
			xor edx, edx				// Reset EDX to 0


			//E: Swap with encode table
			xor ebx, ebx						// Zero out ebx
			mov bl, al							// Move al in bl

			mov ah, gEncodeTable[ebx]			// Move byte at gDecodeTable[ebx] to ah

			mov byte ptr[edi + ecx], ah			// Replace original byte with result

			
			//END ENCRYPT

			pop ebx
			pop edx

			inc ecx								// Go to the next byte
			cmp dataLength, ecx					// Check if the EOF has been reached
			je innerDone							// Loop again

			jmp l2

		innerDone:
			pop ecx
			inc ecx
			jmp LOOP1

		done :

	}

	return resulti;
} // encryptData