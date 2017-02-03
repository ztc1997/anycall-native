struct decode_result
{
    char *ret;
    int ret_len;
};

char *base64_encode(const unsigned char *data, int data_len);

decode_result base64_decode(const char *data, int data_len);