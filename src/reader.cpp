#include "Record.h"
#include <sstream>
#include <stdio.h>

int main(int argc, char **argv)
{
	FILE *log = fopen(argc < 2? "post-mortem.log": argv[1], "rb");
	if (!log)
	{
		perror("fopen");
		return 1;
	}
	printf("{ \"log\": [\n");
	bool first = true;
	while(true)
	{
		Record rec;
		if (fread(static_cast<void *>(&rec), sizeof(rec), 1, log) != 1)
			break;

		if (!first)
		{
			printf(",\n");
		}
		else
			first = false;

		std::stringstream ss;
		for(size_t i = 0; i < rec.MaxDepth; ++i);

		switch(rec.Type)
		{
		case RecordType::Alloc:
			printf("{\"action\": \"alloc\", \"ptr\": \"%llu\", \"size\": %lu}", static_cast<unsigned long long>(rec.Ptr), (unsigned long)rec.Size);
			break;
		case RecordType::Free:
			printf("{\"action\": \"free\", \"ptr\": \"%llu\" }", static_cast<unsigned long long>(rec.Ptr));
			break;
		default:
			printf("{\"action\": \"invalid\" }");
		}
	}
	printf("\n]}\n");
	fclose(log);
	return 0;
}