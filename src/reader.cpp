#include <stdio.h>
#include "Record.h"

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

		switch(rec.Type)
		{
		case RecordType::Alloc:
			printf("{\"action\": \"alloc\", \"ptr\": \"%p\", \"size\": %lu}", rec.Ptr, (unsigned long)rec.Size);
			break;
		case RecordType::Free:
			printf("{\"action\": \"free\", \"ptr\": \"%p\" }", rec.Ptr);
			break;
		default:
			printf("{\"action\": \"invalid\" }");
		}
	}
	printf("\n]}\n");
	fclose(log);
	return 0;
}