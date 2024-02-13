#include "Json.h"

#pragma warning(disable : 4996)

Output::Json::Json()
{
	outputStr.assign("{\"blueprint\":{\"icons\":[{\"signal\":{\"type\":\"item\",\"name\":\"solar-panel\"},\"index\":1},{\"signal\":{\"type\":\"item\",\"name\":\"accumulator\"},\"index\":2}]}");
}

Output::Json::~Json() {}

void Output::Json::insertEntity(const Entities::Entity* const entity)
{

}

void Output::Json::saveToFile(const char* fileName)
{
	Zpipe zpipe;

	FILE* fileIn;
	fileIn = fopen("test.txt", "rb");

	FILE* fileOut;
	fileOut = fopen("testOut.Z", "w");

	int ret = zpipe.def(fileIn, fileOut, Z_BEST_COMPRESSION);

	if (ret != Z_OK)
	{
		zpipe.zerr(ret);
	}

	fclose(fileIn);
	fclose(fileOut);

	fileIn = fopen("testOut.Z", "rb");
	fileOut = fopen("decompressed.Z", "w");

	ret = zpipe.inf(fileIn, fileOut);

	if (ret != Z_OK)
	{
		zpipe.zerr(ret);
	}

	fclose(fileIn);
	fclose(fileOut);
}

std::string Output::Json::base64_encode(const void* data, const size_t& len)
{
	std::string result((len + 2) / 3 * 4, '=');
	unsigned char* p = (unsigned  char*)data;
	char* str = &result[0];
	size_t j = 0, pad = len % 3;
	const size_t last = len - pad;

	for (size_t i = 0; i < last; i += 3)
	{
		int n = int(p[i]) << 16 | int(p[i + 1]) << 8 | p[i + 2];
		str[j++] = B64chars[n >> 18];
		str[j++] = B64chars[n >> 12 & 0x3F];
		str[j++] = B64chars[n >> 6 & 0x3F];
		str[j++] = B64chars[n & 0x3F];
	}
	if (pad)  /// Set padding
	{
		int n = --pad ? int(p[last]) << 8 | p[last + 1] : p[last];
		str[j++] = B64chars[pad ? n >> 10 & 0x3F : n >> 2];
		str[j++] = B64chars[pad ? n >> 4 & 0x03F : n << 4 & 0x3F];
		str[j++] = pad ? B64chars[n << 2 & 0x3F] : '=';
	}
	return result;
}

