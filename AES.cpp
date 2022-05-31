#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char round_key[4][64] = {};

// rcon 表，key expansion 用 
unsigned char Rcon[] = {
//   0     1     2     3     4     5     6     7     8     9     10    11    12    13    14    15
	0x87, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a
};

// s表 正查 
unsigned char s_box[256] = {
	//    0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F 
		0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,// 0 
		0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,// 1 
		0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,// 2 
		0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,// 3 
		0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,// 4 
		0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,// 5 
		0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,// 6 
		0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,// 7 
		0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,// 8 
		0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,// 9 
		0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,// A 
		0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,// B 
		0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,// C 
		0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,// D 
		0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,// E 
		0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 // F 
	};

// s表 反查 
unsigned char inv_s_box[256] = {
	//    0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F 
		0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,// 0 
		0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,// 1 
		0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,// 2 
		0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,// 3 
		0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,// 4 
		0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,// 5 
		0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,// 6 
		0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,// 7 
		0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,// 8 
		0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,// 9 
		0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,// A 
		0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,// B 
		0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,// C 
		0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,// D 
		0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,// E 
		0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d // F 
	};

// 印出 4 * 4 矩陣 
void show_matrix(unsigned char matrix[4][4]){
	printf("\n");
	for(int j=0;j<4;j++){
		for(int i=0;i<4;i++){
			printf("%2X ", matrix[j][i]);
		}
		printf("\n");
	}
	printf("\n");
}

// 印出所有的 round key (測試的時候用) 
void show_all_round_key(){
	printf("show all round key:\n\n");
	for(int r=0;r<16;r++){
		for(int j=0;j<4;j++){
			for(int i=0;i<4;i++){
				printf("%2X ", round_key[j][r*4+i]);
			}
			printf("\n");
		}
		printf("\n\n");
	}
} 

//  選擇 密鑰長度 
int choose_key_len(){
	int key_len;
	printf("Input the key len(128, 192, 256) : ");
	scanf("%d", &key_len);
	if(key_len != 128 && key_len != 192 && key_len != 256){
		printf("key len error!");
		exit(0);
	}
	return key_len;
}

// 選擇 加密模式 
int  choose_mode(){
	int mode;
	printf("\n 1.AES-ECB  2.AES-CBC  3.AES-CTR  4.AES-CFB  5.AES-OFB\nchoose mode : ");
	scanf("%d", &mode);
	if(mode != 1 && mode != 2 && mode != 3 && mode != 4 && mode != 5){
		printf("mode error");
		exit(0);
	}
	return mode;
}

// 輸入 密鑰 
void input_key(int cp_len){
	unsigned char in_key[32] = {};
	printf("Secret Key:");
	scanf("%s",in_key);
	
	// 把 輸入的密鑰填進 round key 陣列中，等待 key expansion 
	for(int index=0;index<32;index++){
		round_key[index%4][index/4] = in_key[index];
	}
	printf("\nYour key is:\n\n");
	for(int j=0;j<4;j++){
		for(int i=0;i<cp_len/32;i++){
			printf("%2X ", round_key[j][i]);
		}
		printf("\n");
	}
	printf("\n\n");
}

// 輸入 iv 值 
void input_iv_matrix(unsigned char iv[4][4]){
	char input[17] = {};
	printf("\ninput 16 char for iv matrix : ");
	scanf("%s", input);
	for(int i=0;i<16;i++){
		iv[i%4][i/4] = input[i];
	}
}

// 輸入明文，最大48 
void input_plaintext(unsigned char p[3][4][4]){
	char input[49] = {};
	setbuf(stdin, NULL);
	printf("\ninput your plaintext (limit = 48) : ");
	scanf("%[^\n]", input);
	//printf("%s\n%d", input, strlen(input));
	
	// zero padding
	for(int index=strlen(input);index<48;index++){
		input[index] = 0;
	}
	input[48] = '\0';
	
	// 把輸入的明文填進3個 block 
	for(int index = 0;index < 48;index++){
		p[index/16][(index%16)%4][(index%16)/4] = input[index];
	}
}

