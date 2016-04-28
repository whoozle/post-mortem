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
	unsigned version;
	if (fread(&version, sizeof(version), 1, log) != 1)
		perror("fread");

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
		ss << "\"backtrace\": [";
		for(size_t i = 0; i < rec.MaxDepth; ++i)
		{
			_Unwind_Ptr ip = rec.BacktraceData[i];
			if (!ip)
				break;
			if (i)
				ss << ", ";
			ss << ip;
		}
		ss << "], ";
		ss << "\"thread\": " << rec.Thread << ", ";
		ss << "\"ptr\": " << rec.Ptr;
		std::string common = ss.str();

		switch(rec.Type)
		{
		case RecordType::Alloc:
			printf("{\"action\": \"alloc\", \"size\": %lu, %s }", (unsigned long)rec.Size, common.c_str());
			break;
		case RecordType::Free:
			printf("{\"action\": \"free\", %s }", common.c_str());
			break;
		default:
			printf("{\"action\": \"invalid\" }");
		}
	}
	printf("\n]}\n");
	fclose(log);
	return 0;
}