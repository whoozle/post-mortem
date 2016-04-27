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
		ss << "]";
		std::string backtrace = ss.str();

		switch(rec.Type)
		{
		case RecordType::Alloc:
			printf("{\"action\": \"alloc\", \"ptr\": \"%llu\", \"size\": %lu, %s }", static_cast<unsigned long long>(rec.Ptr), (unsigned long)rec.Size, backtrace.c_str());
			break;
		case RecordType::Free:
			printf("{\"action\": \"free\", \"ptr\": \"%llu\", %s }", static_cast<unsigned long long>(rec.Ptr), backtrace.c_str());
			break;
		default:
			printf("{\"action\": \"invalid\" }");
		}
	}
	printf("\n]}\n");
	fclose(log);
	return 0;
}