// 密鑰擴展
void key_expansion(int cp_len){
	int nk = cp_len/32;
	/*
		128/192 的規則相近，128需要擴到44(i = 4 ~ 43)個word，192則是52(i = 6 ~ 51) word， 
		如果 i % nk == 0， 規則是將 i-1 word 經過3步驟處裡(RotWord() -> SubWord() -> 與Rcon[i/Nk]做異或)後，與 i-nk word 做異或，
		 如果 i % nk != 0，i-1 與 i-nk 做異或 。
		 
		256 於129/192大致相同，但多了一項規則，如過 i是4的倍數但不是8的倍數時 ，則是將 i-1 SubWord後，與i-nk異或。 
	*/ 
	switch(nk){
		case 4:
		case 6:
			for(int i = nk;i< (nk+7)*4; i++){
				if(i % nk == 0){
					for(int j=0;j<4;j++){
						round_key[j][i] = round_key[(j+1)%4][i-1];
						round_key[j][i] = s_box[round_key[j][i]];
						if(j == 0){
							round_key[j][i] ^= Rcon[i/nk];
						}
						round_key[j][i] ^= round_key[j][i-nk]; 
					}
				}
				else{
					for(int j=0;j<4;j++){
						round_key[j][i] = round_key[j][i-1]^round_key[j][i-nk];
					}
				}
			}
			break;
		case 8:
			for(int i = nk;i< (nk+7)*4; i++){
				if(i % nk == 0){
					for(int j=0;j<4;j++){
						round_key[j][i] = round_key[(j+1)%4][i-1];
						round_key[j][i] = s_box[round_key[j][i]];
						if(j == 0){
							round_key[j][i] ^= Rcon[i/nk];
						}
						round_key[j][i] ^= round_key[j][i-nk]; 
					}
				}
				else if(i % 4 == 0){
					for(int j=0;j<4;j++){
						round_key[j][i] = s_box[round_key[j][i-1]] ^ round_key[j][i-nk];
					}
				}
				else{
					for(int j=0;j<4;j++){
						round_key[j][i] = round_key[j][i-1]^round_key[j][i-nk];
					}
				}
			}
			break;
	}
}

//  SubstituteBytes 就是將原來的值查表取得新的值 
void SubstituteBytes(unsigned char S[4][4]){
	for(int rowIndex = 0;rowIndex < 4;rowIndex++){
		for(int colIndex = 0;colIndex < 4;colIndex++){
			S[rowIndex][colIndex] = s_box[S[rowIndex][colIndex]];
		}
	}
}

//  InverseSubstituteBytes 就是反查，取得原來的值 
void InverseSubstituteBytes(unsigned char S[4][4]){
	for(int rowIndex = 0;rowIndex < 4;rowIndex++){
		for(int colIndex = 0;colIndex < 4;colIndex++){
			S[rowIndex][colIndex] = inv_s_box[S[rowIndex][colIndex]];
		}
	}
}

//  ShiftRows 是將每一列向左移一格，幅度是遞增的 
void ShiftRows(unsigned char S[4][4]){
	for(int rowIndex = 1;rowIndex<4;rowIndex++){
		for(int round = 0;round < rowIndex;round++){
			int temp = S[rowIndex][0];
			for(int colIndex = 0;colIndex < 3;colIndex++){
				S[rowIndex][colIndex] = S[rowIndex][colIndex + 1];
			}
			S[rowIndex][3] = temp;
		}
	}
}

//  InverseShiftRows 復原 ShiftRows， 將每一列向右移一格，幅度是遞增的 
void InverseShiftRows(unsigned char S[4][4]){
	for(int rowIndex = 1;rowIndex<4;rowIndex++){
		for(int round = 0;round < rowIndex;round++){
			int temp = S[rowIndex][3];
			for(int colIndex = 3;colIndex > 0;colIndex--){
				S[rowIndex][colIndex] = S[rowIndex][colIndex - 1];
			}
			S[rowIndex][0] = temp;
		}
	}
} 

