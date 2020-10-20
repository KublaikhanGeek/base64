/*base64.c*/
#include "base64.h"
#include <stdio.h>

#define B64_ERROR 0xFF //  1111 1111
//定义base64编码表
static const unsigned char data_bin2ascii[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\
abcdefghijklmnopqrstuvwxyz0123456789+/";
//根据base64表，以字符找到对应的十进制数据
static const unsigned char data_ascii2bin[128] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xF0, 0xFF,
    0xFF, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xF2, 0xFF, 0x3F,
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0xFF, 0xFF,
    0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12,
    0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24,
    0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
    0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

/*******************************************************************************
 * function name : base64_encode
 * description   : 对 ASCII 字符串进行 base64 编码
 * param[in]     : in 待编码的字符串
 * param[out]    : out 编码后的字符串
 * return        : 编码后字符串长度。-1: 失败。
 *******************************************************************************/
int base64_encode(unsigned char *in, unsigned char *out, int out_len) {
  int encode_len = 0;
  int in_len = 0;
  int i = 0, n = 0;

  if (in == NULL || out == NULL || out_len < 0) {
    printf("Invalid parameter \n");
    return -1;
  }

  in_len = strlen(in);
  for (i = 0; i < in_len; i++) {
    if (in[i] < 0 || in[i] > 127) {
      printf("can not encode Non-ASCII characters \n");
      return -1;
    }
  }

  //计算经过base64编码后的字符串长度
  if (in_len % 3 == 0) {
    encode_len = in_len / 3 * 4;
  } else {
    encode_len = (in_len / 3 + 1) * 4;
  }

  if (out_len < encode_len) {
    printf("no enough buffer space \n");
    return -1;
  }
  out[encode_len] = '\0';

  //以3个8位字符为一组进行编码
  int nloop = in_len % 3 == 0 ? in_len : in_len - 3;
  for (i = 0; i < nloop; i += 3) {
    //取出第一个字符的前6位并找出对应的结果字符
    out[n] = data_bin2ascii[in[i] >> 2];
    //将第一个字符的后位与第二个字符的前4位进行组合并找到对应的结果字符
    out[n + 1] = data_bin2ascii[(in[i] & 0x3) << 4 | (in[i + 1] >> 4)];
    //将第二个字符的后4位与第三个字符的前2位组合并找出对应的结果字符
    out[n + 2] = data_bin2ascii[(in[i + 1] & 0xf) << 2 | (in[i + 2] >> 6)];
    //取出第三个字符的后6位并找出结果字符
    out[n + 3] = data_bin2ascii[in[i + 2] & 0x3f];
    n += 4;
  }

  switch (in_len % 3) {
  case 1:
    out[n] = data_bin2ascii[in[i] >> 2];
    out[n + 1] = data_bin2ascii[((in[i] & 3) << 4) | ((0 & 0xf0) >> 4)];
    out[n + 2] = '=';
    out[n + 3] = '=';
    break;
  case 2:
    out[n] = data_bin2ascii[in[i] >> 2];
    out[n + 1] = data_bin2ascii[((in[i] & 3) << 4) | ((in[i + 1] & 0xf0) >> 4)];
    out[n + 2] = data_bin2ascii[((in[i + 1] & 0xf) << 2) | ((0 & 0xc0) >> 6)];
    out[n + 3] = '=';
    break;
  default:
    break;
  }

  return encode_len;
}

/*******************************************************************************
 * function name : base64_decode
 * description   : 对 base64 编码后的字符串进行解码
 * param[in]     : in 待解码的字符串
 * param[out]    : out 解码后的字符串
 * return        : 解码后字符串长度。-1: 失败。
 *******************************************************************************/
int base64_decode(unsigned char *in, unsigned char *out, int out_len) {
  int decode_len = 0;
  int in_len = 0;
  int i = 0, n = 0;
  if (in == NULL || out == NULL || out_len < 0) {
    printf("Invalid parameter \n");
    return -1;
  }

  //计算解码后的字符串长度
  in_len = strlen(in);
  if (in_len % 4 != 0) {
    printf("bad base64 string \n");
    return -1;
  }
  //判断编码后的字符串后是否有=
  if (strstr(in, "==")) {
    decode_len = in_len / 4 * 3 - 2;
  } else if (strstr(in, "=")) {
    decode_len = in_len / 4 * 3 - 1;
  } else {
    decode_len = in_len / 4 * 3;
  }

  if (out_len < decode_len) {
    printf("no enough buffer space \n");
    return -1;
  }

  out[decode_len] = '\0';
  //以4个字符为一位进行解码
  int b[4] = {0};
  int table_count = sizeof(data_ascii2bin) / sizeof(data_ascii2bin[0]);
  int nLoop = in[in_len - 1] == '=' ? in_len - 4 : in_len;
  for (i = 0; i < nLoop; i += 4) {
    for (int j = 0; j < sizeof(b) / sizeof(b[0]); ++j) {
      if (in[i] >= table_count) // 0x80 sizeof data_ascii2bin, ascii最大值
      {
        printf("bad base64 string \n");
        return -1;
      }
      b[j] = data_ascii2bin[in[i + j]];
      if (b[j] == B64_ERROR) {
        printf("bad base64 string \n");
        return -1;
      }
    }

    out[n] = (b[0] << 2) | ((b[1] & 0x30) >> 4);
    out[n + 1] = ((b[1] & 0xf) << 4) | ((b[2] & 0x3c) >> 2);
    out[n + 2] = ((b[2] & 0x3) << 6) | b[3];
    n += 3;
  }

  if (in[in_len - 1] == '=' && in[in_len - 2] == '=') {
    if (in[i] >= table_count || in[i + 1] >= table_count) {
      printf("bad base64 string \n");
      return -1;
    }
    b[0] = data_ascii2bin[in[i]];
    b[1] = data_ascii2bin[in[i + 1]];
    if (b[0] == B64_ERROR || b[1] == B64_ERROR) {
      printf("bad base64 string \n");
      return -1;
    }

    out[n] = (b[0] << 2) | ((b[1] & 0x30) >> 4);
  }

  if (in[in_len - 1] == '=' && in[in_len - 2] != '=') {
    if (in[i] >= table_count || in[i + 1] >= table_count ||
        in[i + 2] >= table_count) {
      printf("bad base64 string \n");
      return -1;
    }
    b[0] = data_ascii2bin[in[i]];
    b[1] = data_ascii2bin[in[i + 1]];
    b[2] = data_ascii2bin[in[i + 2]];
    if (b[0] == B64_ERROR || b[1] == B64_ERROR || b[2] == B64_ERROR) {
      printf("bad base64 string \n");
      return -1;
    }

    out[n] = (b[0] << 2) | ((b[1] & 0x30) >> 4);
    out[n + 1] = ((b[1] & 0xf) << 4) | ((b[2] & 0x3c) >> 2);
  }

  return decode_len;
}
