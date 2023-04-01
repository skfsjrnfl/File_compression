# File_compression
Huffman tree를 이용하여 파일 압축하기


## 입력
* 알파벳만 고려
* 모든 대문자는 소문자로 교체

## 입력 예시
hw3_input.txt
>huffman

## 출력
* 출력은 두 개의 파일로 이루어짐
* 첫 번째 파일
  * Huffman tree를 표현하기
  * HEADEREND 로 구분하기
  * 입력받은 내용을 Huffman tree를 통해 변환한 문자열
* 두 번째 파일
  * Huffman tree 해석
  * 변환된 문자열을 재변환한 내용
 
 ## 출력 예시
 hw3_output1.txt <br>
 >(((a,m),f),((h,u),n))HEADEREND100101010100100011
 
 hw3_output2.txt <br>
 >a:000 <br>
 >f:01 <br>
 >h:100 <br>
 >m:001 <br>
 >n:11 <br>
 >u:101 <br>
 >huffman <br>