// 將值 *2 ，如果會異位，就與 0x1b做異或，最後回傳 
unsigned char mul2(unsigned char a){
	unsigned char result;
	result = a << 1;
	if(a >> 7 ){
		result ^= 0x1b;
	}
	return result;
}


//  MixColumns 將 block 與特定矩陣做相乘，利用剛剛的mul2 function 
void MixColumns(unsigned char S[4][4]){
	unsigned char result[4][4] = {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	};
	int matrix[4][4] = {
		{2, 3, 1, 1},
		{1, 2, 3, 1},
		{1, 1, 2, 3},
		{3, 1, 1, 2}
	};
	
	for(int rowIndex=0;rowIndex < 4;rowIndex++){
		for(int colIndex=0;colIndex < 4;colIndex++){
			for(int i=0;i<4;i++){
				unsigned char temp = 0x0;
				if(matrix[rowIndex][i] == 1){
					temp = S[i][colIndex];
				}
				else if(matrix[rowIndex][i] == 2){
					temp = mul2(S[i][colIndex]);
				}
				else{
					temp = mul2(S[i][colIndex]) ^ S[i][colIndex];
				}
				result[rowIndex][colIndex] ^= temp;
			}	
		}
	}
	
	for(int row = 0;row < 4;row++){
		for(int col = 0;col < 4;col++){
			S[row][col] = result[row][col];
		}
	}
}


//  MixColumns 將 block 與特定矩陣做相乘，利用剛剛的mul2 function 
void InverseMixColumns(unsigned char S[4][4]){
	unsigned char result[4][4] = {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	};
	int matrix[4][4] = {
		{14, 11, 13, 9},
		{9, 14, 11, 13},
		{13, 9, 14, 11},
		{11, 13, 9, 14}
	};
	
	/*	
		x * 9 = (((x * 2) * 2) * 2) + x
		x * 11 = ((((x * 2) * 2) + x) * 2) + x
		x * 13 = ((((x * 2) + x) * 2) * 2) + x
		x *14 = ((((x * 2) + x) * 2) + x) * 2
	*/ 
	
	for(int rowIndex=0;rowIndex < 4;rowIndex++){
		for(int colIndex=0;colIndex < 4;colIndex++){
			for(int i=0;i<4;i++){
				unsigned char temp = 0x0;
				if(matrix[rowIndex][i] == 9){
					temp = mul2(mul2(mul2(S[i][colIndex]))) ^ S[i][colIndex];
				}
				else if(matrix[rowIndex][i] == 11){
					temp = mul2(mul2(mul2(S[i][colIndex])) ^ S[i][colIndex]) ^ S[i][colIndex];
				}
				else if(matrix[rowIndex][i] == 13){
					temp = mul2(mul2(mul2(S[i][colIndex]) ^ S[i][colIndex])) ^ S[i][colIndex];
				}
				else{
					temp = mul2(mul2(mul2(S[i][colIndex]) ^ S[i][colIndex]) ^ S[i][colIndex]);
				}
				result[rowIndex][colIndex] ^= temp;
			}	
		}
	}
	
	for(int row = 0;row < 4;row++){
		for(int col = 0;col < 4;col++){
			S[row][col] = result[row][col];
		}
	}
}

// 將 block 與 key expansion 過的 round key 做 xor 
void AddRoundKey(unsigned char S[4][4],int round){
	for(int rowIndex=0;rowIndex < 4;rowIndex++){
		for(int colIndex=0;colIndex < 4;colIndex++){
			S[rowIndex][colIndex] ^= round_key[rowIndex][round*4+colIndex];
		} 
	}
}

// AES 加密流程(不同的key長度，所需的回合不同) 
void encryption(int cp_len, unsigned char S[4][4]){
	int max_round = cp_len/32 + 6;
	
	//  
	AddRoundKey(S, 0);
	for(int round = 1; round <= max_round; round++){
		if(round != max_round){
			SubstituteBytes(S);
			ShiftRows(S);
			MixColumns(S);
			AddRoundKey(S, round);
		}
		else{
			SubstituteBytes(S);
			ShiftRows(S);
			AddRoundKey(S, round);
		}
	}
}

