#define M 256

struct textString{
	int id;
	char string_en[M] ;
	char string_sv[M];
};

int structSize = sizeof(struct textString);