// AES 解密流程(加密過程回推)
void decryption(int cp_len, unsigned char S[4][4]){
	int max_round = cp_len/32 + 6;
	
	// 	
	AddRoundKey(S, max_round);
	for(int round = max_round-1; round >= 0; round--){
		if(round != 0){
			InverseShiftRows(S);
			InverseSubstituteBytes(S);
			AddRoundKey(S, round);
			InverseMixColumns(S);
		}
		else{
			InverseShiftRows(S);
			InverseSubstituteBytes(S);
			AddRoundKey(S, round);
		}
	}
}

// 將兩 4*4 陣列 做 xor 
void xor_matrix(unsigned char a[4][4], unsigned char b[4][4]){
	for(int j=0;j<4;j++){
		for(int i=0;i<4;i++){
			b[j][i] ^= a[j][i];
		}
	}
}

// ECB 加密比較單純，彼此不會依賴，所以可以個別執行加密 
void ECB(int cp_len, unsigned char plaintext[3][4][4]){
	// show加密前的值 
	printf("\nBefore Encryption (3 Block) : \n\n");
	show_matrix(plaintext[0]);
	show_matrix(plaintext[1]);
	show_matrix(plaintext[2]);
	
	//加密 
	encryption(cp_len, plaintext[0]);
	encryption(cp_len, plaintext[1]);
	encryption(cp_len, plaintext[2]);
	
	// show加密後的值 
	printf("\nAfter Encryption (3 Block) : \n\n");
	show_matrix(plaintext[0]);
	show_matrix(plaintext[1]);
	show_matrix(plaintext[2]);
	
	// 解密 
	decryption(cp_len, plaintext[0]);
	decryption(cp_len, plaintext[1]);
	decryption(cp_len, plaintext[2]);
	
	// show解密後的值
	printf("\nAfter Decryption (3 Block) : \n\n");
	show_matrix(plaintext[0]);
	show_matrix(plaintext[1]);
	show_matrix(plaintext[2]);
}

// block彼此會有依賴，規則如內文註解 
void CBC(int cp_len, unsigned char plaintext[3][4][4], unsigned char IV[4][4]){
/*
	unsigned char IV[4][4] = {
		{0x01, 0x02, 0x03, 0x04},
		{0x05, 0x06, 0x07, 0x10},
		{0x11, 0x12, 0x13, 0x14},
		{0x15, 0x16, 0x17, 0x20},
	};
*/
	// show IV 值 
	printf("\ndefault IV matrix:\n");
	show_matrix(IV);
	
	// show加密前的值 
	printf("\nBefore Encryption (3 Block) : \n\n");
	show_matrix(plaintext[0]);
	show_matrix(plaintext[1]);
	show_matrix(plaintext[2]);
	
	// 加密 
	//每一個block加密前都需要與前一個密文 xor，第一個 block 則是 與 IV xor 
	xor_matrix(IV, plaintext[0]);
	encryption(cp_len, plaintext[0]);
	xor_matrix(plaintext[0], plaintext[1]);
	encryption(cp_len, plaintext[1]);
	xor_matrix(plaintext[1], plaintext[2]);
	encryption(cp_len, plaintext[2]);
	
	// show加密後的值 
	printf("\nAfter Encryption (3 Block) : \n\n");
	show_matrix(plaintext[0]);
	show_matrix(plaintext[1]);
	show_matrix(plaintext[2]);
	
	// 解密 
	//  與加密相反，從最後一個block開始解，之後再與前一個密文xor，第一個則與 IV xor 
	decryption(cp_len, plaintext[2]);
	xor_matrix(plaintext[1], plaintext[2]);
	decryption(cp_len, plaintext[1]);
	xor_matrix(plaintext[0], plaintext[1]);
	decryption(cp_len, plaintext[0]);
	xor_matrix(IV, plaintext[0]);
	
	// show解密後的值
	printf("\nAfter Decryption (3 Block) : \n\n");
	show_matrix(plaintext[0]);
	show_matrix(plaintext[1]);
	show_matrix(plaintext[2]);
}

void CTR(int cp_len, unsigned char plaintext[3][4][4], unsigned char IV_[4][4]){
/*	
	unsigned char IV_[4][4] = {
		{0x01, 0x02, 0x03, 0x04},
		{0x05, 0x06, 0x07, 0x10},
		{0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00},
	};
*/
	int count;
	unsigned char IV[4][4];
	
	// show IV 值
	printf("\ndefault IV matrix:\n");
	show_matrix(IV_);
	
	// show加密前的值 
	printf("\nBefore Encryption (3 Block) : \n\n");
	show_matrix(plaintext[0]);
	show_matrix(plaintext[1]);
	show_matrix(plaintext[2]);
	
	//加密 
	// IV block 最後一個byte 要加上 ，然後把IV加密後，與明文xor 
	count = 0;
	for(int k=0;k<3;k++){
		// 將 IV 復原 
		for(int j=0;j<4;j++){
			for(int i=0;i<4;i++){
				IV[j][i] = IV_[j][i];
			}
		}
		IV[3][3] += count;
		encryption(cp_len, IV);
		xor_matrix(IV, plaintext[k]);
		count += 1;
	}
	
	// show加密後的值
	printf("\nAfter Encryption (3 Block) : \n\n");
	show_matrix(plaintext[0]);
	show_matrix(plaintext[1]);
	show_matrix(plaintext[2]);
	
	// 解密
	// 與加密相似 
	count = 0;
	for(int k=0;k<3;k++){
		for(int j=0;j<4;j++){
			for(int i=0;i<4;i++){
				IV[j][i] = IV_[j][i];
			}
		}
		IV[3][3] += count;
		encryption(cp_len, IV);
		xor_matrix(IV, plaintext[k]);
		count += 1;
	}
	
	// show解密後的值
	printf("\nAfter Decryption (3 Block) : \n\n");
	show_matrix(plaintext[0]);
	show_matrix(plaintext[1]);
	show_matrix(plaintext[2]);
}

void CFB(int cp_len, unsigned char plaintext[3][4][4], unsigned char IV_[4][4]){
	// show IV 值
	printf("\ndefault IV matrix:\n");
	show_matrix(IV_);
	
	unsigned char temp[4][4] = {};
	unsigned char temp_[4][4] = {};
	
	unsigned char IV[4][4];
	for(int j=0;j<4;j++){
		for(int i=0;i<4;i++){
			IV[j][i] = IV_[j][i];
		}
	}
	
	// show加密前的值
	printf("\nBefore Encryption (3 Block) : \n\n");
	show_matrix(plaintext[0]);
	show_matrix(plaintext[1]);
	show_matrix(plaintext[2]);
	
	//加密
	//將前一個密文再加密一次(第一個 IV取代)，然後再做xor 
	encryption(cp_len, IV);
	xor_matrix(IV, plaintext[0]);
	
	// 把第一個block的密文用temp存起來，之後加密用 
	for(int j=0;j<4;j++){
		for(int i=0;i<4;i++){
			temp[j][i] = plaintext[0][j][i];
		}
	}
	
	// 加密第一個block的密文，然後與第二個 block xor 
	encryption(cp_len, temp);
	xor_matrix(temp, plaintext[1]);
	
	// 把第二個block的密文用temp存起來，之後加密用 
	for(int j=0;j<4;j++){
		for(int i=0;i<4;i++){
			temp[j][i] = plaintext[1][j][i];
		}
	}
	
	// 加密第二個block的密文，然後與第三個 block xor
	encryption(cp_len, temp);
	xor_matrix(temp, plaintext[2]);
	
	// show加密後的值
	printf("\nAfter Encryption (3 Block) : \n\n");
	show_matrix(plaintext[0]);
	show_matrix(plaintext[1]);
	show_matrix(plaintext[2]);
	
	// 解密 
	// 把IV 讀回最原始的狀態 
	for(int j=0;j<4;j++){
		for(int i=0;i<4;i++){
			IV[j][i] = IV_[j][i];
		}
	}
	
	// 把第一個密文存進temp，之後解密用 
	for(int j=0;j<4;j++){
		for(int i=0;i<4;i++){
			temp[j][i] = plaintext[0][j][i];
		}
	}
	
	// 把IV encryption，與block 1 xor，第一個block明文就出來了 
	encryption(cp_len, IV);
	xor_matrix(IV, plaintext[0]);
	
	// 第二個密文存進temp_，之後解密用 
	for(int j=0;j<4;j++){
		for(int i=0;i<4;i++){
			temp_[j][i] = plaintext[1][j][i];
		}
	}
	
	// block 1 密文再encrytion一次 ，然後 xor block 2密文 ，block 2 明文就出來了 
	encryption(cp_len, temp);
	xor_matrix(temp, plaintext[1]);
	
	// block 2 密文再encrytion一次 ，然後 xor block 3密文 ，block 3 明文就出來了 
	encryption(cp_len, temp_);
	xor_matrix(temp_, plaintext[2]);
	
	// show解密後的值
	printf("\nAfter Decryption (3 Block) : \n\n");
	show_matrix(plaintext[0]);
	show_matrix(plaintext[1]);
	show_matrix(plaintext[2]);
}


/*

*/
void OFB(int cp_len, unsigned char plaintext[3][4][4], unsigned char IV_[4][4]){
	printf("\ndefault IV matrix:\n");
	show_matrix(IV_);
	// show IV 值
	unsigned char IV[4][4];
	for(int j=0;j<4;j++){
		for(int i=0;i<4;i++){
			IV[j][i] = IV_[j][i];
		}
	}
	
	// show加密前的值
	printf("\nBefore Encryption (3 Block) : \n\n");
	show_matrix(plaintext[0]);
	show_matrix(plaintext[1]);
	show_matrix(plaintext[2]);
	
	// IV 做 encrytion ，然後與明文xor，循環這兩個動作(IV不重置)   
	for(int k=0;k<3;k++){
		encryption(cp_len, IV);
		xor_matrix(IV, plaintext[k]);
	}
	
	// show加密後的值
	printf("\nAfter Encryption (3 Block) : \n\n");
	show_matrix(plaintext[0]);
	show_matrix(plaintext[1]);
	show_matrix(plaintext[2]);
	
	// 重置 IV 
	for(int j=0;j<4;j++){
		for(int i=0;i<4;i++){
			IV[j][i] = IV_[j][i];
		}
	}
	
	// 和加密一樣 
	for(int k=0;k<3;k++){
		encryption(cp_len, IV);
		xor_matrix(IV, plaintext[k]);
	}
	
	// show解密後的值
	printf("\nAfter Decryption (3 Block) : \n\n");
	show_matrix(plaintext[0]);
	show_matrix(plaintext[1]);
	show_matrix(plaintext[2]);
}

int main(){
	int cp_len, mode;
	unsigned char plaintext[3][4][4] = {};
	unsigned char IV[4][4] = {};
	cp_len = choose_key_len();
	input_key(cp_len);
	key_expansion(cp_len);
	mode = choose_mode();
	
	// 根據mode 選擇要執行的 function 
	switch(mode){
		case 1:
			input_plaintext(plaintext);
			ECB(cp_len, plaintext);
			break;
		case 2:
			input_iv_matrix(IV);
			input_plaintext(plaintext);
			CBC(cp_len, plaintext, IV);
			break;
		case 3:
			input_iv_matrix(IV);
			input_plaintext(plaintext);
			CTR(cp_len, plaintext, IV);
			break;
		case 4:
			input_iv_matrix(IV);
			input_plaintext(plaintext);
			CFB(cp_len, plaintext, IV);
			break;
		case 5:
			input_iv_matrix(IV);
			input_plaintext(plaintext);
			OFB(cp_len, plaintext, IV);
	}
	return 0;
}



